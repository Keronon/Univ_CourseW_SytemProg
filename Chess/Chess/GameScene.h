#pragma once

#include "BoardDrawingScene.h"
#include "chess/Board.h"

#include <chrono>

/// <summary>
/// Основная сцена игры
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
	/// Запуск новой игры
	/// </summary>
	static void newGame() { instance().newGameImpl(); }

	/// <summary>
	/// Отрисовка игрового поля
	/// </summary>
	/// <param name="paint">Область отрисовки</param>
	void drawBoard(core::Paint& paint) const override;

	/// <summary>
	/// Обратный вызов при нажатии клавиши
	/// </summary>
	/// <param name="k"></param>
	void onKeyDown(char k) override;

	/// <summary>
	/// Обратный вызов при отжатии ЛКМ
	/// </summary>
	/// <param name="pos">Позиция отжатия</param>
	void onLeftMouseUp(core::Point pos) override;

	/// <summary>
	/// Обратный вызов при вжатии ЛКМ
	/// </summary>
	/// <param name="pos">Позиция вжатия</param>
	void onLeftMouseDown(core::Point pos) override;

	/// <summary>
	/// Обратный вызов при движении мыши
	/// </summary>
	/// <param name="pos">Позиция курсора</param>
	void onMouseMove(core::Point pos) override;

	/// <summary>
	/// Выдаёт отображение возможных ходов
	/// </summary>
	/// <returns>true - если возможные ходы надо отображать</returns>
	static bool getShowingValidMoves();

	/// <summary>
	/// Устанавливает отображение возможных ходов
	/// </summary>
	/// <param name="val">true - если возможные ходы надо отображать</param>
	static void setShowingValidMoves(bool val);

	/// <summary>
	/// Переключает отображение возможных ходов
	/// </summary>
	static void toggleShowingValidMoves();

	/// <summary>
	/// Выдаёт игровое поле
	/// </summary>
	/// <returns>Игровое поле</returns>
	chess::Board& getBoard() { return board; }

	/// <summary>
	/// Выдаёт игровое поле
	/// </summary>
	/// <returns>Игровое поле</returns>
	const chess::Board& getBoard() const override { return board; }

	/// <summary>
	/// Выдаёт определение белого игрока
	/// </summary>
	/// <returns>Цвет игрока</returns>
	chess::Side getPlayerSide() const override { return chess::Side::White; }

	/// <summary>
	/// Выдаёт имя игрока по цвету
	/// </summary>
	/// <param name="'side'">Цвет игрока</param>
	/// <returns>Имя игрока</returns>
	const std::string& getPlayerName(chess::Side s) const override
	{
		return playerNames[s];
	}

private:
	GameScene();

	/// <summary>
	/// Реализация запуска новой игры
	/// </summary>
	void newGameImpl();

	chess::SideEntries<std::string> playerNames;

	chess::Board board;

	chess::Pos selectedPos;
	chess::Pos cursor;
	std::vector<chess::Move> validMoves;

	bool showingValidMoves;

	/// <summary>
	/// Определяет, есть ли выделенная ячейка на поле
	/// </summary>
	/// <returns>true - если есть</returns>
	bool isSelected() const;

	/// <summary>
	/// Снимает выделение с ячеек
	/// </summary>
	void deselect();

	/// <summary>
	/// Устанавливает выделение на ячейку
	/// </summary>
	/// <param name="pos">Позиция ячейки</param>
	/// <returns>true - если в ячейке есть фигура и она цвета ходящего игрока</returns>
	bool trySelect(chess::Pos pos);

	/// <summary>
	/// Двигает курсор ячейки вертикально
	/// </summary>
	/// <param name="dir">Направление перестановки курсора { 1 (вверх) \ -1 (вниз) }</param>
	void moveVert(int dir);

	/// <summary>
	/// Выполнить действие
	/// [ Выделить фигуру \ сделать ход ]
	/// </summary>
	void spaceAction();

	/// <summary>
	/// Обратный вызов, когда был выполнен ход
	/// </summary>
	/// <param name="m">Игровой ход</param>
	static void onExecutedMove(chess::FullMove m);

	/// <summary>
	/// Обратный вызов, когда был найден ход ботом
	/// [ подготовка под реализацию игры с ПК ]
	/// </summary>
	/// <param name="m"></param>
	static void onFoundMove(chess::FullMove m);
};
