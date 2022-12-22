#pragma once

#include "MenuScene.h"
#include "chess/Common.h"

/// <summary>
/// ����� �������� ����
/// </summary>
class MainMenuScene : public MenuScene
{
public:
	static MainMenuScene& instance()
	{
		static MainMenuScene s;
		return s;
	}

	/// <summary>
	/// �������� ����� ��� ��������� ������� ����
	/// </summary>
	/// <param name="size">������ ����</param>
	void onSizeChanged(core::Point size) override;

	/// <summary>
	/// �������� ����� ��� �������������� ����������
	/// [ ��������: �������������� ������ �� ������� ]
	/// </summary>
	/// <param name="p">������� ���������</param>
	void onDrawExtra(core::Paint& p) override;

	/// <summary>
	/// �������� ����� ��� ��������� ������� ����
	/// </summary>
	/// <param name="p">������� ���������</param>
	void onDrawBackground(core::Paint& p) override;

	/// <summary>
	/// �������� ����� ��� ������ ������ �� �����
	/// </summary>
	/// <param name="i"></param>
	void onButtonSelected(int i) override;

private:
	MainMenuScene();

	core::RectGroup rects;

	chess::Side side;
	bool stretch;
	bool isFullScreen;
};
