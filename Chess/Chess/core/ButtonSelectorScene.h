#pragma once

#include "Scene.h"
#include "RectGroup.h"

namespace core
{
	/// <summary>
	/// Макет для сцен с экранными кнопками
	/// </summary>
	class ButtonSelectorScene : public virtual Scene
	{
	public:
		/// <summary>
		/// Тип сцены { Вертикальная, Горизонтальная }
		/// </summary>
		enum class Mode { Vertical, Horizontal };

		ButtonSelectorScene(size_t btnCount, Mode mode) : rects(btnCount), mode(mode)
		{}

		/// <summary>
		/// Обратный вызов при запуске сцены
		/// [ Должна вызываться дочерним классом ]
		/// </summary>
		void onStart() override;

		// Must be called by subclass

		/// <summary>
		/// Обратный вызов при нажатии клавиш
		/// [ Должна вызываться дочерним классом ]
		/// </summary>
		/// <param name="k">Клавиша</param>
		void onKeyDown(char k) override;

		/// <summary>
		/// Обратный вызов при вжатии ЛКМ
		/// [ Должна вызываться дочерним классом ]
		/// </summary>
		/// <param name="pos">Позиция вжатия</param>
		void onLeftMouseDown(Point pos) override;

		/// <summary>
		/// Обратный вызов при движении мыши
		/// [ Должна вызываться дочерним классом ]
		/// </summary>
		/// <param name="pos">Позиция курсора</param>
		void onMouseMove(Point pos) override;

		/// <summary>
		/// Обратный вызов при перерисовке сцены
		/// [ Должна вызываться дочерним классом ]
		/// </summary>
		/// <param name="p"></param>
		void onDraw(Paint& p) override;

		/// <summary>
		/// Переопределяемая функция обратного вызова при получении фокуса кнопкой сцены
		/// </summary>
		/// <param name="i">Номер кнопки сцены</param>
		virtual void onButtonSelected(int i) = 0;

		/// <summary>
		/// Переопределяемая функция, которая отвечает за переключение фокуса кнопок сцены при помощи клавиатуры
		/// [ нажатия Влево и Вверх требуют i=-1 ]
		/// [ нажатия Вправо и Вниз требуют i=1 ]
		/// [ Не должна вызываться дочерним классом ]
		/// </summary>
		/// <param name="i">Модификатор смещения фокуса [ 1 \ -1 ]</param>
		/// <param name="delta">Поправка смещения для крайних смещений</param>
		virtual void onButtonFocusMove(int i, int delta);

		/// <summary>
		/// Переопределяемая функция обратного вызова при нажатии на кнопку сцены
		/// [ Может вызываться дочерним классом ]
		/// </summary>
		/// <param name="i">Номер кнопки сцены</param>
		/// <param name="mousePos">Позиция курсора</param>
		virtual void onButtonClick(int i, Point mousePos);

		/// <summary>
		/// Переопределяемая функция отрисовки подсветки при смене фокуса мышью
		/// </summary>
		/// <param name="p">Область, на которой отрисовать</param>
		/// <param name="r">Кнопка для отрисовки</param>
		virtual void drawMouse(Paint& p, const Rect& r) = 0;

		/// <summary>
		/// Переопределяемая функция отрисовки подсветки при смене фокуса клавиатурой
		/// </summary>
		/// <param name="p">Область, на которой отрисовать</param>
		/// <param name="r">Кнопка для отрисовки</param>
		virtual void drawKeyboard(Paint& p, const Rect& r) = 0;

	protected:
		/// <summary>
		/// Выдаёт список отрисовок кнопок сцены
		/// [ рассчитана на использование: getButtonRects().update'Smthn'() - в onSizeChanged() ]
		/// </summary>
		/// <returns>Список отрисовок кнопок сцены</returns>
		RectGroup& getButtonRects() { return rects; }

		/// <summary>
		/// Выдаёт отрисовоку кнопки сцены по номеру
		/// </summary>
		/// <param name="i">Номер кнопки</param>
		/// <returns>Отрисовока кнопки</returns>
		const Rect& getButtonRect(int i) const { return rects[i]; }
		      Rect& getButtonRect(int i)       { return rects[i]; }

		/// <summary>
		/// Выдаёт тип сцены
		/// </summary>
		/// <returns></returns>
		Mode getMode() const { return mode; }

	private:
		RectGroup rects;

		Mode  mode;
		bool  usingKeyboard = true;
		int   keyboardCursor;
		int   mouseCursor;
		Point prevMousePos;

		/// <summary>
		/// Устанавливает отображение подсветки за счёт курсора мыши
		/// </summary>
		/// <param name="val">Номер подсвечиваемой кнопки</param>
		void setMouseCursor(int val);

		/// <summary>
		/// Перемещает отображение подсветки за счёт клавиатуры
		/// </summary>
		/// <param name="delta">Поправка смещения</param>
		/// <param name="btnMode">Тип сцены</param>
		void moveKeyboardCursor(int delta, Mode btnMode);

		/// <summary>
		/// Завершает выполнение действий на сцене
		/// </summary>
		void done()
		{
			onButtonSelected(usingKeyboard ? keyboardCursor : mouseCursor);
		}
	};
}
