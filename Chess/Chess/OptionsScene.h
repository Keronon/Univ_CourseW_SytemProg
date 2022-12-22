#pragma once

#include "MenuScene.h"

/// <summary>
/// Сцена настроек игры
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
	/// Загружает сцену
	/// </summary>
	/// <param name="onBack"></param>
	static void load(Callback onBack);

	/// <summary>
	/// Обратный вызов при изменении размера окна
	/// </summary>
	/// <param name="size">Размер окна</param>
	void onSizeChanged(core::Point size) override;

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

private:
	OptionsScene();
	Callback onBack;

	core::RectGroup rects;
};
