#pragma once
#include "core/ButtonSelectorScene.h"
#include "chess/Piece.h"

/// <summary>
/// ����� ��������� ����������� �����
/// </summary>
class PromotionScene : public core::ButtonSelectorScene
{
public:
	static PromotionScene& instance()
	{
		static PromotionScene s;
		return s;
	}

	/// <summary>
	/// �������� ����� ����� �����������
	/// </summary>
	/// <param name="side">���� �����</param>
	static void onPromotion(chess::Side side);

	/// <summary>
	/// �������� ����� ��� ��������� ������� ����
	/// </summary>
	/// <param name="size">������ ����</param>
	void onSizeChanged(core::Point size) override;

	/// <summary>
	/// �������� ����� ��� ��������� �����
	/// </summary>
	/// <param name="p">������� ���������</param>
	void onDraw(core::Paint& p) override;

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

	/// <summary>
	/// ��������� ��������� ��� ����� ������ �����
	/// </summary>
	/// <param name="p">�������, �� ������� ����������</param>
	/// <param name="r">������ ��� ���������</param>
	void drawMouse(core::Paint& p, const core::Rect& r) override;

	/// <summary>
	/// ��������� ��������� ��� ����� ������ �����������
	/// </summary>
	/// <param name="p">�������, �� ������� ����������</param>
	/// <param name="r">������ ��� ���������</param>
	void drawKeyboard(core::Paint& p, const core::Rect& r) override;

private:
	PromotionScene();

	chess::Side side = chess::Side::White;
	core::RectGroup rects;
};
