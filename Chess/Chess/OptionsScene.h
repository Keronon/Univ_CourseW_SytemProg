#pragma once

#include "MenuScene.h"

/// <summary>
/// ����� �������� ����
/// </summary>
class OptionsScene : public MenuScene
{
public:
	using Callback = void(*)();
	static OptionsScene& instance()
	{
		static OptionsScene s;
		return s;
	}

	/// <summary>
	/// ��������� �����
	/// </summary>
	/// <param name="onBack"></param>
	static void load(Callback onBack);

	/// <summary>
	/// �������� ����� ��� ��������� ������� ����
	/// </summary>
	/// <param name="size">������ ����</param>
	void onSizeChanged(core::Point size) override;

	/// <summary>
	/// �������� ����� ��� ��������� ������� ����
	/// </summary>
	/// <param name="p">������� ���������</param>
	void onDrawBackground(core::Paint& p) override;

	/// <summary>
	/// �������� ����� ��� ��������� ������ ������� �����
	/// </summary>
	/// <param name="i">����� ������ �����</param>
	void onButtonSelected(int i) override;

private:
	OptionsScene();
	Callback onBack;

	core::RectGroup rects;
};
