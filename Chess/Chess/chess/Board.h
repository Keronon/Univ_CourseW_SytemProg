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
	/// Игровое поле
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
		/// Инициализирует игровое поле с использованием функций обратного вызова
		/// </summary>
		/// <param name="promotionCallback">Обратный вызов при превращении пешки</param>
		/// <param name="checkmateCallback">Обратный вызов при мате</param>
		/// <param name="stalemateCallback">Обратный вызов при тупике</param>
		/// <param name="drawCallback">Обратный вызов при преждевременном соглашении на ничью</param>
		Board(PromotionCallback promotionCallback,
			  CheckmateCallback checkmateCallback,
			  StalemateCallback stalemateCallback,
			  DrawCallback      drawCallback);

		Board(const Board&) = delete;
		Board(Board&&) = delete;
		Board& operator=(const Board&) = delete;
		Board& operator=(Board&&) = delete;

		/// <summary>
		/// Сброс состояния поля
		/// </summary>
		void reset();

		/// <summary>
		/// Обратный вызов при завершении выбора превращения пешки
		/// </summary>
		/// <param name="side">Цвет пешки</param>
		/// <param name="res">Выбранная фигура</param>
		void onGetPromotionResult(Side side, PromotionResult res);

		/// <summary>
		/// Обращение к фигуре на поле по позиции
		/// </summary>
		/// <param name="x">Горизонталь</param>
		/// <param name="y">Вертикаль</param>
		/// <returns>Ссылка на фигуру</returns>
		constexpr std::unique_ptr<Piece>& at(int x, int y)
		{
			return pieces[y * 8 + x];
		}

		/// <summary>
		/// Обращение к фигуре на поле по координатам
		/// </summary>
		/// <param name="x">Горизонталь</param>
		/// <param name="y">Вертикаль</param>
		/// <returns>Ссылка на фигуру</returns>
		constexpr const std::unique_ptr<Piece>& at(int x, int y) const
		{
			return pieces[y * 8 + x];
		}

		/// <summary>
		/// Обращение к фигуре на поле по позиции
		/// </summary>
		/// <param name="p">Позиция</param>
		/// <returns>Ссылка на фигуру</returns>
		constexpr std::unique_ptr<Piece>& at(Pos p) { return at(p.x(), p.y()); }
		constexpr const std::unique_ptr<Piece>& at(Pos p) const
		{
			return at(p.x(), p.y());
		}

		/// <summary>
		/// Пытается сделать ход
		/// </summary>
		/// <param name="from">Начальная позиция хода</param>
		/// <param name="to">Конечная позиция хода</param>
		/// <param name="moveExecutedCallback">[ может быть null ] Обратный вызов при успешном ходе</param>
		/// <returns>true - если ход удался</returns>
		bool tryMove(Pos from, Pos to, MoveExecutedCallback moveExecutedCallback);

		/// <summary>
		/// Выполняет ход [ основано на попытке сделать ход ( tryMove() ) ]
		/// </summary>
		/// <param name="move">Описание хода</param>
		void doFullMove(FullMove move);

		/// <summary>
		/// Определяет игрока, которого сейчас ход
		/// </summary>
		/// <returns>Цвет игрока</returns>
		constexpr Side getCurrentSide() const { return state.currentSide; }

		/// <summary>
		/// Проверка шаха игроку
		/// </summary>
		/// <param name="side">Сторона проверяемого игрока</param>
		/// <returns>true - если есть шах</returns>
		constexpr bool getIsInCheck(Side side) const
		{
			return state.isInCheck[side];
		}

		/// <summary>
		/// Возвращает переменную состояния поля
		/// </summary>
		/// <returns>Состояние поля</returns>
		const auto& getState() const { return state; }

		/// <summary>
		/// Возвращает переменную истории ходов
		/// </summary>
		/// <returns>История ходов</returns>
		const auto& getMoveHistory() const { return moveHistory; }

		/// <summary>
		/// Возвращает переменную, хранящую все съеденные фигуры игроком
		/// </summary>
		/// <param name="side">Цвет игрока</param>
		/// <returns>Список фигур</returns>
		constexpr const auto& getEatenPieces(Side side) const
		{
			return eatenPieces[side];
		}

	private:
		std::array<std::unique_ptr<Piece>, 64> pieces;
		BoardState state;

		/// <summary>
		/// key: состояние поля в FEN ;
		/// value: повторение позиций
		/// </summary>
		std::map<std::string, int> boardHistory;

		SideEntries<std::vector<std::unique_ptr<Piece>>> eatenPieces;

		std::vector<FullMove> moveHistory;

		PromotionCallback promotionCallback;
		FullMove promotionMove;

		// Используется только при превращении
		// (ход выполнен - после того как получен результат превращения)
		MoveExecutedCallback moveExecutedCallback;

		CheckmateCallback checkmateCallback;
		StalemateCallback stalemateCallback;
		DrawCallback      drawCallback;

		/// <summary>
		/// Съесть фигуру в позиции [ основано на текущем игроке ]
		/// </summary>
		/// <param name="p">Позиция поедания</param>
		void eatAt(Pos p);

		/// <summary>
		/// Передвижение фигуры по полю без воздействия на UI
		/// </summary>
		/// <param name="from">Начало передвижения</param>
		/// <param name="to">Конец передвижения</param>
		void moveUnchecked(Pos from, Pos to);

		/// <summary>
		/// Выполняет завершающие действия для хода фигуры
		/// </summary>
		/// <param name="move">Ход</param>
		void finishMove(FullMove move);
	};
}
