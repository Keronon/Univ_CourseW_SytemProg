#pragma once

#include "core/ButtonSelectorScene.h"
#include "Sprites.h"

#include <variant>

/// <summary>
/// ����� ��� ���� ����
/// </summary>
class MenuScene : public core::ButtonSelectorScene
{
public:
	/// <summary>
	/// ���������� � ������� ��� �����
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
		/// ������� ������ ������������ ����
		/// </summary>
		/// <param name="text">������� �� ������</param>
		/// <returns>���������� � ������</returns>
		static ButtonData makeNormal(std::string_view text)
		{
			return { text, Normal{} };
		}

		/// <summary>
		/// ������� ������ ���� ������������� ����-�����
		/// </summary>
		/// <param name="text">������� �� �������������</param>
		/// <param name="min">����������� ��������</param>
		/// <param name="max">������������ ��������</param>
		/// <param name="getVal">������� ��������� ��������</param>
		/// <param name="setVal">������� ��������� ��������</param>
		/// <returns>���������� � �������������</returns>
		static ButtonData makeLeftRight(std::string_view text, int min, int max,
			                            LeftRight::GetVal getVal,
			                            LeftRight::SetVal setVal)
		{
			return { text, LeftRight{min, max, getVal, setVal} };
		}

		/// <summary>
		/// ������� ������ ���� �����-������
		/// </summary>
		/// <param name="text">������� �� ������</param>
		/// <param name="getVal">������� ��������� ��������</param>
		/// <returns>���������� � ������</returns>
		static ButtonData makeRadio(std::string_view text, Radio::GetVal getVal)
		{
			return { text, Radio{getVal} };
		}

		/// <summary>
		/// ������� �����-������ � ������������
		/// </summary>
		/// <param name="text">������� ��� ������</param>
		/// <returns>���������� � ������</returns>
		static ButtonData makeSpriteRadio(std::string_view text)
		{
			return { text, SpriteRadio{} };
		}

		/// <summary>
		/// ����� ������������� ������
		/// </summary>
		/// <returns>������������� ������</returns>
		core::Rect& rect() { return *rect_; }

		/// <summary>
		/// ����� ������������� ������
		/// </summary>
		/// <returns>������������� ������</returns>
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
	/// �������� ������� ������ � ������������ ����
	/// [ ������ ���� ������� � onSizeChanged() ]
	/// </summary>
	/// <param name="r">����� �������������� ��� �����������</param>
	void updateVertSizes(core::Rect r);

	// If you want to override this use onDrawExtra
	// only override in special cases

	/// <summary>
	/// �������� ����� ��� ��������� �����
	/// </summary>
	/// <param name="p">������� ���������</param>
	void onDraw(core::Paint& p) override;

	/// <summary>
	/// ���������������� �������� ����� ��� �������������� ����������
	/// [ ��������: �������������� ������ �� ������� ]
	/// </summary>
	/// <param name="p">������� ���������</param>
	virtual void onDrawExtra(core::Paint& /*p*/) {}

	/// <summary>
	/// �������� ����� ��� ��������� ������� ����
	/// [ ������ ���� ������� � �������� ������� ]
	/// </summary>
	/// <param name="p">������� ���������</param>
	void onDrawBackground(core::Paint& p) override;

	// Must be called by subclass

	/// <summary>
	/// �������� ����� ��� ��������� ������ ������� �����
	/// </summary>
	/// <param name="i">����� ������ �����</param>
	void onButtonSelected(int i) override;

	/// <summary>
	/// �������� ����� ��� ������������ ������ ������ ����� ��� ������ ����������
	/// [ ������� ����� � ����� ������� i=-1 ]
	/// [ ������� ������ � ���� ������� i=1 ]
	/// </summary>
	/// <param name="i">����������� �������� ������ [ 1 \ -1 ]</param>
	/// <param name="delta">�������� �������� ��� ������� ��������</param>
	void onButtonFocusMove(int i, int delta) final;

	/// <summary>
	/// �������� ����� ��� ������� �� ������ �����
	/// </summary>
	/// <param name="i">����� ������ �����</param>
	/// <param name="mousePos">������� �������</param>
	void onButtonClick(int i, core::Point mousePos) final;

	/// <summary>
	/// ������������ ��������� ��� ����� ������ �����
	/// </summary>
	/// <param name="p">�������, �� ������� ����������</param>
	/// <param name="r">������ ��� ���������</param>
	void drawMouse(core::Paint& p, const core::Rect& r) override;

	/// <summary>
	/// ������������ ��������� ��� ����� ������ �����������
	/// </summary>
	/// <param name="p">�������, �� ������� ����������</param>
	/// <param name="r">������ ��� ���������</param>
	void drawKeyboard(core::Paint& p, const core::Rect& r) override;

protected:
	/// <summary>
	/// ����� ������ ����� �� ������
	/// </summary>
	/// <param name="i">����� ������ �����</param>
	/// <returns>���������� � ������</returns>
	ButtonData& getButton(int i) { return buttons[i]; }

	/// <summary>
	/// ����� ������ ����� �� ������
	/// </summary>
	/// <param name="i">����� ������ �����</param>
	/// <returns>���������� � ������</returns>
	const ButtonData& getButton(int i) const { return buttons[i]; }

private:
	std::vector<ButtonData> buttons;
};
