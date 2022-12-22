#pragma once

#include "Scene.h"
#include "RectGroup.h"

namespace core
{
	/// <summary>
	/// ����� ��� ���� � ��������� ��������
	/// </summary>
	class ButtonSelectorScene : public virtual Scene
	{
	public:
		/// <summary>
		/// ��� ����� { ������������, �������������� }
		/// </summary>
		enum class Mode { Vertical, Horizontal };

		ButtonSelectorScene(size_t btnCount, Mode mode) : rects(btnCount), mode(mode)
		{}

		/// <summary>
		/// �������� ����� ��� ������� �����
		/// [ ������ ���������� �������� ������� ]
		/// </summary>
		void onStart() override;

		// Must be called by subclass

		/// <summary>
		/// �������� ����� ��� ������� ������
		/// [ ������ ���������� �������� ������� ]
		/// </summary>
		/// <param name="k">�������</param>
		void onKeyDown(char k) override;

		/// <summary>
		/// �������� ����� ��� ������ ���
		/// [ ������ ���������� �������� ������� ]
		/// </summary>
		/// <param name="pos">������� ������</param>
		void onLeftMouseDown(Point pos) override;

		/// <summary>
		/// �������� ����� ��� �������� ����
		/// [ ������ ���������� �������� ������� ]
		/// </summary>
		/// <param name="pos">������� �������</param>
		void onMouseMove(Point pos) override;

		/// <summary>
		/// �������� ����� ��� ����������� �����
		/// [ ������ ���������� �������� ������� ]
		/// </summary>
		/// <param name="p"></param>
		void onDraw(Paint& p) override;

		/// <summary>
		/// ���������������� ������� ��������� ������ ��� ��������� ������ ������� �����
		/// </summary>
		/// <param name="i">����� ������ �����</param>
		virtual void onButtonSelected(int i) = 0;

		/// <summary>
		/// ���������������� �������, ������� �������� �� ������������ ������ ������ ����� ��� ������ ����������
		/// [ ������� ����� � ����� ������� i=-1 ]
		/// [ ������� ������ � ���� ������� i=1 ]
		/// [ �� ������ ���������� �������� ������� ]
		/// </summary>
		/// <param name="i">����������� �������� ������ [ 1 \ -1 ]</param>
		/// <param name="delta">�������� �������� ��� ������� ��������</param>
		virtual void onButtonFocusMove(int i, int delta);

		/// <summary>
		/// ���������������� ������� ��������� ������ ��� ������� �� ������ �����
		/// [ ����� ���������� �������� ������� ]
		/// </summary>
		/// <param name="i">����� ������ �����</param>
		/// <param name="mousePos">������� �������</param>
		virtual void onButtonClick(int i, Point mousePos);

		/// <summary>
		/// ���������������� ������� ��������� ��������� ��� ����� ������ �����
		/// </summary>
		/// <param name="p">�������, �� ������� ����������</param>
		/// <param name="r">������ ��� ���������</param>
		virtual void drawMouse(Paint& p, const Rect& r) = 0;

		/// <summary>
		/// ���������������� ������� ��������� ��������� ��� ����� ������ �����������
		/// </summary>
		/// <param name="p">�������, �� ������� ����������</param>
		/// <param name="r">������ ��� ���������</param>
		virtual void drawKeyboard(Paint& p, const Rect& r) = 0;

	protected:
		/// <summary>
		/// ����� ������ ��������� ������ �����
		/// [ ���������� �� �������������: getButtonRects().update'Smthn'() - � onSizeChanged() ]
		/// </summary>
		/// <returns>������ ��������� ������ �����</returns>
		RectGroup& getButtonRects() { return rects; }

		/// <summary>
		/// ����� ���������� ������ ����� �� ������
		/// </summary>
		/// <param name="i">����� ������</param>
		/// <returns>���������� ������</returns>
		const Rect& getButtonRect(int i) const { return rects[i]; }
		      Rect& getButtonRect(int i)       { return rects[i]; }

		/// <summary>
		/// ����� ��� �����
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
		/// ������������� ����������� ��������� �� ���� ������� ����
		/// </summary>
		/// <param name="val">����� �������������� ������</param>
		void setMouseCursor(int val);

		/// <summary>
		/// ���������� ����������� ��������� �� ���� ����������
		/// </summary>
		/// <param name="delta">�������� ��������</param>
		/// <param name="btnMode">��� �����</param>
		void moveKeyboardCursor(int delta, Mode btnMode);

		/// <summary>
		/// ��������� ���������� �������� �� �����
		/// </summary>
		void done()
		{
			onButtonSelected(usingKeyboard ? keyboardCursor : mouseCursor);
		}
	};
}
