#pragma once

#include "../Sprites.h"
#include "Common.h"

#include <algorithm>
#include <vector>

namespace chess
{
	class Board;
	class BoardState;

	/// <summary>
	/// ������� ������
	/// </summary>
	class Piece
	{
	protected:
		/// <summary>
		/// ����������� ������
		/// </summary>
		using Sprite = core::PaletteSprite;

		/// <summary>
		/// ��������� ��������� �����
		/// </summary>
		struct ValidMovesHandler
		{
			const BoardState& b;
			std::vector<Move>& res;
			Pos pos;
			Side side;

			/// <summary>
			/// �������� ���
			/// </summary>
			/// <param name="pos">������� ����</param>
			/// <param name="type"> [ ! ] ��� ���� ( = ������� )</param>
			void add(Pos pos, Move::Type type = Move::Type::Normal);

			/// <summary>
			/// ���������� �������� ���
			/// [ �� ��������� ���� � ������� ��������� ������� ������ ]
			/// </summary>
			/// <param name="pos">������� ��� ����</param>
			/// <returns>true - ���� ��������� �������</returns>
			bool tryAdd(Pos pos);

			/// <summary>
			/// �������� ������������ ����
			/// </summary>
			void addDiagonals();

			/// <summary>
			/// �������� ������ ����
			/// </summary>
			void addHorizontalAndVertical();
		};

	public:
		constexpr Piece(Side side) : side(side) {}

		virtual ~Piece() noexcept = default;

		/// <summary>
		/// ����� ���� ������
		/// </summary>
		/// <returns>���� ������</returns>
		constexpr Side getSide() const { return side; }

		/// <summary>
		/// ����� �����������, ��� �� ��� ������ ��� � ������
		/// [ ����� ��� �����, ����� � ������ ]
		/// </summary>
		/// <returns>true - ���� ������ ��� ���</returns>
		constexpr bool getMadeFirstMove() const { return madeFirstMove; }

		/// <summary>
		/// ���������������� ������� ��� ��������� ����������� ������
		/// </summary>
		/// <returns>�����������</returns>
		virtual const Sprite& getSprite() const = 0;

		/// <summary>
		/// ����� ������� ����� ������
		/// </summary>
		/// <param name="side">���� ������</param>
		/// <returns>������� �����</returns>
		constexpr static const auto& getPalette(Side side)
		{
			if (side == Side::White)
				return sprites::WhitePalette;
			return sprites::BlackPalette;
		}

		/// <summary>
		/// ����� ������� ����� ������
		/// </summary>
		/// <returns>������� �����</returns>
		constexpr const auto& getPalette() const { return getPalette(side); }

		/// <summary>
		/// ����� ��������� ���� ������
		/// </summary>
		/// <param name="pos">������� ������</param>
		/// <param name="b">��������� ����</param>
		/// <param name="res">������ ��� ������������ �����</param>
		void getValidMoves(Pos pos, const BoardState& b, std::vector<Move>& res) const;

		/// <summary>
		/// ����� ��������� ���� ������ ��� ����� ���� ������
		/// </summary>
		/// <param name="pos">������� ������</param>
		/// <param name="b">��������� ����</param>
		/// <param name="res">������ ��� ������������ �����</param>
		void getValidMovesDontTestCheck(Pos pos, const BoardState& b, std::vector<Move>& res) const;

		/// <summary>
		/// �������� ����� ��� ���� �������
		/// </summary>
		void onMoved() { madeFirstMove = true; }

		/// <summary>
		/// ����� ����� ������
		/// </summary>
		/// <returns>�����, ������������ ������ � ������� [ ������� - ��� ������ ������ ; ��������� - ��� ������� ������ ]</returns>
		char getLetter() const
		{
			char l = getLetterImpl();
			return side == Side::White ? toupper(l) : tolower(l);
		}

	protected:
		/// <summary>
		/// ���������������� ������� ��� ��������� ��������� ����� ������
		/// </summary>
		/// <param name="vmh">������������ ��������� ��� ��������� �����</param>
		virtual void getValidMoves(ValidMovesHandler vmh) const = 0;

		/// <summary>
		/// ���������������� ������� ��� ��������� ����� ������
		/// </summary>
		/// <returns>����� ������</returns>
		virtual char getLetterImpl() const = 0;

	private:
		Side side;
		bool madeFirstMove = false;
	};

	/// <summary>
	/// ������ [ ��� ������� ������ ]
	/// </summary>
	class King : public Piece
	{
	public:
		constexpr King(Side side) : Piece(side) {}

		/// <summary>
		/// ����� ����������� ������
		/// </summary>
		/// <returns></returns>
		const Sprite& getSprite() const override { return sprites::King; }

		/// <summary>
		/// ����� ����� ������
		/// </summary>
		/// <returns>����� ������</returns>
		char getLetterImpl() const override { return 'K'; }

	protected:
		/// <summary>
		/// ����� ��������� ���� ������
		/// </summary>
		/// <param name="vmh">������������ ��������� ��� ��������� �����</param>
		void getValidMoves(ValidMovesHandler vmh) const override;
	};

	/// <summary>
	/// �������� [ ��� ������� ������ ]
	/// </summary>
	class Queen : public Piece
	{
	public:
		constexpr Queen(Side side) : Piece(side) {}

		/// <summary>
		/// ����� ����������� ������
		/// </summary>
		/// <returns></returns>
		const Sprite& getSprite() const override { return sprites::Queen; }

		/// <summary>
		/// ����� ����� ������
		/// </summary>
		/// <returns>����� ������</returns>
		char getLetterImpl() const override { return 'Q'; }

	protected:
		/// <summary>
		/// ����� ��������� ���� ������
		/// </summary>
		/// <param name="vmh">������������ ��������� ��� ��������� �����</param>
		void getValidMoves(ValidMovesHandler vmh) const override
		{
			vmh.addDiagonals();
			vmh.addHorizontalAndVertical();
		}
	};

	/// <summary>
	/// ����� [ ��� ������� ������ ]
	/// </summary>
	class Rook : public Piece
	{
	public:
		constexpr Rook(Side side) : Piece(side) {}

		/// <summary>
		/// ����� ����������� ������
		/// </summary>
		/// <returns></returns>
		const Sprite& getSprite() const override { return sprites::Rook; }

		/// <summary>
		/// ����� ����� ������
		/// </summary>
		/// <returns>����� ������</returns>
		char getLetterImpl() const override { return 'R'; }

	protected:
		/// <summary>
		/// ����� ��������� ���� ������
		/// </summary>
		/// <param name="vmh">������������ ��������� ��� ��������� �����</param>
		void getValidMoves(ValidMovesHandler vmh) const override
		{
			vmh.addHorizontalAndVertical();
		}
	};

	/// <summary>
	/// ���� [ ��� ������� ������ ]
	/// </summary>
	class Bishop : public Piece
	{
	public:
		constexpr Bishop(Side side) : Piece(side) {}

		/// <summary>
		/// ����� ����������� ������
		/// </summary>
		/// <returns></returns>
		const Sprite& getSprite() const override { return sprites::Bishop; }

		/// <summary>
		/// ����� ����� ������
		/// </summary>
		/// <returns>����� ������</returns>
		char getLetterImpl() const override { return 'B'; }

	protected:
		/// <summary>
		/// ����� ��������� ���� ������
		/// </summary>
		/// <param name="vmh">������������ ��������� ��� ��������� �����</param>
		void getValidMoves(ValidMovesHandler vmh) const override
		{
			vmh.addDiagonals();
		}
	};

	/// <summary>
	/// ���� [ ��� ������� ������ ]
	/// </summary>
	class Knight : public Piece
	{
	public:
		constexpr Knight(Side side) : Piece(side) {}

		/// <summary>
		/// ����� ����������� ������
		/// </summary>
		/// <returns></returns>
		const Sprite& getSprite() const override { return sprites::Knight; }

		/// <summary>
		/// ����� ����� ������
		/// </summary>
		/// <returns>����� ������</returns>
		char getLetterImpl() const override { return 'N'; }

	protected:
		/// <summary>
		/// ����� ��������� ���� ������
		/// </summary>
		/// <param name="vmh">������������ ��������� ��� ��������� �����</param>
		void getValidMoves(ValidMovesHandler vmh) const override;
	};

	/// <summary>
	/// ����� [ ��� ������� ������ ]
	/// </summary>
	class Pawn : public Piece
	{
	public:
		constexpr Pawn(Side side) : Piece(side) {}

		/// <summary>
		/// ����� ����������� ������
		/// </summary>
		/// <returns></returns>
		const Sprite& getSprite() const override { return sprites::Pawn; }

		/// <summary>
		/// ����� ����� ������
		/// </summary>
		/// <returns>����� ������</returns>
		char getLetterImpl() const override { return 'P'; }

	protected:
		/// <summary>
		/// ����� ��������� ���� ������
		/// </summary>
		/// <param name="vmh">������������ ��������� ��� ��������� �����</param>
		void getValidMoves(ValidMovesHandler vmh) const override;
	};
}
