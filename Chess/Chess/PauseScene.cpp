#include "PauseScene.h"
#include "GameScene.h"
#include "EndGameScene.h"
#include "MainMenuScene.h"
#include "OptionsScene.h"

using namespace core;

enum Button : int
{
	Continue = 0,
	StepBack,
	FinishMatch,
	Options,
	MainMenu,
	Quit,

	BtnCount,
};

void PauseScene::onReturnFromOptions()
{
	WindowHandler::instance().setSnapshot(instance().snapshot);
	SceneManager::load(instance());
}

PauseScene::PauseScene()
	: MenuScene({
		ButtonData::makeNormal("Continue"),
		ButtonData::makeNormal("Step Back"),
		ButtonData::makeNormal("Finish Match"),
		ButtonData::makeNormal("Options"),
		ButtonData::makeNormal("Main Menu"),
		ButtonData::makeNormal("Quit"),
		}, Mode::Vertical), rects(2), useSnapshot(false)
{}

void PauseScene::onSizeChanged(core::Point size)
{
	constexpr Point titleSize{ 580, 130 };

	constexpr int spacing = 10;
	rects.updateCenteredVert(GameScene::getBoardRect(size), titleSize,
		getVertButtonsSize(BtnCount), spacing,
		{ 10, 20 });
	updateVertSizes(rects[1]);
}

void PauseScene::onDrawBackground(core::Paint& p)
{
	constexpr const char* titleFont = "Arial";

	p.fill(Color::Blurry);
	p.fillRect(rects.boundingRect, MenuBgCol);
	p.drawRectOut(rects.boundingRect, MarginSize, MenuMarginCol);
	p.setTextColor(BigTextCol);
	p.setFont(titleFont, SmallTitleSize, true);
	p.drawText(rects[0], "Pause");

	MenuScene::onDrawBackground(p);
}

void PauseScene::onButtonSelected(int index)
{
	MenuScene::onButtonSelected(index);
	switch (index)
	{
		case Button::Continue:
			SceneManager::load<GameScene>();
			break;
		case Button::StepBack:
			SceneManager::load<GameScene>();
			GameScene::onStepBack();
			break;
		case Button::FinishMatch:
			SceneManager::load<GameScene>();
			EndGameScene::onGameDraw({}, "Both players agreed to the draw.");
			break;
		case Button::Options:
			WindowHandler::instance().getSnapshot(snapshot);
			OptionsScene::load(onReturnFromOptions);
			break;
		case Button::MainMenu:
			SceneManager::load<MainMenuScene>();
			break;
		case Button::Quit:
			quit();
			break;
	}
}