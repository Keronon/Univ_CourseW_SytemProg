#pragma once

#include "WindowHandler.h"

namespace core
{
	/// <summary>
	/// �����. ���������� ��� WindowHandler, ��� ������������.
	/// ������������ ������� �������.
	/// { �������� - �����: �������� ����, ����, ����� }
	/// [ ����� ��������� ���� �����, �� ������������� ���������� ����������� ��������� ]
	/// [ ����� �������������� ��� ������ ����������� ����������� ]
	/// [ ������������ ������ ���� ������ ����� ����� ]
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
		/// �������� �����
		/// [ ������������� ��� �������� ]
		/// </summary>
		static void quit()
		{
			WindowHandler::instance().quit();
		}

		/// <summary>
		/// ������������ �����
		/// </summary>
		static void redraw()
		{
			WindowHandler::instance().redraw();
		}

		/// <summary>
		/// ������������ ������ ���
		/// </summary>
		static void redrawBackground()
		{
			WindowHandler::instance().redrawBackground();
		}

		/// <summary>
		/// ���������������� ������� ��������� ������ ��� ������� �����
		/// [ ���������� ����� onSelected() ]
		/// </summary>
		virtual void onStart() {}

		// Called when this scene gets deselected or the window is closed

		/// <summary>
		/// ���������������� ������� ��������� ������ ��� �������� �����
		/// </summary>
		virtual void onStop() {}

		/// <summary>
		/// ���������������� ������� ��������� ������ ��� ��������� �����
		/// </summary>
		/// <param name="p">������� ���������</param>
		virtual void onDraw(Paint& p) = 0;

		/// <summary>
		/// ���������������� ������� ��������� ������ ��� ��������� ������� ����
		/// </summary>
		/// <param name="p">������� ���������</param>
		virtual void onDrawBackground(Paint& p) = 0;

		/// <summary>
		/// ���������������� ������� ��������� ������ ��� ������ �������
		/// </summary>
		/// <param name="'key'">�������</param>
		virtual void onKeyDown(char /*key*/) {}

		/// <summary>
		/// ���������������� ������� ��������� ������ ��� ������� �������
		/// </summary>
		/// <param name="'key'"></param>
		virtual void onKeyUp(char /*key*/) {}

		/// <summary>
		/// ���������������� ������� ��������� ������ ��� �������� ����
		/// </summary>
		/// <param name="'pos'">������� �������</param>
		virtual void onMouseMove(Point /*pos*/) {}

		/// <summary>
		/// ���������������� ������� ��������� ������ ��� ���-�������
		/// </summary>
		/// <param name="'pos'"></param>
		virtual void onRightMouseUp(Point /*pos*/) {}

		/// <summary>
		/// ���������������� ������� ��������� ������ ��� ���-������
		/// </summary>
		/// <param name="'pos'"></param>
		virtual void onRightMouseDown(Point /*pos*/) {}

		/// <summary>
		/// ���������������� ������� ��������� ������ ��� ���-�������
		/// </summary>
		/// <param name="'pos'"></param>
		virtual void onLeftMouseUp(Point /*pos*/) {}

		/// <summary>
		/// ���������������� ������� ��������� ������ ��� ���-������
		/// </summary>
		/// <param name="'pos'"></param>
		virtual void onLeftMouseDown(Point /*pos*/) {}

		/// <summary>
		/// ���������������� ������� ��������� ������ ��� ����� ������� ����
		/// [ ���������� ����� onSelected() ]
		/// </summary>
		/// <param name="'newSize'"></param>
		virtual void onSizeChanged(Point /*newSize*/) {}

		virtual ~Scene() noexcept = default;
	};
} // namespace core
