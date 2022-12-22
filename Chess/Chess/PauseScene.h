#pragma once

#include "MenuScene.h"

/// <summary>
/// ����� ������������ ������
/// </summary>
class PauseScene : public MenuScene
{
public:
	static PauseScene& instance()
	{
		static PauseScene s;
		return s;
	}

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
	PauseScene();

	core::RectGroup rects;
	bool useSnapshot;
	std::vector<uint8_t> snapshot;

	/// <summary>
	/// �������� ����� ��� �������� �� ����� ��������
	/// </summary>
	static void onReturnFromOptions();
};
