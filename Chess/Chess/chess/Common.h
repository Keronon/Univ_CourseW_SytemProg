#pragma once

#include "../core/Utils.h"

#include <array>

namespace chess
{
	/// <summary>
	/// Позиция фигуры на поле
	/// </summary>
	struct Pos
	{
		core::Point val;
		constexpr Pos() : val() {}
		constexpr Pos(int x, int y) : val(x, y) {}
		explicit constexpr Pos(core::Point p) : val(p) {}
		constexpr explicit operator core::Point() const { return val; }

		constexpr int& x()       { return val.x; }
		constexpr int& y()       { return val.y; }
		constexpr int  x() const { return val.x; }
		constexpr int  y() const { return val.y; }

		constexpr         Pos operator+(Pos p) const { return Pos(val + p.val); }
		constexpr         Pos operator-(Pos p) const { return Pos(val - p.val); }
		constexpr core::Point operator*(int i) const { return val * i; }

		friend bool operator ==(Pos a, Pos b) { return a.val == b.val; }
		friend bool operator !=(Pos a, Pos b) { return a.val != b.val; }

		friend std::ostream& operator<<(std::ostream& s, Pos p)
		{
			if (!p.isValid()) return s << '-';
			char buff[3] = {};
			p.writeStringAt(buff);
			return s << buff;
		}

		/// <summary>
		/// Проверяет, попадают ли координаты позиции в рамки игрового поля
		/// </summary>
		/// <returns>true - если такая позиция возможна</returns>
		constexpr bool isValid() const
		{
			return x() >= 0 && x() < 8 && y() >= 0 && y() < 8;
		}

		/// <summary>
		/// Записывает данную позицию в массив символов
		/// как [0]=буква >= 'a' (горизонталь) ; [1]=цифра >= 1 (вертикаль)
		/// </summary>
		/// <param name="c">Массив символов, в который будет записана позиция</param>
		constexpr void writeStringAt(char* c) const
		{
			c[0] = 'a' + x();
			c[1] = '1' + y();
		};

		/// <summary>
		/// Состояние неверной позиции
		/// [ для проверки текущей позиции использовать isValid() ]
		/// </summary>
		static const Pos Invalid;
	};
	inline constexpr Pos Pos::Invalid = { -1, -1 };

	/// <summary>
	/// Гровой ход
	/// </summary>
	struct Move
	{
		/// <summary>
		/// Тип хода { Обычный, Двойной пешечный, Взятие на проходе, Ракировка, Длинная ракировка, Превращение }
		/// </summary>
		enum class Type
		{
			Normal,
			DoubleAdvance,
			Passing,
			Castling,
			QueensideCastling,
			Promotion,
		};

		Pos pos;
		Type type;
		constexpr Move(Pos pos, Type type) : pos(pos), type(type) {}
	};

	/// <summary>
	/// Возможные превращения { Нет, Конь, Слон, Ладья, Королева }
	/// </summary>
	enum class PromotionResult : char
	{
		None = 0,
		Knight = 'n',
		Bishop = 'b',
		Rook = 'r',
		Queen = 'q',
	};

	/// <summary>
	/// Полное описание хода
	/// </summary>
	struct FullMove
	{
		Pos from, to;
		PromotionResult promotionResult;

		constexpr FullMove() : FullMove(Pos::Invalid, Pos::Invalid, {}) {}
		constexpr FullMove(Pos from, Pos to, PromotionResult promotionResult = PromotionResult::None)
			              : from(from), to(to), promotionResult(promotionResult)
		{}

		/// <summary>
		/// Запись хода в виде записи позиций - ДоПослеПревращение
		/// </summary>
		/// <param name="p">Массив символов для записи хода</param>
		constexpr void writeStringAt(char* p) const
		{
			from.writeStringAt(p);
			to.writeStringAt(p + 2);
			p[4] = (char)promotionResult;
		};

		/// <summary>
		/// Запись хода в поток при помощи writeStringAt()
		/// </summary>
		/// <param name="s">Поток вывода</param>
		/// <param name="m">Описание хода</param>
		/// <returns>Поток вывода</returns>
		friend std::ostream& operator<<(std::ostream& s, const FullMove& m)
		{
			char buff[6] = {};
			m.writeStringAt(buff);
			return s << buff;
		}
	};

	/// <summary>
	/// Цвета игроков { Белый, Чёрный }
	/// </summary>
	enum class Side { White = 0, Black = 1 };

	/// <summary>
	/// Запись цвета игрока в виде строки
	/// </summary>
	/// <param name="s">Поток вывода</param>
	/// <param name="side">Цвет игрока</param>
	/// <returns>Поток вывода</returns>
	inline std::ostream& operator<<(std::ostream& s, Side side)
	{
		switch (side)
		{
			case Side::White: return s << "White";
			case Side::Black: return s << "Black";
		}
		return s << "?";
	}

	/// <summary>
	/// Получение другого цвета игрока
	/// </summary>
	/// <param name="s">Цвет игрока</param>
	/// <returns>Другой цвет игрока</returns>
	constexpr Side getOtherSide(Side s)
	{
		return (Side)(!(bool)s);
	}

	/// <summary>
	/// Хранилище для элементов обоих игроков сразу
	/// </summary>
	/// <typeparam name="T">Тип элемента для хранения</typeparam>
	template<typename T>
	class SideEntries
	{
		std::array<T, 2> val;

	public:
		/// <summary>
		/// Получение элементов одного игрока
		/// </summary>
		/// <returns>Массив элементов</returns>
		constexpr auto begin()       { return val.begin(); }

		/// <summary>
		/// Получение элементов одного игрока
		/// </summary>
		/// <returns>Массив элементов</returns>
		constexpr auto begin() const { return val.begin(); }

		/// <summary>
		/// Получение элементов другого игрока
		/// </summary>
		/// <returns>Массив элементов</returns>
		constexpr auto end()         { return val.end(); }

		/// <summary>
		/// Получение элементов другого игрока
		/// </summary>
		/// <returns>Массив элементов</returns>
		constexpr auto end()   const { return val.end(); }

		/// <summary>
		/// Получение элементов одного из игроков по цвету
		/// </summary>
		/// <param name="side">Цвет игрока</param>
		/// <returns>Массив элементов</returns>
		constexpr       T& operator[](Side side)       { return val[(int)side]; }

		/// <summary>
		/// Получение элементов одного из игроков по цвету
		/// </summary>
		/// <param name="side">Цвет игрока</param>
		/// <returns>Массив элементов</returns>
		constexpr const T& operator[](Side side) const { return val[(int)side]; }
	};
}
