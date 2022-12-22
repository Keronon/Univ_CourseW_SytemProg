#pragma once

#include "../core/Utils.h"
#include "Piece.h"
#include "BoardState.h"

#include <memory>
#include <vector>
#include <map>

namespace chess
{
	class Piece;

	/// <summary>
	/// ������� ����
	/// </summary>
	class Board
	{
	public:
		using PromotionCallback = void(*)(Side);
		using CheckmateCallback = void(*)(FullMove, Side whoWon);
		using StalemateCallback = void(*)(FullMove, Side whoCantMove);
		using DrawCallback = void(*)(FullMove, std::string_view why);
		using MoveExecutedCallback = void(*)(FullMove);

		/// <summary>
		/// �������������� ������� ���� � �������������� ������� ��������� ������
		/// </summary>
		/// <param name="promotionCallback">�������� ����� ��� ����������� �����</param>
		/// <param name="checkmateCallback">�������� ����� ��� ����</param>
		/// <param name="stalemateCallback">�������� ����� ��� ������</param>
		/// <param name="drawCallback">�������� ����� ��� ��������������� ���������� �� �����</param>
		Board(PromotionCallback promotionCallback,
			  CheckmateCallback checkmateCallback,
			  StalemateCallback stalemateCallback,
			  DrawCallback      drawCallback);

		Board(const Board&) = delete;
		Board(Board&&) = delete;
		Board& operator=(const Board&) = delete;
		Board& operator=(Board&&) = delete;

		/// <summary>
		/// ����� ��������� ����
		/// </summary>
		void reset();

		/// <summary>
		/// �������� ����� ��� ���������� ������ ����������� �����
		/// </summary>
		/// <param name="side">���� �����</param>
		/// <param name="res">��������� ������</param>
		void onGetPromotionResult(Side side, PromotionResult res);

		/// <summary>
		/// ��������� � ������ �� ���� �� �������
		/// </summary>
		/// <param name="x">�����������</param>
		/// <param name="y">���������</param>
		/// <returns>������ �� ������</returns>
		constexpr std::unique_ptr<Piece>& at(int x, int y)
		{
			return pieces[y * 8 + x];
		}

		/// <summary>
		/// ��������� � ������ �� ���� �� �����������
		/// </summary>
		/// <param name="x">�����������</param>
		/// <param name="y">���������</param>
		/// <returns>������ �� ������</returns>
		constexpr const std::unique_ptr<Piece>& at(int x, int y) const
		{
			return pieces[y * 8 + x];
		}

		/// <summary>
		/// ��������� � ������ �� ���� �� �������
		/// </summary>
		/// <param name="p">�������</param>
		/// <returns>������ �� ������</returns>
		constexpr std::unique_ptr<Piece>& at(Pos p) { return at(p.x(), p.y()); }
		constexpr const std::unique_ptr<Piece>& at(Pos p) const
		{
			return at(p.x(), p.y());
		}

		/// <summary>
		/// �������� ������� ���
		/// </summary>
		/// <param name="from">��������� ������� ����</param>
		/// <param name="to">�������� ������� ����</param>
		/// <param name="moveExecutedCallback">[ ����� ���� null ] �������� ����� ��� �������� ����</param>
		/// <returns>true - ���� ��� ������</returns>
		bool tryMove(Pos from, Pos to, MoveExecutedCallback moveExecutedCallback);

		/// <summary>
		/// ��������� ��� [ �������� �� ������� ������� ��� ( tryMove() ) ]
		/// </summary>
		/// <param name="move">�������� ����</param>
		void doFullMove(FullMove move);

		/// <summary>
		/// ���������� ������, �������� ������ ���
		/// </summary>
		/// <returns>���� ������</returns>
		constexpr Side getCurrentSide() const { return state.currentSide; }

		/// <summary>
		/// �������� ���� ������
		/// </summary>
		/// <param name="side">������� ������������ ������</param>
		/// <returns>true - ���� ���� ���</returns>
		constexpr bool getIsInCheck(Side side) const
		{
			return state.isInCheck[side];
		}

		/// <summary>
		/// ���������� ���������� ��������� ����
		/// </summary>
		/// <returns>��������� ����</returns>
		const auto& getState() const { return state; }

		/// <summary>
		/// ���������� ���������� ������� �����
		/// </summary>
		/// <returns>������� �����</returns>
		const auto& getMoveHistory() const { return moveHistory; }

		/// <summary>
		/// ���������� ����������, �������� ��� ��������� ������ �������
		/// </summary>
		/// <param name="side">���� ������</param>
		/// <returns>������ �����</returns>
		constexpr const auto& getEatenPieces(Side side) const
		{
			return eatenPieces[side];
		}

	private:
		std::array<std::unique_ptr<Piece>, 64> pieces;
		BoardState state;

		/// <summary>
		/// key: ��������� ���� � FEN ;
		/// value: ���������� �������
		/// </summary>
		std::map<std::string, int> boardHistory;

		SideEntries<std::vector<std::unique_ptr<Piece>>> eatenPieces;

		std::vector<FullMove> moveHistory;

		PromotionCallback promotionCallback;
		FullMove promotionMove;

		// ������������ ������ ��� �����������
		// (��� �������� - ����� ���� ��� ������� ��������� �����������)
		MoveExecutedCallback moveExecutedCallback;

		CheckmateCallback checkmateCallback;
		StalemateCallback stalemateCallback;
		DrawCallback      drawCallback;

		/// <summary>
		/// ������ ������ � ������� [ �������� �� ������� ������ ]
		/// </summary>
		/// <param name="p">������� ��������</param>
		void eatAt(Pos p);

		/// <summary>
		/// ������������ ������ �� ���� ��� ����������� �� UI
		/// </summary>
		/// <param name="from">������ ������������</param>
		/// <param name="to">����� ������������</param>
		void moveUnchecked(Pos from, Pos to);

		/// <summary>
		/// ��������� ����������� �������� ��� ���� ������
		/// </summary>
		/// <param name="move">���</param>
		void finishMove(FullMove move);
	};
}
