#pragma once

#include "../core/Utils.h"

#include "Piece.h"

#include <array>

namespace chess
{
	/// <summary>
	/// ��������� ������� ���� { ���������� , ������ , ����� , ����� }
	/// </summary>
	enum class GameResult
	{
		Continue, Win, Stalemate, Draw
	};

	/// <summary>
	/// ��������� �������� ����
	/// </summary>
	class BoardState
	{
	public:
		BoardState() = default;

		/// <summary>
		/// �������� ������ �� �������
		/// </summary>
		/// <param name="p">�������</param>
		/// <returns>������ �� ������</returns>
		constexpr const Piece*& at(Pos p)              { return at(p.x(), p.y()); }

		/// <summary>
		/// �������� ������ �� �������
		/// </summary>
		/// <param name="p">�������</param>
		/// <returns>������ �� ������</returns>
		constexpr const Piece*  at(Pos p)        const { return at(p.x(), p.y()); }

		/// <summary>
		/// �������� ������ �� �����������
		/// </summary>
		/// <param name="x">���������</param>
		/// <param name="y">�����������</param>
		/// <returns>������ �� ������</returns>
		constexpr const Piece*& at(int x, int y)       { return val[y * 8 + x]; }

		/// <summary>
		/// �������� ������ �� �����������
		/// </summary>
		/// <param name="x">���������</param>
		/// <param name="y">�����������</param>
		/// <returns>������ �� ������</returns>
		constexpr const Piece*  at(int x, int y) const { return val[y * 8 + x]; }

		/// <summary>
		/// ����� ��������� ����
		/// </summary>
		void reset();

		/// <summary>
		/// �������� ��������� ����
		/// </summary>
		void update();

		/// <summary>
		/// ���������� ��������� ���� � ������������ � ���������� ��������
		/// </summary>
		/// <param name="pieces">������</param>
		void update(const std::array<std::unique_ptr<Piece>, 64>& pieces);

		/// <summary>
		/// �������� ������������ ���� ��� ����
		/// </summary>
		/// <param name="from">��������� ������� ����</param>
		/// <param name="to">�������� ������� ����</param>
		/// <returns>true - ���� ����� ���� ��� ����������</returns>
		bool moveLeavesInCheck(Pos from, Pos to) const;

		GameResult testWinOrStalemate(Side s) const;

		/// <summary>
		/// ���������� ����������� ������ ��������� ���� (Forsyth-Edwards Notation)
		/// </summary>
		/// <returns>������ � ���������� ����</returns>
		std::string getFEN() const;

		// Like FEN but don't print the conters

		/// <summary>
		/// ����������� ������ ��������� ���� ( �� �������� �������������� ������ � ����� )
		/// </summary>
		/// <returns>������ � ������� ���������� ����</returns>
		std::string getShortenedFEN() const;

		/// <summary>
		/// ��� ������ ��������� ���� � �����
		/// </summary>
		/// <param name="s">����� ������</param>
		/// <param name="b">��������� ���� ( &this )</param>
		/// <returns>����� ������</returns>
		friend std::ostream& operator<<(std::ostream& s, const BoardState& b);

		/// <summary>
		/// ���������� ���� �������� ���� �����
		/// </summary>
		/// <returns>������� ����</returns>
		constexpr Pos getPassingTarget() const
		{
			return passingTarget;
		}

	private:
		std::array<const Piece*, 64> val;
		SideEntries<bool> isInCheck;
		SideEntries<Pos> kingPos;

		/// <summary>
		/// ���� ������� "50-� �����" :
		/// ���� � ������ ��������� 50 �����,
		/// �� ��������������� ����� ����������
		/// </summary>
		int halfMoveClock = 0;

		// It's just used for the FEN
		// It starts at 1, and is incremented after Black's move.

		/// <summary>
		/// ������������ ��� ������ ��������� ����.
		/// ���� ��� - �������� ����� �������
		/// </summary>
		int moveCounter = 1;

		Side currentSide = Side::White;

		Pos passingTarget = Pos::Invalid;

		/// <summary>
		/// �������� ������� ��������� � �����
		/// </summary>
		void incrementHalfMove()
		{
			++halfMoveClock;
			if (currentSide != Side::White)
				++moveCounter;
			currentSide = getOtherSide(currentSide);
		}

		/// <summary>
		/// ����� � ����� ������� ������ ��������� ����
		/// </summary>
		/// <param name="s">����� ������</param>
		/// <returns>����� ������</returns>
		std::ostream& shortenedFenImpl(std::ostream& s) const;

		// ������������� ������ ������ �������� ����
		friend class Board;
	};
}
