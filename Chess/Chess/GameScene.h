#pragma once

#include "BoardDrawingScene.h"
#include "chess/Board.h"

#include <chrono>

class GameScene : public BoardDrawingScene
{
public:
	static GameScene& instance()
	{
		static GameScene s;
		return s;
	}

	static void newGame() { instance().newGameImpl(); }
	static void onStepBack();

	void drawBoard(core::Paint& paint) const override;

	void onKeyDown(char k) override;

	void onLeftMouseUp(core::Point pos) override;
	void onLeftMouseDown(core::Point pos) override;
	void onMouseMove(core::Point pos) override;

	static bool getShowingValidMoves();
	static void setShowingValidMoves(bool val);
	static void toggleShowingValidMoves();

	chess::Board& getBoard() { return board; }

	const chess::Board& getBoard() const override { return board; }
	chess::Side getPlayerSide() const override { return whiteSide; }
	const std::string& getPlayerName(chess::Side s) const override
	{
		return playerNames[s];
	}

private:
	GameScene();
	void newGameImpl();

	chess::Side whiteSide;
	chess::SideEntries<std::string> playerNames;

	chess::Board board;

	chess::Pos selectedPos;
	chess::Pos cursor;
	std::vector<chess::Move> validMoves;

	bool showingValidMoves;

	bool isSelected() const;
	void deselect();

	// Fails if there's no piece at pos or
	// if the piece is not the correct side
	bool trySelect(chess::Pos pos);

	void moveVert(int dir);
	void spaceAction();

	static void onExecutedMove(chess::FullMove m);
	static void onFoundMove(chess::FullMove m);
};
