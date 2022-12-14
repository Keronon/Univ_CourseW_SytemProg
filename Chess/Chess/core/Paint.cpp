#include "Paint.h"

#include <cstring>

namespace core
{
	template<class T>
	T selectObject(HDC hdc, T t)
	{
		T res = (T) ::SelectObject(hdc, t);
		if (t == nullptr || t == HGDI_ERROR)
			throw WinapiError("SelectObject");
		return res;
	}

	template<class T>
	void deleteObject(T t)
	{
		if (!::DeleteObject(t))
			throw WinapiError("DeleteObject");
	}

	Paint::Paint(HDC ogHDC, Point size)
		     : hdc(CreateCompatibleDC(ogHDC)), bounds({ 0,0 }, size)
	{
		int width = size.x;
		int height = size.y;
		auto& h = bmi.bmiHeader;
		h.biSize = sizeof(BITMAPINFO);
		h.biWidth = width;
		h.biHeight = -height;
		h.biPlanes = 1;
		h.biBitCount = 24;
		h.biCompression = BI_RGB;
		h.biXPelsPerMeter = 0;
		h.biYPelsPerMeter = 0;
		h.biSizeImage = width * height * 3;
		h.biClrUsed = 0;
		h.biClrImportant = 0;

		bitmap = ::CreateDIBSection(
			ogHDC, &bmi, DIB_RGB_COLORS,
			(void**)&data, nullptr, 0);

		if (bitmap == nullptr)
			throw WinapiError("CreateDIBSection");
		h.biHeight = height;

		oldBitmap = selectObject(hdc, bitmap);
		oldFont = setFontImpl("Arial", 32, false);

		::SetBkMode(hdc, TRANSPARENT);
	}

	Paint::~Paint() noexcept
	{
		selectObject(hdc, oldBitmap);
		deleteObject(bitmap);

		auto font = selectObject(hdc, oldFont);
		deleteObject(font);
		::DeleteDC(hdc);
	}

	void Paint::getSnapshot(std::vector<uint8_t>& snap) const
	{
		snap.assign(data, data + byteSize());
	}

	void Paint::setSnapshot(const std::vector<uint8_t>& snap)
	{
		std::memcpy(data, snap.data(), snap.size());
	}

	void stretchBlt(HDC dst, Paint::StretchData::SzRect dstRect,
		HDC src, Point srcPos, Point srcSize)
	{
		if (!::StretchBlt(dst, dstRect.x, dstRect.y, dstRect.w, dstRect.h,
			src, srcPos.x, srcPos.y, srcSize.x, srcSize.y,
			SRCCOPY))
		{
			throw WinapiError("StretchedBlt");
		}
	}

	Paint::StretchData::StretchData(Point size, bool shouldStretch)
	{
		srcSize = size;
		srcAspectRatio = size.aspectRatio();
		updateSize(size);
		setShouldStretch(shouldStretch);
	}

	void Paint::StretchData::setShouldStretch(bool val)
	{
		shouldStretch = val;
		if (!val)
		{
			dstToSrcRatio = 1;
		}
	}

	Point Paint::StretchData::dstToSrcCoord(Point dstPt)
	{
		dstPt -= drawRect.p0();
		return { int(dstPt.x * dstToSrcRatio), int(dstPt.y * dstToSrcRatio) };
	}

	void Paint::StretchData::updateSize(Point dstSize)
	{
		if (!shouldStretch) return;
		if (srcAspectRatio < dstSize.aspectRatio())
		{
			int width = int(srcAspectRatio * dstSize.y);
			int offsetX = (dstSize.x - width) / 2;

			margins[0] = { 0, 0, offsetX, dstSize.y };
			drawRect = { offsetX, 0, width, dstSize.y };
			margins[1] = { drawRect.x1(), 0, dstSize.x - drawRect.x1(), dstSize.y };

			dstToSrcRatio = double(srcSize.x) / width;
		}
		else
		{
			int height = int(dstSize.x / srcAspectRatio);
			int offsetY = (dstSize.y - height) / 2;

			margins[0] = { 0, 0, dstSize.x, offsetY };
			drawRect = { 0, offsetY, dstSize.x, height };
			margins[1] = { 0, drawRect.y1(), dstSize.x, dstSize.y - drawRect.y1() };

			dstToSrcRatio = double(srcSize.y) / height;
		}
	}

	void Paint::copyToStretched(HDC dst, const StretchData& sd) const
	{
		if (!sd.shouldStretch)
		{
			copyTo(dst);
			return;
		}
		for (const auto& m : sd.margins)
			stretchBlt(dst, m, hdc, {}, { 1, 1 });

		stretchBlt(dst, sd.drawRect, hdc, {}, size());
	}

	void Paint::copyTo(HDC dst) const
	{
		if (!::BitBlt(dst, 0, 0, width(), height(), hdc, 0, 0, SRCCOPY))
			throw WinapiError("BitBlt");
	}

	void Paint::copyTo(Paint& dst) const
	{
		copyTo(dst.hdc);
	}

	void Paint::drawText(Rect r, std::string_view s, UINT format)
	{
		RECT winRect = r;
		if (!::DrawTextA(hdc, s.data(), (int)s.length(), &winRect, format))
			throw WinapiError("DrawTextA");
	}

	void Paint::textOut(Point p, std::string_view s)
	{
		textOut(p.x, p.y, s);
	}

	void Paint::textOut(int x, int y, std::string_view s)
	{
		if (!::TextOutA(hdc, x, y, s.data(), (int)s.length()))
			throw WinapiError("TextOutA");
	}

	void Paint::setTextColor(Color c)
	{
		if (::SetTextColor(hdc, (COLORREF)c) == CLR_INVALID)
			throw WinapiError("SetTextColor");
	}

	HFONT Paint::setFontImpl(const char* name, int size, bool bold)
	{
		HFONT font = ::CreateFontA(
			size, 0, 0, 0,
			bold ? FW_BOLD : FW_NORMAL,
			false, // ?????????
			false, // ???????????
			false, // ??????????
			ANSI_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			NONANTIALIASED_QUALITY,
			DEFAULT_PITCH,
			name);
		if (font == nullptr)
			throw WinapiError("CreateFontA");
		return selectObject(hdc, font);
	}

	void Paint::setFont(const char* name, int size, bool bold)
	{
		auto f = setFontImpl(name, size, bold);
		deleteObject(f);
	}

	void Paint::setPixel(int x, int y, Color col)
	{
		setPixel({ x, y }, col);
	}

	void Paint::setPixel(Point p, Color col)
	{
		if (!bounds.contains(p)) return;
		setPixelUnchecked(p.x, p.y, col);
	}

	void Paint::fill(Color col)
	{
		for (int i = 0; i < byteSize(); i += 3)
		{
			setPixelUnchecked(data + i, col);
		}
	}

	void Paint::fillCircle(Point c, int r, Color col)
	{
		int xMin = std::max(0, c.x - r + 1);
		int yMin = std::max(0, c.y - r + 1);

		int xMax = std::min(width() - 1, c.x + r - 1);
		int yMax = std::min(height() - 1, c.y + r - 1);

		int r2 = r * r;
		for (int y = yMin; y <= yMax; ++y)
		{
			for (int x = xMin; x <= xMax; ++x)
			{
				Point p = Point(x, y) - c;
				if (p.x * p.x + p.y * p.y <= r2)
					setPixelUnchecked(x, y, col);
			}
		}
	}

	void Paint::fillPixelatedCircle(Point c, int radius, Color col,
		int pixelSize)
	{
		auto r = radius / pixelSize;
		auto r2 = r * r;
		for (int j = -r + 1; j < r; ++j)
		{
			for (int i = -r + 1; i < r; ++i)
			{
				if (i * i + j * j <= r2)
				{
					Point start = c + Point(i, j) * pixelSize;
					fillRect({ start, start + Point(pixelSize, pixelSize) }, col);
				}
			}
		}
	}

	void Paint::fillRect(int x0, int y0, int x1, int y1, Color col)
	{
		fillRect({ x0, y0, x1, y1 }, col);
	}

	void Paint::drawRectIn(Rect r, int t, Color col)
	{
		fillRect(r.x0(), r.y0(), r.x1(), r.y0() + t, col);
		fillRect(r.x0(), r.y0() + t, r.x0() + t, r.y1(), col);
		fillRect(r.x1() - t, r.y0() + t, r.x1(), r.y1(), col);
		fillRect(r.x0() + t, r.y1() - t, r.x1() - t, r.y1(), col);
	}

	void Paint::drawRectOut(Rect r, int thickness, Color col)
	{
		r.x0() -= thickness;
		r.y0() -= thickness;
		r.x1() += thickness;
		r.y1() += thickness;
		drawRectIn(r, thickness, col);
	}

	void Paint::fillRect(Rect r, Color col)
	{
		if (r.x0() < 0) r.x0() = 0;
		if (r.y0() < 0) r.y0() = 0;
		if (r.x1() >= width()) r.x1() = width() - 1;
		if (r.y1() >= height()) r.y1() = height() - 1;
		for (auto j = r.y0(); j < r.y1(); ++j)
			for (auto i = r.x0(); i < r.x1(); ++i)
				setPixelUnchecked(i, j, col);
	}

	void Paint::setPixelUnchecked(int x, int y, Color col)
	{
		auto at = [this](int x, int y)
		{
			return data + (y * width() + x) * 3;
		};
		setPixelUnchecked(at(x, y), col);
	}

	void Paint::setPixelUnchecked(uint8_t* ptr, Color col)
	{
		constexpr auto setColor = [](uint8_t* ptr, Color col)
		{
			ptr[2] = col.r();
			ptr[1] = col.g();
			ptr[0] = col.b();
		};
		constexpr auto getColor = [](const uint8_t* ptr)
		{
			return Color(ptr[2], ptr[1], ptr[0]);
		};
		Color c = getColor(ptr);
		setColor(ptr, Color::lerp8(c, col, col.a()));
	}

	void Paint::drawSprite(Point p, const PaletteSprite& s, const Palette& pal)
	{
		for (size_t j = 0; j < s.height(); ++j)
		{
			for (size_t i = 0; i < s.width(); ++i)
			{
				auto index = s.at(i, j);
				if (index) setPixel(p + Point(i, j), pal[index]);
			}
		}
	}

	void Paint::drawSprite(Point p, const PaletteSprite& s, const Palette& pal,
		int scale)
	{
		for (size_t j = 0; j < s.height(); ++j)
		{
			for (size_t i = 0; i < s.width(); ++i)
			{
				auto index = s.at(i, j);
				if (!index)
					continue;
				auto start = Point(i, j) * scale + p;
				fillRect({ start, start + Point(scale, scale) }, pal[index]);
			}
		}
	}

	void Paint::drawSprite(int x, int y, const PaletteSprite& s,
		const Palette& pal)
	{
		return drawSprite({ x, y }, s, pal);
	}

	void Paint::drawSprite(int x, int y,
		const PaletteSprite& s,
		const Palette& palette,
		int scale)
	{
		return drawSprite({ x, y }, s, palette, scale);
	}
}
