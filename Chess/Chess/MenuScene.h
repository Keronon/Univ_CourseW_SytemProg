#pragma once

#include "core/ButtonSelectorScene.h"
#include "Sprites.h"

#include <variant>

/// <summary>
/// Макет для сцен меню
/// </summary>
class MenuScene : public core::ButtonSelectorScene
{
public:
	/// <summary>
	/// Информация о кнопках для сцены
	/// </summary>
	class ButtonData
	{
		struct Normal {};
		struct Radio
		{
			using GetVal = bool (*)();
			GetVal getVal;
		};
		struct SpriteRadio
		{
			// all the drawaing should be done in the subclass
		};
		struct LeftRight
		{
			using GetVal = int (*)();
			using SetVal = void (*)(int);
			int min, max;
			GetVal getVal;
			SetVal setVal;
		};
		using Data = std::variant<Normal, Radio, SpriteRadio, LeftRight>;

	public:
		/// <summary>
		/// Создать кнопку стандартного типа
		/// </summary>
		/// <param name="text">Надпись на кнопке</param>
		/// <returns>Информация о кнопке</returns>
		static ButtonData makeNormal(std::string_view text)
		{
			return { text, Normal{} };
		}

		/// <summary>
		/// Создать кнопку типа Переключатель лево-право
		/// </summary>
		/// <param name="text">Надпись на переключателе</param>
		/// <param name="min">Минимальное значение</param>
		/// <param name="max">Максимальное значение</param>
		/// <param name="getVal">Функция получения значения</param>
		/// <param name="setVal">Функция установки значения</param>
		/// <returns>Информация о переключателе</returns>
		static ButtonData makeLeftRight(std::string_view text, int min, int max,
			                            LeftRight::GetVal getVal,
			                            LeftRight::SetVal setVal)
		{
			return { text, LeftRight{min, max, getVal, setVal} };
		}

		/// <summary>
		/// Создать кнопку типа Радио-кнопка
		/// </summary>
		/// <param name="text">Надпись на кнопке</param>
		/// <param name="getVal">Функция получения значения</param>
		/// <returns>Информация о кнопке</returns>
		static ButtonData makeRadio(std::string_view text, Radio::GetVal getVal)
		{
			return { text, Radio{getVal} };
		}

		/// <summary>
		/// Создать Радио-кнопку с изображением
		/// </summary>
		/// <param name="text">Надпись для кнопки</param>
		/// <returns>Информация о кнопке</returns>
		static ButtonData makeSpriteRadio(std::string_view text)
		{
			return { text, SpriteRadio{} };
		}

		/// <summary>
		/// Выдаёт прямоугольник кнопки
		/// </summary>
		/// <returns>Прямоугольник кнопки</returns>
		core::Rect& rect() { return *rect_; }

		/// <summary>
		/// Выдаёт прямоугольник кнопки
		/// </summary>
		/// <returns>Прямоугольник кнопки</returns>
		const core::Rect& rect() const { return *rect_; }

	private:
		std::string text;
		core::Rect* rect_ = nullptr;
		Data data;

		ButtonData() {}
		ButtonData(std::string_view text, Data data)
			: text(text), data(data)
		{}

		friend class MenuScene;
	};

	MenuScene(std::initializer_list<ButtonData> buttons, Mode mode);

	void drawLeft(core::Paint& p, size_t index,
		          const core::PaletteSprite& sprite,
		          const core::Palette& palette) const;

	void drawLeft(core::Paint& p, const core::Rect& r,
		          const core::PaletteSprite& sprite,
		          const core::Palette& palette) const;

	void drawRight(core::Paint& p, const core::Rect& r,
		           const core::PaletteSprite& sprite,
		           const core::Palette& palette) const;

	void drawRight(core::Paint& p, size_t index,
		           const core::PaletteSprite& sprite,
		           const core::Palette& palette) const;

	/// <summary>
	/// Обновить размеры кнопок в вертикальном меню
	/// [ должна быть вызвана в onSizeChanged() ]
	/// </summary>
	/// <param name="r">Новые прямоугольники для перерисовки</param>
	void updateVertSizes(core::Rect r);

	// If you want to override this use onDrawExtra
	// only override in special cases

	/// <summary>
	/// Обратный вызов при отрисовке сцены
	/// </summary>
	/// <param name="p">Область отрисовки</param>
	void onDraw(core::Paint& p) override;

	/// <summary>
	/// Переопределяемый обратный вызов при дополнительных отрисовках
	/// [ например: дополнительный объект на кнопках ]
	/// </summary>
	/// <param name="p">Область отрисовки</param>
	virtual void onDrawExtra(core::Paint& /*p*/) {}

	/// <summary>
	/// Обратный вызов при отрисовке заднего фона
	/// [ должна быть вызвана в дочерних классах ]
	/// </summary>
	/// <param name="p">Область отрисовки</param>
	void onDrawBackground(core::Paint& p) override;

	// Must be called by subclass

	/// <summary>
	/// Обратный вызов при получении фокуса кнопкой сцены
	/// </summary>
	/// <param name="i">Номер кнопки сцены</param>
	void onButtonSelected(int i) override;

	/// <summary>
	/// Обратный вызов при переключении фокуса кнопок сцены при помощи клавиатуры
	/// [ нажатия Влево и Вверх требуют i=-1 ]
	/// [ нажатия Вправо и Вниз требуют i=1 ]
	/// </summary>
	/// <param name="i">Модификатор смещения фокуса [ 1 \ -1 ]</param>
	/// <param name="delta">Поправка смещения для крайних смещений</param>
	void onButtonFocusMove(int i, int delta) final;

	/// <summary>
	/// Обратный вызов при нажатии на кнопку сцены
	/// </summary>
	/// <param name="i">Номер кнопки сцены</param>
	/// <param name="mousePos">Позиция курсора</param>
	void onButtonClick(int i, core::Point mousePos) final;

	/// <summary>
	/// Отрисовывает подсветки при смене фокуса мышью
	/// </summary>
	/// <param name="p">Область, на которой отрисовать</param>
	/// <param name="r">Кнопка для отрисовки</param>
	void drawMouse(core::Paint& p, const core::Rect& r) override;

	/// <summary>
	/// Отрисовывает подсветки при смене фокуса клавиатурой
	/// </summary>
	/// <param name="p">Область, на которой отрисовать</param>
	/// <param name="r">Кнопка для отрисовки</param>
	void drawKeyboard(core::Paint& p, const core::Rect& r) override;

protected:
	/// <summary>
	/// Выдаёт кнопку сцены по номеру
	/// </summary>
	/// <param name="i">Номер кнопки сцены</param>
	/// <returns>Информация о кнопке</returns>
	ButtonData& getButton(int i) { return buttons[i]; }

	/// <summary>
	/// Выдаёт кнопку сцены по номеру
	/// </summary>
	/// <param name="i">Номер кнопки сцены</param>
	/// <returns>Информация о кнопке</returns>
	const ButtonData& getButton(int i) const { return buttons[i]; }

private:
	std::vector<ButtonData> buttons;
};
