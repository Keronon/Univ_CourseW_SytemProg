#pragma once

#include "WindowHandler.h"

namespace core
{
	/// <summary>
	/// Сцена. Определяет для WindowHandler, что отрисовывать.
	/// Контролирует события нажатий.
	/// { например - сцены: Главного меню, Игры, Паузы }
	/// [ Сцены накрывают друг друга, не перерисовывая предыдущее отображение полностью ]
	/// [ Может использоваться для вывода Всплывающих уведомлений ]
	/// [ Используется только один объект одной сцены ]
	/// </summary>
	class Scene
	{
	public:
		Scene() = default;
		Scene(const Scene&) = delete;
		Scene(Scene&&) = delete;
		Scene& operator=(const Scene&) = delete;
		Scene& operator=(Scene&&) = delete;

		/// <summary>
		/// Закрытие сцены
		/// [ Предусмотрено для удобства ]
		/// </summary>
		static void quit()
		{
			WindowHandler::instance().quit();
		}

		/// <summary>
		/// Перерисовать сцену
		/// </summary>
		static void redraw()
		{
			WindowHandler::instance().redraw();
		}

		/// <summary>
		/// Перерисовать задний фон
		/// </summary>
		static void redrawBackground()
		{
			WindowHandler::instance().redrawBackground();
		}

		/// <summary>
		/// Переопределяемая функция обратного вызова при запуске сцены
		/// [ вызывается после onSelected() ]
		/// </summary>
		virtual void onStart() {}

		// Called when this scene gets deselected or the window is closed

		/// <summary>
		/// Переопределяемая функция обратного вызова при закрытии сцены
		/// </summary>
		virtual void onStop() {}

		/// <summary>
		/// Переопределяемая функция обратного вызова при отрисовке сцены
		/// </summary>
		/// <param name="p">Область отрисовки</param>
		virtual void onDraw(Paint& p) = 0;

		/// <summary>
		/// Переопределяемая функция обратного вызова при отрисовке заднего фона
		/// </summary>
		/// <param name="p">Область отрисовки</param>
		virtual void onDrawBackground(Paint& p) = 0;

		/// <summary>
		/// Переопределяемая функция обратного вызова при вжатии клавиши
		/// </summary>
		/// <param name="'key'">Клавиша</param>
		virtual void onKeyDown(char /*key*/) {}

		/// <summary>
		/// Переопределяемая функция обратного вызова при отжатии клавиши
		/// </summary>
		/// <param name="'key'"></param>
		virtual void onKeyUp(char /*key*/) {}

		/// <summary>
		/// Переопределяемая функция обратного вызова при движении мыши
		/// </summary>
		/// <param name="'pos'">Позиция курсора</param>
		virtual void onMouseMove(Point /*pos*/) {}

		/// <summary>
		/// Переопределяемая функция обратного вызова при ПКМ-отжатии
		/// </summary>
		/// <param name="'pos'"></param>
		virtual void onRightMouseUp(Point /*pos*/) {}

		/// <summary>
		/// Переопределяемая функция обратного вызова при ПКМ-вжатии
		/// </summary>
		/// <param name="'pos'"></param>
		virtual void onRightMouseDown(Point /*pos*/) {}

		/// <summary>
		/// Переопределяемая функция обратного вызова при ЛКМ-отжатии
		/// </summary>
		/// <param name="'pos'"></param>
		virtual void onLeftMouseUp(Point /*pos*/) {}

		/// <summary>
		/// Переопределяемая функция обратного вызова при ЛКМ-вжатии
		/// </summary>
		/// <param name="'pos'"></param>
		virtual void onLeftMouseDown(Point /*pos*/) {}

		/// <summary>
		/// Переопределяемая функция обратного вызова при смене размера окна
		/// [ вызывается после onSelected() ]
		/// </summary>
		/// <param name="'newSize'"></param>
		virtual void onSizeChanged(Point /*newSize*/) {}

		virtual ~Scene() noexcept = default;
	};
} // namespace core
