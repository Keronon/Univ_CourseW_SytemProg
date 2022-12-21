#include "GameScene.h"

#include "EndGameScene.h"
#include "PauseScene.h"
#include "PromotionScene.h"

using namespace core;

static void onPromotion(chess::Side side) { PromotionScene::onPromotion(side); }
static void onCheckmate(chess::FullMove move, chess::Side whoWon)
{
	EndGameScene::onCheckmate(move, concat(whoWon, " (", GameScene::instance().getPlayerName(whoWon), ") won!"));
}

static void onStalemate(chess::FullMove move, chess::Side whoCantMove)
{
	EndGameScene::onStalemate(move, concat(whoCantMove, " can't move"));
}
static void onGameDraw(chess::FullMove move, std::string_view why)
{
	EndGameScene::onGameDraw(move, std::string(why));
}
void GameScene::onExecutedMove(chess::FullMove m)
{
	char buff[6] = {};
	m.writeStringAt(buff);
	std::string move(buff);
	return;
}

static void onStepBack()
{
	GameScene::onStepBack();
}
void GameScene::onStepBack()
{

}
void GameScene::onFoundMove(chess::FullMove m)
{
	auto& i = instance();
	i.board.doFullMove(m);
	i.pieceMovingData.setInterpolatedMove(m);
	i.redraw();
}

GameScene::GameScene() : board(onPromotion, onCheckmate, onStalemate, onGameDraw)
{
	showingValidMoves = true;
}

bool GameScene::getShowingValidMoves() { return instance().showingValidMoves; }
void GameScene::setShowingValidMoves(bool val)
{
	instance().showingValidMoves = val;
}
void GameScene::toggleShowingValidMoves()
{
	instance().showingValidMoves = !instance().showingValidMoves;
}

void GameScene::newGameImpl()
{
	cursor = { 4, 0 }; // the king

	playerNames[chess::Side::White] = "Player 1";
	playerNames[chess::Side::Black] = "Player 2";
	selectedPos = chess::Pos::Invalid;
	pieceMovingData.reset();
	board.reset();
	validMoves.clear();
	SceneManager::load(*this);
}

void GameScene::drawBoard(Paint& paint) const
{
	if (isSelected())
	{
		auto pt = boardPosToScreen(selectedPos);
		paint.fillRect({ pt, pt + SquareSize }, SelectedColor);
	}
	BoardDrawingScene::drawBoard(paint);
	if (showingValidMoves)
	{
		for (auto p : validMoves)
		{
			auto pt = boardPosToScreen(p.pos) + SquareSize / 2;
			paint.fillPixelatedCircle(pt, SquareLength / 4, ValidColor, 2);
		}
	}
	spriteOnBoard(paint, cursor, sprites::Cursor, sprites::CursorPalette);
}
void GameScene::moveVert(int dir)
{
	if (whiteSide != chess::Side::White)
		dir *= -1;
	cursor.y() += dir;
	if (cursor.y() < 0)
		cursor.y() = 0;
	if (cursor.y() > 7)
		cursor.y() = 7;
}

bool GameScene::isSelected() const { return selectedPos.isValid(); }
void GameScene::deselect()
{
	validMoves.clear();
	selectedPos = chess::Pos::Invalid;
	redraw();
}
bool GameScene::trySelect(chess::Pos pos)
{
	deselect();
	auto& val = board.at(pos);

	if (!val || val->getSide() != board.getCurrentSide())
		return false;
	val->getValidMoves(pos, board.getState(), validMoves);

	selectedPos = pos;
	return true;
}

void GameScene::spaceAction()
{
	if (board.getCurrentSide() != whiteSide)
	{
		deselect();
		return;
	}

	if (isSelected())
	{
		if (selectedPos == cursor || board.tryMove(selectedPos, cursor, onExecutedMove))
		{
			deselect();
			return;
		}
	}
	trySelect(cursor);
}
void GameScene::onLeftMouseDown(Point pt)
{
	if (getBoardRect().contains(pt))
	{
		auto pos = screenToBoardPos(pt);
		cursor = pos;
		if (isSelected())
		{
			if (selectedPos == cursor ||
				board.tryMove(selectedPos, cursor, onExecutedMove))
			{
				deselect();
				return;
			}
		}
		if (!trySelect(pos))
			return;

		auto offset = pt - boardPosToScreen(pos);
		pieceMovingData.setMousePos(offset, pos);
		pieceMovingData.onMouseMove(pt);

		redraw();
	}
}
void GameScene::onLeftMouseUp(Point pt)
{
	if (pieceMovingData.onMouseMove(pt))
	{
		cursor = screenToBoardPos(pt);

		auto pos = screenToBoardPos(pt);
		cursor = pos;

		if (selectedPos != cursor)
		{
			board.tryMove(selectedPos, cursor, onExecutedMove);
			deselect();
		}

		pieceMovingData.onLeftMouseUp();
		redraw();
	}
}
void GameScene::onMouseMove(Point pt)
{
	if (pieceMovingData.onMouseMove(pt))
	{
		cursor = screenToBoardPos(pt);
		redraw();
	}
}

void GameScene::onKeyDown(char k)
{
	switch (k)
	{
		case VK_ESCAPE:
		case 'Q':
		case 'P':
			SceneManager::load<PauseScene>();
			break;
		case 'A':
		case VK_LEFT:
			if (cursor.x() > 0)
				--cursor.x();
			break;
		case 'D':
		case VK_RIGHT:
			if (cursor.x() < 7)
				++cursor.x();
			break;
		case 'W':
		case VK_UP:
			moveVert(1);
			break;
		case 'S':
		case VK_DOWN:
			moveVert(-1);
			break;
		case VK_RETURN:
		case VK_SPACE:
			spaceAction();
			break;
		default:
			return;
	}
	redraw();
}