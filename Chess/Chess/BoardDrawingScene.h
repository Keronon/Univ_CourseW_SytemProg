#pragma once

#include "SceneCommon.h"
#include "chess/Board.h"

#include <chrono>

/// <summary>
/// ����� � ������� �����
/// </summary>
class BoardDrawingScene : public virtual core::Scene
{
public:
	static core::Rect getBoardRect(core::Point size);

	/// <summary>
	/// �������� ����� ��� ��������� ������� ����
	/// [ ������ ���������� �������� ������� ]
	/// </summary>
	/// <param name="size">������ ����</param>
	void onSizeChanged(core::Point size) override;

	/// <summary>
	/// �������� ����� ��� ��������� ������� ����
	/// [ ������ ���������� �������� ������� ]
	/// </summary>
	/// <param name="paint">������� ���������</param>
	void onDrawBackground(core::Paint& paint) override;

	/// <summary>
	/// �������� ����� ��� ��������� �����
	/// [ ������ ���������� �������� ������� ]
	/// </summary>
	/// <param name="paint"></param>
	void onDraw(core::Paint& paint) override;

protected:
	/// <summary>
	/// ���������������� ������� ��� ������ �������� ����
	/// </summary>
	/// <returns>������� ����</returns>
	virtual const chess::Board& getBoard() const = 0;

	/// <summary>
	/// ���������������� ������� ��� ������ ����� ������
	/// </summary>
	/// <param name="'side'">���� ������</param>
	/// <returns>��� ������</returns>
	virtual const std::string& getPlayerName(chess::Side) const = 0;

	/// <summary>
	/// ���������������� ������� ��� ������ ����������� ������ ������
	/// </summary>
	/// <returns>���� ������</returns>
	virtual chess::Side getPlayerSide() const = 0;

	BoardDrawingScene();

	/// <summary>
	/// ��������������� ��������� �� ������� ���� � �������
	/// </summary>
	/// <param name="boardPos">���������� �� ���� (� �������)</param>
	/// <returns>���������� �� ������ (� ��������)</returns>
	core::Point boardPosToScreen(chess::Pos boardPos) const;

	/// <summary>
	/// ��������������� ������� ��������� �� ������� ����
	/// </summary>
	/// <param name="pt">���������� �� ������ (� ��������)</param>
	/// <returns>���������� �� ���� (� �������)</returns>
	chess::Pos screenToBoardPos(core::Point pt) const;

	/// <summary>
	/// ���������������� ������� ��� ��������� ����� ����
	/// </summary>
	/// <param name="paint">������� ���������</param>
	virtual void drawBoardSquares(core::Paint& paint) const;

	/// <summary>
	/// ���������������� ������� ��� ��������� ����������� �� ���� { A..H, 1..8 }
	/// </summary>
	/// <param name="paint">������� ���������</param>
	virtual void drawBoardMarkings(core::Paint& paint) const;

	/// <summary>
	/// ���������������� ������� ��� ��������� ��������� �����
	/// </summary>
	/// <param name="paint">������� ���������</param>
	virtual void drawEatenPieces(core::Paint& paint) const;

	//must be called

	/// <summary>
	/// ���������������� ������� ��� ��������� �������� ����
	/// [ ������ ���������� �������� ������� ]
	/// </summary>
	/// <param name="paint">������� ���������</param>
	virtual void drawBoard(core::Paint& paint) const;

	/// <summary>
	/// ���������������� ������� ��� ��������� ������ � ����������� �� ���� ������ �� ����
	/// </summary>
	/// <param name="paint">������� ���������</param>
	virtual void drawRightPaneInfo(core::Paint& paint) const;

	/// <summary>
	/// ������������ �����������
	/// </summary>
	/// <param name="paint">������� ���������</param>
	/// <param name="pt">������� ���������</param>
	/// <param name="s">�����������</param>
	/// <param name="palette">�������</param>
	void spriteAt(core::Paint& paint, core::Point pt,
		          const core::PaletteSprite& s,
		          const core::Palette& palette) const;

	/// <summary>
	/// ������������ ����������� �� ������� ����
	/// </summary>
	/// <param name="paint">������� ���������</param>
	/// <param name="p">������� ��������� (� �������)</param>
	/// <param name="s">�����������</param>
	/// <param name="palette">�������</param>
	void spriteOnBoard(core::Paint& paint, chess::Pos p,
		               const core::PaletteSprite& s,
		               const core::Palette& palette) const;

	/// <summary>
	/// ����� ������������� ���� ��� ����������
	/// </summary>
	/// <returns></returns>
	const core::Rect& getBoardRect() const { return boardRect; }

private:
	core::Rect boardRect; // ��� ���������� ( ������ ������� � { 1..8, A..H } ���������)
	core::Rect paneRect;

protected:
	/// <summary>
	/// ������ �������� �����
	/// [ ��� �������� �������� ]
	/// </summary>
	struct PieceMovingData
	{
		/// <summary>
		/// ��� �������� { �� ��������, �������� ����, ������� ����� }
		/// </summary>
		enum class State
		{
			NotMoving,
			Interpolating,
			MouseMoving,
		};

		PieceMovingData(const BoardDrawingScene& parent) : parent(parent) {}

		/// <summary>
		/// ����� ������������
		/// </summary>
		void reset()
		{
			state = State::NotMoving;
			piecePos = chess::Pos::Invalid;
		}

		/// <summary>
		/// ����������, �������� �� ������
		/// </summary>
		/// <returns>true - ���� ��������</returns>
		constexpr bool isMoving() const { return state != State::NotMoving; }

		/// <summary>
		/// ����� ������� ������ (� �������)
		/// </summary>
		/// <returns></returns>
		constexpr chess::Pos getPiecePos() const { return piecePos; }

		/// <summary>
		/// ����� �������� ����� ��������
		/// </summary>
		/// <returns></returns>
		core::Point getPoint();

		/// <summary>
		/// ������������� ����������� �������� ��� ������
		/// </summary>
		/// <param name="move">������� ���</param>
		void setInterpolatedMove(chess::FullMove move);

		/// <summary>
		/// ���������� ����������� �� ������� �� �������
		/// </summary>
		/// <param name="p">������� �������</param>
		/// <param name="boardPos">������ �������� ����</param>
		void setMousePos(core::Point p, chess::Pos boardPos);

		/// <summary>
		/// �������� ����� ��� �������� ����
		/// </summary>
		/// <param name="p">������� �������</param>
		/// <returns>true - ���� ���� ������������ ������</returns>
		bool onMouseMove(core::Point p);

		/// <summary>
		/// �������� ����� ��� ���-�������
		/// </summary>
		void onLeftMouseUp();

		/// <summary>
		/// ��������� �������� ������
		/// </summary>
		void stop();

	private:
		const BoardDrawingScene& parent;
		chess::Pos piecePos = chess::Pos::Invalid;
		State state = State::NotMoving;

		union
		{
			struct { core::Point from,   to; }      interp;
			struct { core::Point offset, current; } mouse;
		};

		std::chrono::time_point<std::chrono::system_clock> t0;
	} pieceMovingData;

private:
	/// <summary>
	/// ����� ������ ��������� � �������� ����
	/// </summary>
	/// <returns>����� ������ �������� ����</returns>
	core::Point boardStart() const { return boardRect.topLeft(); }
};
