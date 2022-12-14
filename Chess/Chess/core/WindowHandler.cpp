#include "WindowHandler.h"

#include "Scene.h"
#include "../Sprites.h"

#include <windowsx.h>

namespace core
{
	WindowHandler* WindowHandler::theInstance = nullptr;

	int WindowHandler::run(Point size, const char* title, Scene& initialScene, int nCmdShow)
	{
		HWND hwnd = createWindow(title, Rect({ 300, 200 }, size), PFD_TYPE_RGBA, PFD_DOUBLEBUFFER);
		WindowDC dc(hwnd);
		WindowHandler wh(title, initialScene, size, hwnd, dc);
		::ShowWindow(hwnd, nCmdShow);

		MSG msg;
		while (::GetMessage(&msg, hwnd, 0, 0))
		{
			if (wh.hasQuit) return 0;
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		return (int)msg.wParam;
	}

	WindowHandler::WindowHandler(const char* title, Scene& scene, Point size, HWND hwnd, const WindowDC& dc)
		: currentScene(&scene), title(title), hwnd(hwnd), background(dc, size), foreground(dc, size),
		  windowMode(WindowMode::Resizeable), stretchData(size, true)
	{
		if (theInstance != nullptr)
		{
			throw std::logic_error("Multiple instances of WindowHandler aren't allowed");
		}
		theInstance = this;
		scene.onStart();
		updateSize();
	}

	void modifyStyle(HWND hwnd, DWORD dwStyle, Rect rect, UINT uFlags = SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED)
	{
		::ShowWindow(hwnd, SW_HIDE);

		if (!::SetWindowLongPtrA(hwnd, GWL_STYLE, dwStyle))
			throw WinapiError("SetWindowLongPtr");

		if (!::SetWindowPos(hwnd, HWND_TOP, rect.x0(), rect.y0(), rect.width(), rect.height(), uFlags))
			throw WinapiError("SetWindowPos");

		::ShowWindow(hwnd, SW_SHOW);
	}

	constexpr DWORD ResizeableStyle = WS_OVERLAPPEDWINDOW;
	constexpr DWORD StaticStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

	void WindowHandler::setWindowMode(WindowMode val)
	{
		if (val == windowMode) return;
		Rect rect = background.size().asRect();

		switch (val)
		{
			case WindowMode::Resizeable:
				rect.adjustWindowRect(ResizeableStyle, false);
				modifyStyle(hwnd, ResizeableStyle, rect);
				break;
			case WindowMode::Static:
				modifyStyle(hwnd, StaticStyle, rect);
				break;
		}
		windowMode = val;
		stretchData.setShouldStretch(windowMode != WindowMode::Static);
		stretchData.updateSize(getSize());
	}

	WindowHandler::~WindowHandler() noexcept
	{
		::DestroyWindow(hwnd);
		theInstance = nullptr;
	}

	void WindowHandler::getSnapshot(std::vector<uint8_t>& snap) const
	{
		foreground.getSnapshot(snap);
	}

	void WindowHandler::setSnapshot(const std::vector<uint8_t>& snap)
	{
		background.setSnapshot(snap);
	}

	void WindowHandler::updateSize()
	{
		currentScene->onSizeChanged(background.size());
		redrawBackground();
	}

	const std::string& WindowHandler::getTitle() const
	{
		return title;
	}

	void WindowHandler::setTitle(const std::string& val)
	{
		::SetWindowTextA(hwnd, val.c_str());
		title = val;
	}

	Point WindowHandler::getSize() const
	{
		return Rect::getClientRect(hwnd).size();
	}

	void WindowHandler::setSize(Point val)
	{
		auto r = Rect::getClientRect(hwnd);

		if (!::MoveWindow(hwnd, r.x0(), r.y0(), val.x, val.y, false)) // repaint = false, ?.?. ????? ????? ????????? ????????????
		{
			throw WinapiError("MoveWindow");
		}
		updateSize();
		redraw();
	}

	void WindowHandler::quit()
	{
		hasQuit = true;
		::PostQuitMessage(0);
	}

	void WindowHandler::redraw()
	{
		::InvalidateRect(hwnd, nullptr, false /*erase*/);
	}

	void WindowHandler::redrawBackground()
	{
		currentScene->onDrawBackground(background);
		redraw();
	}

	void WindowHandler::changeScene(Scene& s)
	{
		currentScene->onDraw(background);
		currentScene->onStop();
		currentScene = &s;
		currentScene->onStart();
		updateSize();
		redraw();
	}

	Point WindowHandler::getMousePos(LPARAM lParam)
	{
		Point p{ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		return stretchData.dstToSrcCoord(p);
	}

	LRESULT CALLBACK WindowHandler::eventHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		PAINTSTRUCT ps;
		switch (uMsg)
		{
			case WM_PAINT:
			{
				auto hdc = ::BeginPaint(hWnd, &ps);
				auto& bg = instance().background;
				auto& fg = instance().foreground;
				bg.copyTo(fg);
				scene().onDraw(fg);
				fg.copyToStretched(hdc, instance().stretchData);

				::EndPaint(hWnd, &ps);
				return 0;
			}
			case WM_SIZE:
			{
				if (lParam != 0) // ???? ?? ????????
				{
					instance().stretchData.updateSize({ LOWORD(lParam), HIWORD(lParam) });
					instance().redraw();
				}
				return 0;
			}
			case WM_KEYDOWN:
				scene().onKeyDown((char)wParam);
				return 0;
			case WM_KEYUP:
				scene().onKeyUp((char)wParam);
				return 0;
			case WM_MOUSEMOVE:
				scene().onMouseMove(instance().getMousePos(lParam));
				return 0;
			case WM_LBUTTONDOWN:
				scene().onLeftMouseDown(instance().getMousePos(lParam));
				return 0;
			case WM_LBUTTONUP:
				scene().onLeftMouseUp(instance().getMousePos(lParam));
				return 0;
			case WM_RBUTTONDOWN:
				scene().onRightMouseDown(instance().getMousePos(lParam));
				return 0;
			case WM_RBUTTONUP:
				scene().onRightMouseUp(instance().getMousePos(lParam));
				return 0;
			case WM_CLOSE:
				instance().hasQuit = true;
				::PostQuitMessage(0);
				return 0;
		}
		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	template<size_t Width, size_t Height, const char CP[]>
	HICON createIcon(const ConstPaletteSprite<Width, Height, CP>& sprite, const ConstPalette<CP>& palette)
	{
		ICONINFO info;
		info.fIcon = true; // ????????? ????????? ???????? ???????
		std::array<uint32_t, Width* Height> data;

		constexpr auto toInt = [](Color c)
		{
			return c.a() << 24 | c.r() << 16 | c.r() << 8 | c.b();
		};

		int z = 0;
		for (size_t j = 0; j < Height; ++j)
		{
			for (size_t i = 0; i < Width; ++i)
			{
				data[z++] = toInt(palette[sprite.at(i, j)]);
			}
		}

		struct RaiiBitmap
		{
			HBITMAP val;
			RaiiBitmap(int width, int height,
				       UINT planes, UINT bitCount,
				       const void* data)
			{
				val = ::CreateBitmap(width, height, planes, bitCount, data);
				if (!val)
					throw WinapiError("CreateBitmap");
			}

			operator HBITMAP() const { return val; }

			~RaiiBitmap()
			{
				::DeleteObject(val);
			}
		};

		RaiiBitmap col = RaiiBitmap(Width, Height, 1, 32, data.data());
		RaiiBitmap mask = RaiiBitmap(Width, Height, 1, 1, nullptr);

		info.hbmColor = col;
		info.hbmMask = mask;
		HICON res = ::CreateIconIndirect(&info);
		if (res == nullptr) throw WinapiError("CreateIconIndirect");

		return res;
	}

	HWND WindowHandler::createWindow(const char* title, Rect r,
		BYTE type, DWORD flags)
	{
		HINSTANCE hInstance = ::GetModuleHandle(nullptr);
		if (hInstance == nullptr) throw WinapiError("GetModuleHandle()");
		WNDCLASSW wc;

		ConstPalette<sprites::CharPalette> iconPalette = {
			core::Color::Clear,
			core::Color::Black,
			core::Color::Black,
			core::Color::Black,
		};

		wc.style = CS_OWNDC;
		wc.lpfnWndProc = eventHandler;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = createIcon(sprites::Pawn, iconPalette);
		wc.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
		wc.hbrBackground = nullptr;
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = L"Pixels";

		if (!::RegisterClassW(&wc))
		{
			throw WinapiError("RegisterClassW");
		}

		DWORD dwStyle = ResizeableStyle;
		Rect adjustedR = r;
		adjustedR.adjustWindowRect(dwStyle, false);
		r.x1() = r.x0() + adjustedR.width();
		r.y1() = r.y0() + adjustedR.height();

		HWND hwnd = ::CreateWindowA("Pixels", title, dwStyle,
			r.x0(), r.y0(), r.width(), r.height(),
			nullptr, nullptr, hInstance, nullptr);

		if (hwnd == nullptr)
		{
			throw WinapiError("CreateWindow");
		}
		WindowDC dc(hwnd);
		PIXELFORMATDESCRIPTOR pfd = {};

		pfd.nSize = sizeof(pfd);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | flags;
		pfd.iPixelType = type;
		pfd.cColorBits = 32;
		int pf = ::ChoosePixelFormat(dc, &pfd);
		if (pf == 0)
		{
			throw WinapiError("ChoosePixelFormat");
		}

		if (!::SetPixelFormat(dc, pf, &pfd))
		{
			throw WinapiError("SetPixelFormat()");
		}
		return hwnd;
	}
}
