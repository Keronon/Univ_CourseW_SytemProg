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
	/// Обратный вызов при мате
	/// </summary>
	/// <param name="move">Последний шаг</param>
	/// <param name="msg">Сообщение для вывода</param>
	static void onCheckmate(chess::FullMove move, const std::string& msg);

	/// <summary>
	/// Обратный вызов при тупике
	/// </summary>
	/// <param name="move">Последний шаг</param>
	/// <param name="msg">Сообщение для вывода</param>
	static void onStalemate(chess::FullMove move, const std::string& msg);

	/// <summary>
	/// Обратный вызов при ничьей
	/// </summary>
	/// <param name="move">Последний шаг</param>
	/// <param name="msg">Сообщение для вывода</param>
	static void onGameDraw(chess::FullMove move, const std::string& msg);

	/// <summary>
	/// Обратный вызов при изменении размера окна
	/// </summary>
	/// <param name="size">Размер окна</param>
	void onSizeChanged(core::Point size) override;

	/// <summary>
	/// Обратный вызов при запуске сцены
	/// </summary>
	void onStart() override;

	/// <summary>
	/// Обратный вызов при отрисовке сцены
	/// </summary>
	/// <param name="p">Область отрисовки</param>
	void onDraw(core::Paint& p) override;

	/// <summary>
	/// Обратный вызов при отрисовке заднего фона
	/// </summary>
	/// <param name="p">Область отрисовки</param>
	void onDrawBackground(core::Paint& p) override;

	/// <summary>
	/// Обратный вызов при нажатии кнопки
	/// </summary>
	/// <param name="k">Кнопка</param>
	void onKeyDown(char k) override;

	/// <summary>
	/// Обратный вызов при вжатии ЛКМ
	/// </summary>
	/// <param name="pos">Позиция вжатия</param>
	void onLeftMouseDown(core::Point pos) override;

	/// <summary>
	/// Обратный вызов при смене фокуса кнопок на сцене
	/// </summary>
	/// <param name="i">Номер кнопки</param>
	void onButtonSelected(int i) override;

	/// <summary>
	/// Обратный вызов при движении мыши
	/// </summary>
	/// <param name="pos">Позиция курсора</param>
	void onMouseMove(core::Point pos) override;

	// // // // // относятся к сцене отрисовки игрового поля ( BoardDrawingScene )

	/// <summary>
	/// Выдаёт игровое поле
	/// </summary>
	/// <returns>Игровое поле</returns>
	const chess::Board& getBoard() const override;

	/// <summary>
	/// Выдаёт имя игрока по цвету
	/// </summary>
	/// <param name="'side'">Цвет игрока</param>
	/// <returns>Имя игрока</returns>
	const std::string& getPlayerName(chess::Side) const override;

	/// <summary>
	/// Выдаёт определение белого игрока
	/// </summary>
	/// <returns>Цвет игрока</returns>
	chess::Side getPlayerSide() const override;

private:
	/// <summary>
	/// Состояния сцены { Движется фигура, Анимируется, Анимация завершена, Просмотр игрового поля }
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
	/// Вернуться от просмотра игрового поля
	/// </summary>
	/// <returns>Состояние сцены</returns>
	State returnFromBoardViewing();

	/// <summary>
	/// Реализация загрузки сцены
	/// </summary>
	/// <param name="move">...</param>
	/// <param name="title">...</param>
	/// <param name="subtitle">...</param>
	void loadImpl(chess::FullMove move,
		          std::string_view title,
		          const std::string& subtitle);

	/// <summary>
	/// Вызывается из onLeftMouseDown() и onKeyDown().
	/// Проверяет действия пользователя.
	/// Если состояние сцены - просмотр игрового поля,
	/// то просмотр завершается.
	/// </summary>
	/// <returns>true - если надо передать ввод в MenuScene</returns>
	bool checkUserInput();

	/// <summary>
	/// Завершение движения фигур
	/// </summary>
	void finishPieceMoving();

	/// <summary>
	/// Завершение анимаций
	/// </summary>
	void finishAnimation();

	/// <summary>
	/// Отобразить игровое поле
	/// </summary>
	void viewBoard();
};
