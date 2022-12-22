#pragma once
#include "core/ButtonSelectorScene.h"
#include "chess/Piece.h"

/// <summary>
/// Сцена вариантов превращения пешки
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
	/// Обратный вызов после превращения
	/// </summary>
	/// <param name="side">Цвет пешки</param>
	static void onPromotion(chess::Side side);

	/// <summary>
	/// Обратный вызов при изменении размера окна
	/// </summary>
	/// <param name="size">Размер окна</param>
	void onSizeChanged(core::Point size) override;

	/// <summary>
	/// Обратный вызов при отрисовке сцены
	/// </summary>
	/// <param name="p">Область отрисовки</param>
	void onDraw(core::Paint& p) override;

	/// <summary>
	/// Обратный вызов при отрисовке заднего фона
	/// </summary>
	/// <param name="p">Область отрисовки</param>
	void onDrawBackground(core::Paint& p) override;

	/// <summary>
	/// Обратный вызов при получении фокуса кнопкой сцены
	/// </summary>
	/// <param name="i">Номер кнопки сцены</param>
	void onButtonSelected(int i) override;

	/// <summary>
	/// Отрисовка подсветки при смене фокуса мышью
	/// </summary>
	/// <param name="p">Область, на которой отрисовать</param>
	/// <param name="r">Кнопка для отрисовки</param>
	void drawMouse(core::Paint& p, const core::Rect& r) override;

	/// <summary>
	/// Отрисовка подсветки при смене фокуса клавиатурой
	/// </summary>
	/// <param name="p">Область, на которой отрисовать</param>
	/// <param name="r">Кнопка для отрисовки</param>
	void drawKeyboard(core::Paint& p, const core::Rect& r) override;

private:
	PromotionScene();

	chess::Side side = chess::Side::White;
	core::RectGroup rects;
};
