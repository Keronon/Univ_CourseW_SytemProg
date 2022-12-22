#pragma once

#include "../core/Utils.h"

#include "Piece.h"

#include <array>

namespace chess
{
	/// <summary>
	/// Возможные статусы игры { Продолжить , Победа , Тупик , Ничья }
	/// </summary>
	enum class GameResult
	{
		Continue, Win, Stalemate, Draw
	};

	/// <summary>
	/// Состояние игрового поля
	/// </summary>
	class BoardState
	{
	public:
		BoardState() = default;

		/// <summary>
		/// Получить фигуру по позиции
		/// </summary>
		/// <param name="p">Позиция</param>
		/// <returns>Ссылка на фигуру</returns>
		constexpr const Piece*& at(Pos p)              { return at(p.x(), p.y()); }

		/// <summary>
		/// Получить фигуру по позиции
		/// </summary>
		/// <param name="p">Позиция</param>
		/// <returns>Ссылка на фигуру</returns>
		constexpr const Piece*  at(Pos p)        const { return at(p.x(), p.y()); }

		/// <summary>
		/// Получить фигуру по координатам
		/// </summary>
		/// <param name="x">Вертикаль</param>
		/// <param name="y">Горизонталь</param>
		/// <returns>Ссылка на фигуру</returns>
		constexpr const Piece*& at(int x, int y)       { return val[y * 8 + x]; }

		/// <summary>
		/// Получить фигуру по координатам
		/// </summary>
		/// <param name="x">Вертикаль</param>
		/// <param name="y">Горизонталь</param>
		/// <returns>Ссылка на фигуру</returns>
		constexpr const Piece*  at(int x, int y) const { return val[y * 8 + x]; }

		/// <summary>
		/// Сброс состояния поля
		/// </summary>
		void reset();

		/// <summary>
		/// Обновить состояние поля
		/// </summary>
		void update();

		/// <summary>
		/// Обновление состояния поля в соответствии с имеющимися фигурами
		/// </summary>
		/// <param name="pieces">Фигуры</param>
		void update(const std::array<std::unique_ptr<Piece>, 64>& pieces);

		/// <summary>
		/// Проверка допустимости хода при шахе
		/// </summary>
		/// <param name="from">Начальная позиция хода</param>
		/// <param name="to">Конечная позиция хода</param>
		/// <returns>true - если после хода шах сохранится</returns>
		bool moveLeavesInCheck(Pos from, Pos to) const;

		GameResult testWinOrStalemate(Side s) const;

		/// <summary>
		/// Возвращает специальную запись состояния поля (Forsyth-Edwards Notation)
		/// </summary>
		/// <returns>Строка с состоянием поля</returns>
		std::string getFEN() const;

		// Like FEN but don't print the conters

		/// <summary>
		/// Укороченная запись состояния поля ( не включает дополнительных данных о ходах )
		/// </summary>
		/// <returns>Строка с кратким состоянием поля</returns>
		std::string getShortenedFEN() const;

		/// <summary>
		/// Для вывода состояния поля в поток
		/// </summary>
		/// <param name="s">Поток вывода</param>
		/// <param name="b">Состояние поля ( &this )</param>
		/// <returns>Поток вывода</returns>
		friend std::ostream& operator<<(std::ostream& s, const BoardState& b);

		/// <summary>
		/// Определяет цель широкого хода пешек
		/// </summary>
		/// <returns>Позиция хода</returns>
		constexpr Pos getPassingTarget() const
		{
			return passingTarget;
		}

	private:
		std::array<const Piece*, 64> val;
		SideEntries<bool> isInCheck;
		SideEntries<Pos> kingPos;

		/// <summary>
		/// Учёт правила "50-и ходов" :
		/// Если в партии разыграно 50 ходов,
		/// то преждевременная ничья невозможна
		/// </summary>
		int halfMoveClock = 0;

		// It's just used for the FEN
		// It starts at 1, and is incremented after Black's move.

		/// <summary>
		/// Используется для записи состояния поля.
		/// Одни ход - действия обоих игроков
		/// </summary>
		int moveCounter = 1;

		Side currentSide = Side::White;

		Pos passingTarget = Pos::Invalid;

		/// <summary>
		/// Повышает подсчёт полуходов и ходов
		/// </summary>
		void incrementHalfMove()
		{
			++halfMoveClock;
			if (currentSide != Side::White)
				++moveCounter;
			currentSide = getOtherSide(currentSide);
		}

		/// <summary>
		/// Вывод в поток краткой записи состояния поля
		/// </summary>
		/// <param name="s">Поток вывода</param>
		/// <returns>Поток вывода</returns>
		std::ostream& shortenedFenImpl(std::ostream& s) const;

		// Предоставляем полный доступ объектам поля
		friend class Board;
	};
}
