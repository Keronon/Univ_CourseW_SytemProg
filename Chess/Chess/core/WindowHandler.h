#pragma once

#include "Color.h"
#include "Paint.h"

#include <iostream>

namespace core
{
	class Scene;

	/// <summary>
	/// ��� ���� { ���������, ���������� ������ }
	/// </summary>
	enum class WindowMode
	{
		Static,
		Resizeable
	};

	/// <summary>
	/// ���������� WinAPI � ���� ��������� � ���������� � ������� �������
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
		/// ����� ��������� ����
		/// </summary>
		/// <returns>��������� ����</returns>
		const std::string& getTitle() const;

		/// <summary>
		/// ������������� ��������� ����
		/// </summary>
		/// <param name="val">��������� ����</param>
		void setTitle(const std::string& val);

		/// <summary>
		/// ����� ������ ������� ����
		/// </summary>
		/// <returns>������ ����</returns>
		Point getSize() const;

		/// <summary>
		/// ������������� ������ ����
		/// </summary>
		/// <param name="val">������ ����</param>
		void setSize(Point val);

		/// <summary>
		/// ����������� ����
		/// </summary>
		void redraw();

		/// <summary>
		/// ����������� ������� ���� ����
		/// </summary>
		void redrawBackground();

		/// <summary>
		/// ����� �� ����������
		/// </summary>
		void quit();

		/// <summary>
		/// ����������� �����
		/// </summary>
		/// <param name="s">�����</param>
		void changeScene(Scene& s);

		/// <summary>
		/// ����� ����� ����������� ����
		/// </summary>
		/// <returns>����� ����</returns>
		WindowMode getWindowMode() const { return windowMode; }

		/// <summary>
		/// ������������� ����� ����������� ����
		/// </summary>
		/// <param name="val">����� ����</param>
		void setWindowMode(WindowMode val);

		/// <summary>
		/// ��������� ��������� ��������� ����
		/// </summary>
		/// <param name="snap">��������� ��� ������� ����</param>
		void getSnapshot(std::vector<uint8_t>& snap) const;

		/// <summary>
		/// ������������� ��������� ��������� ����
		/// </summary>
		/// <param name="snap">������� ����</param>
		void setSnapshot(const std::vector<uint8_t>& snap);

	private:
		/// <summary>
		/// �������� ������ ����
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
