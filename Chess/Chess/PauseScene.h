#pragma once

#include "MenuScene.h"

/// <summary>
/// Сцена приостановки партии
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
	PauseScene();

	core::RectGroup rects;
	bool useSnapshot;
	std::vector<uint8_t> snapshot;

	/// <summary>
	/// Обратный вызов при возврате из сцены настроек
	/// </summary>
	static void onReturnFromOptions();
};
