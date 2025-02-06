/*
 * Copyright (c) 2025 HiEasyX
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * \file hex_impl_easyx.cpp
 * \brief The HiEasyX impl of the EasyX
 */

#include <algorithm>
#include <include/impl/EasyX/hex_impl_easyx.h>

#include <graphics.h>
#include <iterator>

namespace HX {
void Begin(HXContext *RenderContext);

void OSAPI(HXOSOperation *API);

void HXBegin() {
	static HXContextImpl     context;
	static HXOSOperationImpl api;
	Begin(&context);
	OSAPI(&api);
}

void MessageSender(HXMessageSender *Sender);

void CreateTheme();

void HXInitForEasyX() {
	HXMessageSenderImpl *sender = new HXMessageSenderImpl;
	MessageSender(sender);

	CreateTheme();
}

void *GetHXBuffer(IMAGE *Buffer) {
	return static_cast<void *>(Buffer);
}
}

#define HXColorToEasyXColor(COLOR) (RGB(COLOR.R, COLOR.G, COLOR.B))

/////////////////////////////////////////////
/// HXBufferPainterImpl

HXBufferPainterImpl::HXBufferPainterImpl(IMAGE *Buffer) {
	_buffer = Buffer;

	setbkmode(TRANSPARENT);
}

void HXBufferPainterImpl::DrawLine(HXPoint Point1, HXPoint Point2, HXColor Color) {
	setlinecolor(HXColorToEasyXColor(Color));

	line(Point1.X, Point1.Y, Point2.X, Point2.Y);
}

void HXBufferPainterImpl::DrawRectangle(HXRect Rect, HXColor Color) {
	setlinecolor(HXColorToEasyXColor(Color));

	rectangle(Rect.Left, Rect.Top, Rect.Right, Rect.Bottom);
}

void HXBufferPainterImpl::DrawFilledRectangle(HXRect Rect, HXColor Color, HXColor FillColor) {
	setlinecolor(HXColorToEasyXColor(Color));
	setfillcolor(HXColorToEasyXColor(FillColor));

	solidrectangle(Rect.Left, Rect.Top, Rect.Right, Rect.Bottom);
	rectangle(Rect.Left, Rect.Top, Rect.Right, Rect.Bottom);
}

void HXBufferPainterImpl::DrawPainter(HXBufferPainter *Painter, HXPoint Where) {
	putimage(Where.X, Where.Y, static_cast<HXBufferPainterImpl *>(Painter)->_buffer);
}

void HXBufferPainterImpl::SetupEasyXFont(HXFont Font, HXGUInt Height) {
	LOGFONT font;

	gettextstyle(&font);
	_tcscpy_s(font.lfFaceName, Font.Family.c_str());
	font.lfItalic  = Font.Italic;
	font.lfQuality = PROOF_QUALITY;

	switch (Font.Style) {
	case HXFontStyle::Regular:
		font.lfWeight = FW_NORMAL;
		break;
	case HXFontStyle::Bold:
		font.lfWeight = FW_BOLD;
		break;
	case HXFontStyle::Black:
		font.lfWeight = FW_BLACK;
		break;
	case HXFontStyle::Light:
		font.lfWeight = FW_LIGHT;
		break;
	case HXFontStyle::Medium:
		font.lfWeight = FW_MEDIUM;

	// Just for fallback
	default:
		font.lfWeight = FW_NORMAL;
		break;
	}
	font.lfHeight = static_cast<LONG>(Height);

	settextstyle(&font);
}

void HXBufferPainterImpl::DrawText(const HXString &Text, HXFont Font, HXPoint Where, HXColor Color, HXGUInt Height) {
	SetupEasyXFont(Font, Height);

	settextcolor(HXColorToEasyXColor(Color));

	outtextxy(Where.X, Where.Y, Text.c_str());
}

void HXBufferPainterImpl::DrawFilledPolygon(std::vector<HXPoint> Points, HXColor Color) {
	setfillcolor(HXColorToEasyXColor(Color));

	std::vector<POINT> points;
	points.reserve(Points.size());
	std::ranges::transform(Points, std::back_inserter(points), [](const HXPoint &Point) {
		return POINT{static_cast<LONG>(Point.X), static_cast<LONG>(Point.Y)};
	});

	solidpolygon(points.data(), points.size());
}

void HXBufferPainterImpl::DrawFilledRoundedRectangle(HXRect Rect, HXColor Color, HXColor FillColor, HXGInt Radius) {
	setlinecolor(HXColorToEasyXColor(Color));
	setfillcolor(HXColorToEasyXColor(FillColor));

	fillroundrect(Rect.Left, Rect.Top, Rect.Right, Rect.Bottom, Radius, Radius);
}

HXRect HXBufferPainterImpl::MeasureText(const HXString &Text, HXFont Font, HXGUInt Height) {
	SetupEasyXFont(Font, Height);

	return {.Left = 0, .Top = 0, .Right = textwidth(Text.c_str()), .Bottom = textheight(Text.c_str())};
}

void HXBufferPainterImpl::Clear(HXColor Color) {
	setbkcolor(HXColorToEasyXColor(Color));

	cleardevice();
}

HXBufferPainter *HXBufferPainterImpl::CreateSubPainter(HXGInt Width, HXGInt Height) {
	return new HXExHostedBufferPainterImpl(Width, Height);
}

HXBufferPainter *HXBufferPainterImpl::CreateFromBuffer(void *Buffer) {
	return new HXBufferPainterImpl(static_cast<IMAGE *>(Buffer));
}

void HXBufferPainterImpl::Begin() {
	SetWorkingImage(_buffer);

	setbkmode(TRANSPARENT);
}

void HXBufferPainterImpl::End() {
	SetWorkingImage();
}

/////////////////////////////////////////////
/// HXExHostedBufferPainterImpl

HXExHostedBufferPainterImpl::HXExHostedBufferPainterImpl(HXGInt Width, HXGInt Height)
	: HXBufferPainterImpl(new IMAGE(Width, Height)) {
}

HXExHostedBufferPainterImpl::~HXExHostedBufferPainterImpl() {
	delete _buffer;
}

/////////////////////////////////////////////
/// HXContextImpl

HXContextImpl::HXContextImpl() {
	_defaultPainter = new HXBufferPainterImpl(nullptr);
}

HXBufferPainter *HXContextImpl::DefaultPainter() {
	return _defaultPainter;
}

HXBufferPainter *HXContextImpl::BufferToPainter(void *Buffer) {
	return new HXBufferPainterImpl(static_cast<IMAGE *>(Buffer));
}

HXBuffer *HXContextImpl::GetDeviceBuffer() {
	return reinterpret_cast<HXBuffer *>(GetImageBuffer());
}

/////////////////////////////////////////////
/// HXMessageSenderImpl

HXMessage HXMessageSenderImpl::Message(void *Message) {
	HXMessage message{};
	auto      exMessage = static_cast<ExMessage *>(Message);
	if (exMessage->message == WM_LBUTTONDOWN || exMessage->message == WM_LBUTTONUP) {
		message.MouseAction = true;
	}
	if (exMessage->message == WM_MOUSEMOVE) {
		message.MouseAction = true;
	}
	if (exMessage->message == WM_LBUTTONDOWN) {
		message.MouseLeftPressed = true;
	}
	if (exMessage->message == WM_LBUTTONUP) {
		message.MouseLeftRelease = true;
	}
	if (message.MouseAction) {
		message.MouseX = exMessage->x;
		message.MouseY = exMessage->y;
	}

	return message;
}

////////////////////////////////////////////
/// Global

namespace HX {
void *GetHXMessage(ExMessage *Message) {
	return static_cast<void *>(Message);
}
}

void HXOSOperationImpl::SetCursorStyle(HXCursorStyle Style) {
	switch (Style) {
	case HXCursorStyle::Normal: {
		LPCTSTR cursor       = IDC_ARROW;
		auto    cursorHandle = LoadCursor(NULL, cursor);
		SetClassLongPtr(GetHWnd(), GCLP_HCURSOR, reinterpret_cast<LPARAM>(cursorHandle));

		break;
	}
	case HXCursorStyle::Editing: {
		LPCTSTR cursor       = IDC_IBEAM;
		auto    cursorHandle = LoadCursor(NULL, cursor);
		SetClassLongPtr(GetHWnd(), GCLP_HCURSOR, reinterpret_cast<LPARAM>(cursorHandle));

		break;
	}
	case HXCursorStyle::ResizeNW: {
		LPCTSTR cursor       = IDC_SIZENWSE;
		auto    cursorHandle = LoadCursor(NULL, cursor);
		SetClassLongPtr(GetHWnd(), GCLP_HCURSOR, reinterpret_cast<LPARAM>(cursorHandle));

		break;
	}
	case HXCursorStyle::ResizeN: {
		LPCTSTR cursor       = IDC_SIZENS;
		auto    cursorHandle = LoadCursor(NULL, cursor);
		SetClassLongPtr(GetHWnd(), GCLP_HCURSOR, reinterpret_cast<LPARAM>(cursorHandle));

		break;
	}
	case HXCursorStyle::ResizeE: {
		LPCTSTR cursor       = IDC_SIZEWE;
		auto    cursorHandle = LoadCursor(NULL, cursor);
		SetClassLongPtr(GetHWnd(), GCLP_HCURSOR, reinterpret_cast<LPARAM>(cursorHandle));

		break;
	}
	}
}
