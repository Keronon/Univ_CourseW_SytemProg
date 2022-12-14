#include "Board.h"

#include "Piece.h"

namespace chess
{
	template <class T>
	void addBoth(Board* p, int x)
	{
		p->at(x, 0) = std::make_unique<T>(Side::White);
		p->at(x, 7) = std::make_unique<T>(Side::Black);
	}

	void Board::reset()
	{
		pieces = {};
		for (auto& vec : eatenPieces)
			vec.clear();
		state.reset();

		for (int i = 0; i < 8; ++i)
		{
			at(i, 1) = std::make_unique<Pawn>(Side::White);
			at(i, 6) = std::make_unique<Pawn>(Side::Black);
		}
		addBoth<Rook>(this, 0);
		addBoth<Rook>(this, 7);

		addBoth<Knight>(this, 1);
		addBoth<Knight>(this, 6);

		addBoth<Bishop>(this, 2);
		addBoth<Bishop>(this, 5);

		addBoth<Queen>(this, 3);
		addBoth<King>(this, 4);

		state.update(pieces);
	}
	Board::Board(PromotionCallback promotionCallback,
		         CheckmateCallback checkmateCallback,
		         StalemateCallback stalemateCallback,
		         DrawCallback      drawCallback) : pieces{},
		         promotionCallback(promotionCallback),
		         moveExecutedCallback(nullptr),
		         checkmateCallback(checkmateCallback),
		         stalemateCallback(stalemateCallback),
		         drawCallback(drawCallback)
	{}

	void Board::finishMove(FullMove move)
	{
		state.update(pieces);

		for (auto side : { Side::White, Side::Black })
		{
			if (!state.isInCheck[side]) continue;
			auto res = state.testWinOrStalemate(side);

			switch (res)
			{
				case GameResult::Win:
					checkmateCallback(move, getOtherSide(side));
					break;
				case GameResult::Stalemate:
					stalemateCallback(move, side);
					break;
				default:
					break;
			}
		}
		std::clog << state << "\n";

		if (state.halfMoveClock >= 100) drawCallback(move, "??????? 50-? ?????"); // 50 moves = 100 half-moves

		auto count = ++boardHistory[state.getShortenedFEN()];

		if (count == 3)
		{
			drawCallback(move, "??????? ??????????");
		}
		moveHistory.push_back(move);
		state.incrementHalfMove();
	}

	void Board::eatAt(Pos p)
	{
		auto t = std::exchange(at(p), nullptr);
		if (t != nullptr)
		{
			state.halfMoveClock = 0;

			eatenPieces[t->getSide()].push_back(std::move(t));
		}
	}

	void Board::onGetPromotionResult(Side side, PromotionResult res)
	{
		using core::concat;

		auto& to = at(promotionMove.to);
		switch (res)
		{
			case PromotionResult::Knight: to = std::make_unique<Knight>(side); break;
			case PromotionResult::Bishop: to = std::make_unique<Bishop>(side); break;
			case PromotionResult::Rook:   to = std::make_unique<Rook>(side); break;
			case PromotionResult::Queen:  to = std::make_unique<Queen>(side); break;
			default:
				throw std::logic_error(concat("invalid promotionResult ", (int)res));
		}
		promotionMove.promotionResult = res;
		finishMove(promotionMove);
		if (moveExecutedCallback)
		{
			moveExecutedCallback(promotionMove);
		}
	}
	void Board::moveUnchecked(Pos from, Pos to)
	{
		eatAt(to);
		at(to) = std::exchange(at(from), nullptr);
		at(to)->onMoved();
	}
	void doNothingPC(Side) {}

	void Board::doFullMove(FullMove move)
	{
		auto pc = promotionCallback;
		promotionCallback = doNothingPC;

		auto doFirstValid = [&]()
		{
			std::vector<Move> validMoves;
			for (int j = 0; j < 8; ++j)
			{
				for (int i = 0; i < 8; ++i)
				{
					auto& ptr = at(i, j);
					if (ptr == nullptr) continue;
					if (ptr->getSide() != getCurrentSide()) continue;
					ptr->getValidMoves(move.from, state, validMoves);
					if (validMoves.empty()) continue;
					auto p = validMoves.front().pos;
					if (tryMove({ i, j }, p, nullptr))
					{
						finishMove({ {i, i}, p });
						std::cout << "OI\n";
						return;
					}
				}
			}
		};

		if (!tryMove(move.from, move.to, nullptr))
		{
			std::clog << "fen " << state << "\n";
			std::cerr << ("Can't do that?! Doing first valid move.\n");
			doFirstValid();
		}
		if (move.promotionResult != PromotionResult::None)
		{
			onGetPromotionResult(getCurrentSide(), move.promotionResult);
		}

		promotionCallback = pc;
	}

	bool Board::tryMove(Pos from, Pos to, MoveExecutedCallback callback)
	{
		auto& ptr = at(from);
		if (ptr == nullptr)
			return false;

		std::vector<Move> validMoves;
		ptr->getValidMoves(from, state, validMoves);

		auto it = std::find_if(validMoves.begin(), validMoves.end(),
			[&](Move m) { return m.pos == to; });
		if (it == validMoves.end())
			return false;

		FullMove move{ from, to };

		if (dynamic_cast<const Pawn*>(ptr.get()) != nullptr)
			state.halfMoveClock = 0;

		if (it->type != Move::Type::Passing)
			state.passingTarget = Pos::Invalid;

		switch (it->type)
		{
			case Move::Type::DoubleAdvance:
			{
				state.passingTarget = to;
			} break;
			case Move::Type::Passing:
			{
				eatAt(state.passingTarget);
			} break;
			case Move::Type::Promotion:
				promotionMove = move;
				this->moveExecutedCallback = callback;
				promotionCallback(state.currentSide);
				break;
			case Move::Type::QueensideCastling:
				moveUnchecked({ 0, from.y() }, from - Pos{ 1, 0 });
				break;
			case Move::Type::Castling:
				moveUnchecked({ 7, from.y() }, from + Pos{ 1, 0 });
				break;
			case Move::Type::Normal:
				break;
		}

		moveUnchecked(from, to);

		if (it->type != Move::Type::Promotion)
		{
			finishMove(move);
			if (callback)
				callback(move);
		}

		return true;
	}
}
