#pragma once

#include "SceneCommon.h"
#include "chess/Board.h"

#include <chrono>

/// <summary>
/// Сцена с игровым полем
/// </summary>
class BoardDrawingScene : public virtual core::Scene
{
public:
	static core::Rect getBoardRect(core::Point size);

	/// <summary>
	/// Обратный вызов при изменении размера окна
	/// [ должен вызываться дочерним классом ]
	/// </summary>
	/// <param name="size">Размер окна</param>
	void onSizeChanged(core::Point size) override;

	/// <summary>
	/// Обратный вызов при отрисовке заднего фона
	/// [ должен вызываться дочерним классом ]
	/// </summary>
	/// <param name="paint">Область отрисовки</param>
	void onDrawBackground(core::Paint& paint) override;

	/// <summary>
	/// Обратный вызов при отрисовке сцены
	/// [ должен вызываться дочерним классом ]
	/// </summary>
	/// <param name="paint"></param>
	void onDraw(core::Paint& paint) override;

protected:
	/// <summary>
	/// Переопределяемая функция для выдачи игрового поля
	/// </summary>
	/// <returns>Игровое поле</returns>
	virtual const chess::Board& getBoard() const = 0;

	/// <summary>
	/// Переопределяемая функция для выдачи имени игрока
	/// </summary>
	/// <param name="'side'">Цвет игрока</param>
	/// <returns>Имя игрока</returns>
	virtual const std::string& getPlayerName(chess::Side) const = 0;

	/// <summary>
	/// Переопределяемая функция для выдачи определения белого игрока
	/// </summary>
	/// <returns>Цвет игрока</returns>
	virtual chess::Side getPlayerSide() const = 0;

	BoardDrawingScene();

	/// <summary>
	/// Преобразователь координат на игровом поле в оконные
	/// </summary>
	/// <param name="boardPos">Координаты на поле (в ячейках)</param>
	/// <returns>Координаты на экране (в пикселях)</returns>
	core::Point boardPosToScreen(chess::Pos boardPos) const;

	/// <summary>
	/// Преобразователь оконных координат на игровое поле
	/// </summary>
	/// <param name="pt">Координаты на экране (в пикселях)</param>
	/// <returns>Координаты на поле (в ячейках)</returns>
	chess::Pos screenToBoardPos(core::Point pt) const;

	/// <summary>
	/// Переопределяемая функция для отрисовки ячеек поля
	/// </summary>
	/// <param name="paint">Область отрисовки</param>
	virtual void drawBoardSquares(core::Paint& paint) const;

	/// <summary>
	/// Переопределяемая функция для отрисовки обозначений на поле { A..H, 1..8 }
	/// </summary>
	/// <param name="paint">Область отрисовки</param>
	virtual void drawBoardMarkings(core::Paint& paint) const;

	/// <summary>
	/// Переопределяемая функция для отрисовки съеденных фигур
	/// </summary>
	/// <param name="paint">Область отрисовки</param>
	virtual void drawEatenPieces(core::Paint& paint) const;

	//must be called

	/// <summary>
	/// Переопределяемая функция для отрисовки игрового поля
	/// [ должна вызываться дочерним классом ]
	/// </summary>
	/// <param name="paint">Область отрисовки</param>
	virtual void drawBoard(core::Paint& paint) const;

	/// <summary>
	/// Переопределяемая функция для отрисовки панели с информацией по игре справа от поля
	/// </summary>
	/// <param name="paint">Область отрисовки</param>
	virtual void drawRightPaneInfo(core::Paint& paint) const;

	/// <summary>
	/// Отрисовывает изображение
	/// </summary>
	/// <param name="paint">Область отрисовки</param>
	/// <param name="pt">Позиция отрисовки</param>
	/// <param name="s">Изображение</param>
	/// <param name="palette">Палитра</param>
	void spriteAt(core::Paint& paint, core::Point pt,
		          const core::PaletteSprite& s,
		          const core::Palette& palette) const;

	/// <summary>
	/// Отрисовывает изображение на игровом поле
	/// </summary>
	/// <param name="paint">Область отрисовки</param>
	/// <param name="p">Позиция отрисовки (в ячейках)</param>
	/// <param name="s">Изображение</param>
	/// <param name="palette">Палитра</param>
	void spriteOnBoard(core::Paint& paint, chess::Pos p,
		               const core::PaletteSprite& s,
		               const core::Palette& palette) const;

	/// <summary>
	/// Выдаёт прямоугольник поля без маркировок
	/// </summary>
	/// <returns></returns>
	const core::Rect& getBoardRect() const { return boardRect; }

private:
	core::Rect boardRect; // без маркировок ( внутри области с { 1..8, A..H } отметками)
	core::Rect paneRect;

protected:
	/// <summary>
	/// Данные движения фигур
	/// [ для плавного движения ]
	/// </summary>
	struct PieceMovingData
	{
		/// <summary>
		/// Тип движения { Не движется, Движется сама, Тянется мышью }
		/// </summary>
		enum class State
		{
			NotMoving,
			Interpolating,
			MouseMoving,
		};

		PieceMovingData(const BoardDrawingScene& parent) : parent(parent) {}

		/// <summary>
		/// Сброс передвижения
		/// </summary>
		void reset()
		{
			state = State::NotMoving;
			piecePos = chess::Pos::Invalid;
		}

		/// <summary>
		/// Определяет, движется ли фигура
		/// </summary>
		/// <returns>true - если движется</returns>
		constexpr bool isMoving() const { return state != State::NotMoving; }

		/// <summary>
		/// Выдаёт позицию фигуры (в ячейках)
		/// </summary>
		/// <returns></returns>
		constexpr chess::Pos getPiecePos() const { return piecePos; }

		/// <summary>
		/// Выдаёт экранную точку движения
		/// </summary>
		/// <returns></returns>
		core::Point getPoint();

		/// <summary>
		/// Устанавливает необходимое движение для фигуры
		/// </summary>
		/// <param name="move">Игровой ход</param>
		void setInterpolatedMove(chess::FullMove move);

		/// <summary>
		/// Определяет фокусировку на фигурах по курсору
		/// </summary>
		/// <param name="p">Позиция курсора</param>
		/// <param name="boardPos">Ячейка игрового поля</param>
		void setMousePos(core::Point p, chess::Pos boardPos);

		/// <summary>
		/// Обратный вызов при движении мыши
		/// </summary>
		/// <param name="p">Позиция курсора</param>
		/// <returns>true - если надо перерисовать фигуру</returns>
		bool onMouseMove(core::Point p);

		/// <summary>
		/// Обратный вызов при ЛКМ-отжатии
		/// </summary>
		void onLeftMouseUp();

		/// <summary>
		/// Остановка движения фигуры
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
	/// Выдаёт начало отрисовки у игрового поля
	/// </summary>
	/// <returns>Точка левого верхнего угла</returns>
	core::Point boardStart() const { return boardRect.topLeft(); }
};
