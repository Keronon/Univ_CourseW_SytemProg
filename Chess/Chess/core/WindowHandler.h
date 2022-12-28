#pragma once

#include "Color.h"
#include "Paint.h"

#include <iostream>

namespace core
{
	class Scene;

	/// <summary>
	/// Тип окна { Статичное, Изменяемый размер }
	/// </summary>
	enum class WindowMode
	{
		Static,
		Resizeable
	};

	/// <summary>
	/// Управление WinAPI и всем связанным с отрисовкой и отловом событий
	/// </summary>
	class WindowHandler
	{
	public:
		static WindowHandler& instance() { return *theInstance; }

		static int run(Point size, const char* title,
			Scene& initialScene,
			int nCmdShow = SW_SHOWDEFAULT);

		WindowHandler(WindowHandler&&)      = delete;
		WindowHandler(const WindowHandler&) = delete;

		WindowHandler& operator=(WindowHandler&&)      = delete;
		WindowHandler& operator=(const WindowHandler&) = delete;
		~WindowHandler() noexcept;

		/// <summary>
		/// Выдаёт заголовок окна
		/// </summary>
		/// <returns>Заголовок окна</returns>
		const std::string& getTitle() const;

		/// <summary>
		/// Устанавливает заголовок окна
		/// </summary>
		/// <param name="val">Заголовок окна</param>
		void setTitle(const std::string& val);

		/// <summary>
		/// Выдаёт размер области окна
		/// </summary>
		/// <returns>Размер окна</returns>
		Point getSize() const;

		/// <summary>
		/// Устанавливает размер окна
		/// </summary>
		/// <param name="val">Размер окна</param>
		void setSize(Point val);

		/// <summary>
		/// Перерисовка окна
		/// </summary>
		void redraw();

		/// <summary>
		/// Перерисовна заднего фона окна
		/// </summary>
		void redrawBackground();

		/// <summary>
		/// Выход из приложения
		/// </summary>
		void quit();

		/// <summary>
		/// Переключить сцену
		/// </summary>
		/// <param name="s">Сцена</param>
		void changeScene(Scene& s);

		/// <summary>
		/// Выдаёт режим отображения окна
		/// </summary>
		/// <returns>Режим окна</returns>
		WindowMode getWindowMode() const { return windowMode; }

		/// <summary>
		/// Устанавливает режим отображения окна
		/// </summary>
		/// <param name="val">Режим окна</param>
		void setWindowMode(WindowMode val);

		/// <summary>
		/// Фиксирует состояние отрисовки окна
		/// </summary>
		/// <param name="snap">Хранилище для вырезки окна</param>
		void getSnapshot(std::vector<uint8_t>& snap) const;

		/// <summary>
		/// Устанавливает состояние отрисовки окна
		/// </summary>
		/// <param name="snap">Вырезка окна</param>
		void setSnapshot(const std::vector<uint8_t>& snap);

	private:
		/// <summary>
		/// Обновить размер окна
		/// </summary>
		void updateSize();

		static LRESULT CALLBACK eventHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		static HWND createWindow(const char* title, Rect rect, BYTE type, DWORD flags);

		static WindowHandler* theInstance;

		Scene* currentScene;
		std::string title;

		// Glut could save some headaches but I wanted this to work out of the box in Windows
		HWND  hwnd;
		Paint background, foreground;

		WindowMode windowMode;
		Paint::StretchData stretchData;

		bool hasQuit = false;

		WindowHandler(const char* title, Scene& scene, Point size, HWND hwnd, const WindowDC& hdc);

		static Scene& scene() { return *instance().currentScene; }

		Point getMousePos(LPARAM lParam);
	};
}
