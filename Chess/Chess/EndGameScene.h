#pragma once

#include "MenuScene.h"
#include "BoardDrawingScene.h"
#include "chess/Common.h"

#include <chrono>

// Could be done without having 'The Diamond Problem'
// but where's the fun in that?
class EndGameScene : public virtual MenuScene,
	                 public virtual BoardDrawingScene
{
public:
	static EndGameScene& instance()
	{
		static EndGameScene s;
		return s;
	}

	/// <summary>
	/// �������� ����� ��� ����
	/// </summary>
	/// <param name="move">��������� ���</param>
	/// <param name="msg">��������� ��� ������</param>
	static void onCheckmate(chess::FullMove move, const std::string& msg);

	/// <summary>
	/// �������� ����� ��� ������
	/// </summary>
	/// <param name="move">��������� ���</param>
	/// <param name="msg">��������� ��� ������</param>
	static void onStalemate(chess::FullMove move, const std::string& msg);

	/// <summary>
	/// �������� ����� ��� ������
	/// </summary>
	/// <param name="move">��������� ���</param>
	/// <param name="msg">��������� ��� ������</param>
	static void onGameDraw(chess::FullMove move, const std::string& msg);

	/// <summary>
	/// �������� ����� ��� ��������� ������� ����
	/// </summary>
	/// <param name="size">������ ����</param>
	void onSizeChanged(core::Point size) override;

	/// <summary>
	/// �������� ����� ��� ������� �����
	/// </summary>
	void onStart() override;

	/// <summary>
	/// �������� ����� ��� ��������� �����
	/// </summary>
	/// <param name="p">������� ���������</param>
	void onDraw(core::Paint& p) override;

	/// <summary>
	/// �������� ����� ��� ��������� ������� ����
	/// </summary>
	/// <param name="p">������� ���������</param>
	void onDrawBackground(core::Paint& p) override;

	/// <summary>
	/// �������� ����� ��� ������� ������
	/// </summary>
	/// <param name="k">������</param>
	void onKeyDown(char k) override;

	/// <summary>
	/// �������� ����� ��� ������ ���
	/// </summary>
	/// <param name="pos">������� ������</param>
	void onLeftMouseDown(core::Point pos) override;

	/// <summary>
	/// �������� ����� ��� ����� ������ ������ �� �����
	/// </summary>
	/// <param name="i">����� ������</param>
	void onButtonSelected(int i) override;

	/// <summary>
	/// �������� ����� ��� �������� ����
	/// </summary>
	/// <param name="pos">������� �������</param>
	void onMouseMove(core::Point pos) override;

	// // // // // ��������� � ����� ��������� �������� ���� ( BoardDrawingScene )

	/// <summary>
	/// ����� ������� ����
	/// </summary>
	/// <returns>������� ����</returns>
	const chess::Board& getBoard() const override;

	/// <summary>
	/// ����� ��� ������ �� �����
	/// </summary>
	/// <param name="'side'">���� ������</param>
	/// <returns>��� ������</returns>
	const std::string& getPlayerName(chess::Side) const override;

	/// <summary>
	/// ����� ����������� ������ ������
	/// </summary>
	/// <returns>���� ������</returns>
	chess::Side getPlayerSide() const override;

private:
	/// <summary>
	/// ��������� ����� { �������� ������, �����������, �������� ���������, �������� �������� ���� }
	/// </summary>
	enum class State
	{
		ShowingPieceMoving,
		ShowingAnimation,
		FinishedAnimation,
		ViewingBoard,
	};

	EndGameScene();

	std::string title, subtitle;
	std::chrono::time_point<std::chrono::system_clock> t0;
	State state;

	core::RectGroup rects;

	/// <summary>
	/// ��������� �� ��������� �������� ����
	/// </summary>
	/// <returns>��������� �����</returns>
	State returnFromBoardViewing();

	/// <summary>
	/// ���������� �������� �����
	/// </summary>
	/// <param name="move">...</param>
	/// <param name="title">...</param>
	/// <param name="subtitle">...</param>
	void loadImpl(chess::FullMove move,
		          std::string_view title,
		          const std::string& subtitle);

	/// <summary>
	/// ���������� �� onLeftMouseDown() � onKeyDown().
	/// ��������� �������� ������������.
	/// ���� ��������� ����� - �������� �������� ����,
	/// �� �������� �����������.
	/// </summary>
	/// <returns>true - ���� ���� �������� ���� � MenuScene</returns>
	bool checkUserInput();

	/// <summary>
	/// ���������� �������� �����
	/// </summary>
	void finishPieceMoving();

	/// <summary>
	/// ���������� ��������
	/// </summary>
	void finishAnimation();

	/// <summary>
	/// ���������� ������� ����
	/// </summary>
	void viewBoard();
};
