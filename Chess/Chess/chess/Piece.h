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
	/// Игровая фигура
	/// </summary>
	class Piece
	{
	protected:
		/// <summary>
		/// Изображение фигуры
		/// </summary>
		using Sprite = core::PaletteSprite;

		/// <summary>
		/// Хранилище доступных ходов
		/// </summary>
		struct ValidMovesHandler
		{
			const BoardState& b;
			std::vector<Move>& res;
			Pos pos;
			Side side;

			/// <summary>
			/// Добавить ход
			/// </summary>
			/// <param name="pos">Позиция хода</param>
			/// <param name="type"> [ ! ] Тип хода ( = Обычный )</param>
			void add(Pos pos, Move::Type type = Move::Type::Normal);

			/// <summary>
			/// Попытаться добавить ход
			/// [ не добавляет если в позиции находится союзная фигура ]
			/// </summary>
			/// <param name="pos">Позиция для хода</param>
			/// <returns>true - если добавлено успешно</returns>
			bool tryAdd(Pos pos);

			/// <summary>
			/// Добавить диагональные ходы
			/// </summary>
			void addDiagonals();

			/// <summary>
			/// Добавить прямые ходы
			/// </summary>
			void addHorizontalAndVertical();
		};

	public:
		constexpr Piece(Side side) : side(side) {}

		virtual ~Piece() noexcept = default;

		/// <summary>
		/// Выдаёт цвет фигуры
		/// </summary>
		/// <returns>Цвет игрока</returns>
		constexpr Side getSide() const { return side; }

		/// <summary>
		/// Выдаёт определение, был ли уже первый ход у фигуры
		/// [ важно для пешек, ладей и короля ]
		/// </summary>
		/// <returns>true - если первых ход был</returns>
		constexpr bool getMadeFirstMove() const { return madeFirstMove; }

		/// <summary>
		/// Переопределяемая функция для получения изображения фигуры
		/// </summary>
		/// <returns>Изображение</returns>
		virtual const Sprite& getSprite() const = 0;

		/// <summary>
		/// Выдаёт палитру фигур игрока
		/// </summary>
		/// <param name="side">Цвет игрока</param>
		/// <returns>Палитра фигур</returns>
		constexpr static const auto& getPalette(Side side)
		{
			if (side == Side::White)
				return sprites::WhitePalette;
			return sprites::BlackPalette;
		}

		/// <summary>
		/// Выдаёт палитру фигур игрока
		/// </summary>
		/// <returns>Палитра фигур</returns>
		constexpr const auto& getPalette() const { return getPalette(side); }

		/// <summary>
		/// Выдаёт доступные ходы фигуры
		/// </summary>
		/// <param name="pos">Позиция фигуры</param>
		/// <param name="b">Состояние поля</param>
		/// <param name="res">Группа для возвращаемых ходов</param>
		void getValidMoves(Pos pos, const BoardState& b, std::vector<Move>& res) const;

		/// <summary>
		/// Выдаёт доступные ходы фигуры без учёта шаха королю
		/// </summary>
		/// <param name="pos">Позиция фигуры</param>
		/// <param name="b">Состояние поля</param>
		/// <param name="res">Группа для возвращаемых ходов</param>
		void getValidMovesDontTestCheck(Pos pos, const BoardState& b, std::vector<Move>& res) const;

		/// <summary>
		/// Обратный вызов при ходе фигурой
		/// </summary>
		void onMoved() { madeFirstMove = true; }

		/// <summary>
		/// Выдаёт букву фигуры
		/// </summary>
		/// <returns>Буква, определяющая фигуру в нотации [ большая - для белого игрока ; маленькая - для чёрного игрока ]</returns>
		char getLetter() const
		{
			char l = getLetterImpl();
			return side == Side::White ? toupper(l) : tolower(l);
		}

	protected:
		/// <summary>
		/// Переопределяемая функция для получения доступных ходов фигуры
		/// </summary>
		/// <param name="vmh">Возвращаемое хранилище для доступных ходов</param>
		virtual void getValidMoves(ValidMovesHandler vmh) const = 0;

		/// <summary>
		/// Переопределяемая функция для получения буквы фигуры
		/// </summary>
		/// <returns>Буква фигуры</returns>
		virtual char getLetterImpl() const = 0;

	private:
		Side side;
		bool madeFirstMove = false;
	};

	/// <summary>
	/// Король [ как игровая фигура ]
	/// </summary>
	class King : public Piece
	{
	public:
		constexpr King(Side side) : Piece(side) {}

		/// <summary>
		/// Выдаёт изображение фигуры
		/// </summary>
		/// <returns></returns>
		const Sprite& getSprite() const override { return sprites::King; }

		/// <summary>
		/// Выдаёт букву фигуры
		/// </summary>
		/// <returns>Буква фигуры</returns>
		char getLetterImpl() const override { return 'K'; }

	protected:
		/// <summary>
		/// Выдаёт доступные ходы фигуры
		/// </summary>
		/// <param name="vmh">Возвращаемое хранилище для доступных ходов</param>
		void getValidMoves(ValidMovesHandler vmh) const override;
	};

	/// <summary>
	/// Королева [ как игровая фигура ]
	/// </summary>
	class Queen : public Piece
	{
	public:
		constexpr Queen(Side side) : Piece(side) {}

		/// <summary>
		/// Выдаёт изображение фигуры
		/// </summary>
		/// <returns></returns>
		const Sprite& getSprite() const override { return sprites::Queen; }

		/// <summary>
		/// Выдаёт букву фигуры
		/// </summary>
		/// <returns>Буква фигуры</returns>
		char getLetterImpl() const override { return 'Q'; }

	protected:
		/// <summary>
		/// Выдаёт доступные ходы фигуры
		/// </summary>
		/// <param name="vmh">Возвращаемое хранилище для доступных ходов</param>
		void getValidMoves(ValidMovesHandler vmh) const override
		{
			vmh.addDiagonals();
			vmh.addHorizontalAndVertical();
		}
	};

	/// <summary>
	/// Ладья [ как игровая фигура ]
	/// </summary>
	class Rook : public Piece
	{
	public:
		constexpr Rook(Side side) : Piece(side) {}

		/// <summary>
		/// Выдаёт изображение фигуры
		/// </summary>
		/// <returns></returns>
		const Sprite& getSprite() const override { return sprites::Rook; }

		/// <summary>
		/// Выдаёт букву фигуры
		/// </summary>
		/// <returns>Буква фигуры</returns>
		char getLetterImpl() const override { return 'R'; }

	protected:
		/// <summary>
		/// Выдаёт доступные ходы фигуры
		/// </summary>
		/// <param name="vmh">Возвращаемое хранилище для доступных ходов</param>
		void getValidMoves(ValidMovesHandler vmh) const override
		{
			vmh.addHorizontalAndVertical();
		}
	};

	/// <summary>
	/// Слон [ как игровая фигура ]
	/// </summary>
	class Bishop : public Piece
	{
	public:
		constexpr Bishop(Side side) : Piece(side) {}

		/// <summary>
		/// Выдаёт изображение фигуры
		/// </summary>
		/// <returns></returns>
		const Sprite& getSprite() const override { return sprites::Bishop; }

		/// <summary>
		/// Выдаёт букву фигуры
		/// </summary>
		/// <returns>Буква фигуры</returns>
		char getLetterImpl() const override { return 'B'; }

	protected:
		/// <summary>
		/// Выдаёт доступные ходы фигуры
		/// </summary>
		/// <param name="vmh">Возвращаемое хранилище для доступных ходов</param>
		void getValidMoves(ValidMovesHandler vmh) const override
		{
			vmh.addDiagonals();
		}
	};

	/// <summary>
	/// Конь [ как игровая фигура ]
	/// </summary>
	class Knight : public Piece
	{
	public:
		constexpr Knight(Side side) : Piece(side) {}

		/// <summary>
		/// Выдаёт изображение фигуры
		/// </summary>
		/// <returns></returns>
		const Sprite& getSprite() const override { return sprites::Knight; }

		/// <summary>
		/// Выдаёт букву фигуры
		/// </summary>
		/// <returns>Буква фигуры</returns>
		char getLetterImpl() const override { return 'N'; }

	protected:
		/// <summary>
		/// Выдаёт доступные ходы фигуры
		/// </summary>
		/// <param name="vmh">Возвращаемое хранилище для доступных ходов</param>
		void getValidMoves(ValidMovesHandler vmh) const override;
	};

	/// <summary>
	/// Пешка [ как игровая фигура ]
	/// </summary>
	class Pawn : public Piece
	{
	public:
		constexpr Pawn(Side side) : Piece(side) {}

		/// <summary>
		/// Выдаёт изображение фигуры
		/// </summary>
		/// <returns></returns>
		const Sprite& getSprite() const override { return sprites::Pawn; }

		/// <summary>
		/// Выдаёт букву фигуры
		/// </summary>
		/// <returns>Буква фигуры</returns>
		char getLetterImpl() const override { return 'P'; }

	protected:
		/// <summary>
		/// Выдаёт доступные ходы фигуры
		/// </summary>
		/// <param name="vmh">Возвращаемое хранилище для доступных ходов</param>
		void getValidMoves(ValidMovesHandler vmh) const override;
	};
}
