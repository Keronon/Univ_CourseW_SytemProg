#pragma once

#include "../core/Utils.h"

#include <array>

namespace chess
{
	/// <summary>
	/// ������� ������ �� ����
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
		/// ���������, �������� �� ���������� ������� � ����� �������� ����
		/// </summary>
		/// <returns>true - ���� ����� ������� ��������</returns>
		constexpr bool isValid() const
		{
			return x() >= 0 && x() < 8 && y() >= 0 && y() < 8;
		}

		/// <summary>
		/// ���������� ������ ������� � ������ ��������
		/// ��� [0]=����� >= 'a' (�����������) ; [1]=����� >= 1 (���������)
		/// </summary>
		/// <param name="c">������ ��������, � ������� ����� �������� �������</param>
		constexpr void writeStringAt(char* c) const
		{
			c[0] = 'a' + x();
			c[1] = '1' + y();
		};

		/// <summary>
		/// ��������� �������� �������
		/// [ ��� �������� ������� ������� ������������ isValid() ]
		/// </summary>
		static const Pos Invalid;
	};
	inline constexpr Pos Pos::Invalid = { -1, -1 };

	/// <summary>
	/// ������ ���
	/// </summary>
	struct Move
	{
		/// <summary>
		/// ��� ���� { �������, ������� ��������, ������ �� �������, ���������, ������� ���������, ����������� }
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
	/// ��������� ����������� { ���, ����, ����, �����, �������� }
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
	/// ������ �������� ����
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
		/// ������ ���� � ���� ������ ������� - ������������������
		/// </summary>
		/// <param name="p">������ �������� ��� ������ ����</param>
		constexpr void writeStringAt(char* p) const
		{
			from.writeStringAt(p);
			to.writeStringAt(p + 2);
			p[4] = (char)promotionResult;
		};

		/// <summary>
		/// ������ ���� � ����� ��� ������ writeStringAt()
		/// </summary>
		/// <param name="s">����� ������</param>
		/// <param name="m">�������� ����</param>
		/// <returns>����� ������</returns>
		friend std::ostream& operator<<(std::ostream& s, const FullMove& m)
		{
			char buff[6] = {};
			m.writeStringAt(buff);
			return s << buff;
		}
	};

	/// <summary>
	/// ����� ������� { �����, ׸���� }
	/// </summary>
	enum class Side { White = 0, Black = 1 };

	/// <summary>
	/// ������ ����� ������ � ���� ������
	/// </summary>
	/// <param name="s">����� ������</param>
	/// <param name="side">���� ������</param>
	/// <returns>����� ������</returns>
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
	/// ��������� ������� ����� ������
	/// </summary>
	/// <param name="s">���� ������</param>
	/// <returns>������ ���� ������</returns>
	constexpr Side getOtherSide(Side s)
	{
		return (Side)(!(bool)s);
	}

	/// <summary>
	/// ��������� ��� ��������� ����� ������� �����
	/// </summary>
	/// <typeparam name="T">��� �������� ��� ��������</typeparam>
	template<typename T>
	class SideEntries
	{
		std::array<T, 2> val;

	public:
		/// <summary>
		/// ��������� ��������� ������ ������
		/// </summary>
		/// <returns>������ ���������</returns>
		constexpr auto begin()       { return val.begin(); }

		/// <summary>
		/// ��������� ��������� ������ ������
		/// </summary>
		/// <returns>������ ���������</returns>
		constexpr auto begin() const { return val.begin(); }

		/// <summary>
		/// ��������� ��������� ������� ������
		/// </summary>
		/// <returns>������ ���������</returns>
		constexpr auto end()         { return val.end(); }

		/// <summary>
		/// ��������� ��������� ������� ������
		/// </summary>
		/// <returns>������ ���������</returns>
		constexpr auto end()   const { return val.end(); }

		/// <summary>
		/// ��������� ��������� ������ �� ������� �� �����
		/// </summary>
		/// <param name="side">���� ������</param>
		/// <returns>������ ���������</returns>
		constexpr       T& operator[](Side side)       { return val[(int)side]; }

		/// <summary>
		/// ��������� ��������� ������ �� ������� �� �����
		/// </summary>
		/// <param name="side">���� ������</param>
		/// <returns>������ ���������</returns>
		constexpr const T& operator[](Side side) const { return val[(int)side]; }
	};
}
