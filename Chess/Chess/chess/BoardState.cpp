#include "BoardState.h"

#include "Piece.h"

#include <sstream>

namespace chess
{
	void BoardState::reset()
	{
		val = {};
		halfMoveClock = 0;

		moveCounter = 1;
		currentSide = Side::White;

		passingTarget = Pos::Invalid;
	}

	void BoardState::update(const std::array<std::unique_ptr<Piece>, 64>& pieces)
	{
		for (int i = 0; i < 64; ++i)
			val[i] = pieces[i].get();
		update();
	}
	void BoardState::update()
	{
		std::vector<Move> validMoves;
		for (int j = 0; j < 8; ++j)
		{
			for (int i = 0; i < 8; ++i)
			{
				auto* ptr = at(i, j);
				if (ptr == nullptr)
					continue;
				if (auto* king = dynamic_cast<const King*>(ptr))
				{
					kingPos[king->getSide()] = { i, j };
				}
			}
		}

		isInCheck[Side::White] = isInCheck[Side::Black] = false;
		for (int j = 0; j < 8; ++j)
		{
			for (int i = 0; i < 8; ++i)
			{
				auto* ptr = at(i, j);
				if (ptr == nullptr)
					continue;

				ptr->getValidMovesDontTestCheck({ i, j }, *this, validMoves);
				auto side = getOtherSide(ptr->getSide());
				auto pos = kingPos[side];
				for (auto m : validMoves)
				{
					if (m.pos == pos)
					{
						isInCheck[side] = true;
					}
				}
			}
		}
	}

	GameResult BoardState::testWinOrStalemate(Side side) const
	{
		std::vector<Move> validMoves;
		for (int j = 0; j < 8; ++j)
		{
			for (int i = 0; i < 8; ++i)
			{
				auto* ptr = at(i, j);
				if (ptr == nullptr || ptr->getSide() != side)
					continue;
				ptr->getValidMoves({ i, j }, *this, validMoves);
				if (validMoves.size() > 0) return GameResult::Continue;
			}
		}
		return isInCheck[side] ? GameResult::Win : GameResult::Stalemate;
	}

	bool BoardState::moveLeavesInCheck(Pos from, Pos to) const
	{
		auto* ptr = at(from);
		if (ptr == nullptr)
			throw std::logic_error("moveEscapesCheck has ptr == null");

		auto side = ptr->getSide();

		BoardState state = *this;

		state.at(to) = state.at(from);
		state.at(from) = nullptr;
		state.update();

		return state.isInCheck[side];
	}

	std::string BoardState::getFEN() const
	{
		std::stringstream ss;
		ss << *this;
		return ss.str();
	}
	std::string BoardState::getShortenedFEN() const
	{
		std::stringstream ss;
		shortenedFenImpl(ss);
		return ss.str();
	}

	std::ostream& operator<<(std::ostream& s, const BoardState& b)
	{
		b.shortenedFenImpl(s);
		return s << ' ' << b.halfMoveClock << ' ' << b.moveCounter;
	}

	std::ostream& BoardState::shortenedFenImpl(std::ostream& s) const
	{
		// board pieces
		for (int j = 7;; --j)
		{
			int emptyCount = 0;

			for (int i = 0; i < 8; ++i)
			{
				auto* ptr = at(i, j);

				if (ptr == nullptr)
				{
					++emptyCount;
				}
				else
				{
					if (emptyCount != 0)
					{
						s << (char)('0' + emptyCount);
						emptyCount = 0;
					}
					s << ptr->getLetter();
				}
			}

			if (emptyCount != 0)
			{
				s << (char)('0' + emptyCount);
			}
			if (j == 0) break;
			s << '/';
		}

		s << ' ' << (currentSide == Side::White ? 'w' : 'b') << ' ';

		auto kingCanCastle = [&](int x, int y) // ракировка
		{
			if (auto* ptr = at(x, y))
			{
				if (auto* king = dynamic_cast<const King*>(ptr)) // проверка на Короля
				{
					return !king->getMadeFirstMove();
				}
			}
			return false;
		};

		auto rookCanCastle = [&](int x, int y, char c) // return: true - если можно ракироваться с данной ладьёй. добавляет букву ракировки в поток вывода
		{
			if (auto* ptr = at(x, y))
			{
				if (auto* rook = dynamic_cast<const Rook*>(ptr))
				{
					if (!rook->getMadeFirstMove())
					{
						s << c;
						return true;
					}
				}
			}
			return false;
		};

		auto checkCastlingForSide = [&](int y, char q, char k) -> bool
		{
			if (kingCanCastle(4, y))
			{
				return rookCanCastle(7, y, k) || rookCanCastle(0, y, q);
			}
			return false;
		};

		if (!checkCastlingForSide(0, 'Q', 'K') &&
			!checkCastlingForSide(7, 'q', 'k'))
		{
			s << '-'; // если ракировок нет
		}

		return s << ' ' << passingTarget; // пешка после широкого шага
	}
}
