#pragma once

#include "BoardDrawingScene.h"
#include "chess/Board.h"

#include <chrono>

/// <summary>
/// �������� ����� ����
/// </summary>
class GameScene : public BoardDrawingScene
{
public:
	static GameScene& instance()
	{
		static GameScene s;
		return s;
	}

	/// <summary>
	/// ������ ����� ����
	/// </summary>
	static void newGame() { instance().newGameImpl(); }

	/// <summary>
	/// ��������� �������� ����
	/// </summary>
	/// <param name="paint">������� ���������</param>
	void drawBoard(core::Paint& paint) const override;

	/// <summary>
	/// �������� ����� ��� ������� �������
	/// </summary>
	/// <param name="k"></param>
	void onKeyDown(char k) override;

	/// <summary>
	/// �������� ����� ��� ������� ���
	/// </summary>
	/// <param name="pos">������� �������</param>
	void onLeftMouseUp(core::Point pos) override;

	/// <summary>
	/// �������� ����� ��� ������ ���
	/// </summary>
	/// <param name="pos">������� ������</param>
	void onLeftMouseDown(core::Point pos) override;

	/// <summary>
	/// �������� ����� ��� �������� ����
	/// </summary>
	/// <param name="pos">������� �������</param>
	void onMouseMove(core::Point pos) override;

	/// <summary>
	/// ����� ����������� ��������� �����
	/// </summary>
	/// <returns>true - ���� ��������� ���� ���� ����������</returns>
	static bool getShowingValidMoves();

	/// <summary>
	/// ������������� ����������� ��������� �����
	/// </summary>
	/// <param name="val">true - ���� ��������� ���� ���� ����������</param>
	static void setShowingValidMoves(bool val);

	/// <summary>
	/// ����������� ����������� ��������� �����
	/// </summary>
	static void toggleShowingValidMoves();

	/// <summary>
	/// ����� ������� ����
	/// </summary>
	/// <returns>������� ����</returns>
	chess::Board& getBoard() { return board; }

	/// <summary>
	/// ����� ������� ����
	/// </summary>
	/// <returns>������� ����</returns>
	const chess::Board& getBoard() const override { return board; }

	/// <summary>
	/// ����� ����������� ������ ������
	/// </summary>
	/// <returns>���� ������</returns>
	chess::Side getPlayerSide() const override { return chess::Side::White; }

	/// <summary>
	/// ����� ��� ������ �� �����
	/// </summary>
	/// <param name="'side'">���� ������</param>
	/// <returns>��� ������</returns>
	const std::string& getPlayerName(chess::Side s) const override
	{
		return playerNames[s];
	}

private:
	GameScene();

	/// <summary>
	/// ���������� ������� ����� ����
	/// </summary>
	void newGameImpl();

	chess::SideEntries<std::string> playerNames;

	chess::Board board;

	chess::Pos selectedPos;
	chess::Pos cursor;
	std::vector<chess::Move> validMoves;

	bool showingValidMoves;

	/// <summary>
	/// ����������, ���� �� ���������� ������ �� ����
	/// </summary>
	/// <returns>true - ���� ����</returns>
	bool isSelected() const;

	/// <summary>
	/// ������� ��������� � �����
	/// </summary>
	void deselect();

	/// <summary>
	/// ������������� ��������� �� ������
	/// </summary>
	/// <param name="pos">������� ������</param>
	/// <returns>true - ���� � ������ ���� ������ � ��� ����� �������� ������</returns>
	bool trySelect(chess::Pos pos);

	/// <summary>
	/// ������� ������ ������ �����������
	/// </summary>
	/// <param name="dir">����������� ������������ ������� { 1 (�����) \ -1 (����) }</param>
	void moveVert(int dir);

	/// <summary>
	/// ��������� ��������
	/// [ �������� ������ \ ������� ��� ]
	/// </summary>
	void spaceAction();

	/// <summary>
	/// �������� �����, ����� ��� �������� ���
	/// </summary>
	/// <param name="m">������� ���</param>
	static void onExecutedMove(chess::FullMove m);

	/// <summary>
	/// �������� �����, ����� ��� ������ ��� �����
	/// [ ���������� ��� ���������� ���� � �� ]
	/// </summary>
	/// <param name="m"></param>
	static void onFoundMove(chess::FullMove m);
};
