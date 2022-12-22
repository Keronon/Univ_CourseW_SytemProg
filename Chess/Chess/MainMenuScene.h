#pragma once

#include "MenuScene.h"
#include "chess/Common.h"

/// <summary>
/// Сцена главного меню
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
	/// Обратный вызов при изменении размера окна
	/// </summary>
	/// <param name="size">Размер окна</param>
	void onSizeChanged(core::Point size) override;

	/// <summary>
	/// Обратный вызов при дополнительных отрисовках
	/// [ например: дополнительный объект на кнопках ]
	/// </summary>
	/// <param name="p">Область отрисовки</param>
	void onDrawExtra(core::Paint& p) override;

	/// <summary>
	/// Обратный вызов при отрисовке заднего фона
	/// </summary>
	/// <param name="p">Область отрисовки</param>
	void onDrawBackground(core::Paint& p) override;

	/// <summary>
	/// Обратный вызов при выборе кнопки на сцене
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
