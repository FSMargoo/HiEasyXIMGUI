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
 * \file hex_impl_easyx.h
 * \brief The HiEasyX impl of the EasyX
 */

#pragma once

#ifdef HEX_IMPLEMENTATION
#	error Should only include one HiEasyX UI implementation at once!
#endif

#define HEX_IMPLEMENTATION

#include <include/impl/hex_impl.h>

#include <graphics.h>
#include <vector>

#undef DrawText

namespace HX {
void HXBegin();
void HXInitForEasyX();
void *GetHXBuffer(IMAGE *Buffer);
}

class HXBufferPainterImpl : public HXBufferPainter {
public:
	/**
	 * Constructing the buffer painter
	 * @param Buffer The buffer to be drawn
	 */
	explicit HXBufferPainterImpl(IMAGE *Buffer);

	~HXBufferPainterImpl() override = default;

public:
	void DrawLine(HXPoint Point1, HXPoint Point2, HXColor Color) override;

	void DrawRectangle(HXRect Rect, HXColor Color) override;

	void DrawFilledRectangle(HXRect Rect, HXColor Color, HXColor FillColor) override;

	void DrawPainter(HXBufferPainter *Painter, HXPoint Where) override;

	void DrawText(const HXString &Text, HXFont Font, HXPoint Where, HXColor Color, HXGUInt Height) override;

	void DrawFilledPolygon(std::vector<HXPoint> Points, HXColor Color) override;

	void DrawFilledRoundedRectangle(HXRect Rect, HXColor Color, HXColor FillColor, HXGInt Radius) override;

	void Clear(HXColor Color) override;

	HXRect MeasureText(const HXString &Text, HXFont Font, HXGUInt Height) override;

public:
	HXBufferPainter *CreateSubPainter(HXGInt Width, HXGInt Height) override;

	HXBufferPainter *CreateFromBuffer(void *Buffer) override;

public:
	void Begin() override;

	void End() override;

private:
	static void SetupEasyXFont(HXFont Font, HXGUInt Height);

protected:
	IMAGE *_buffer;
};

class HXExHostedBufferPainterImpl : public HXBufferPainterImpl {
public:
	HXExHostedBufferPainterImpl(HXGInt Width, HXGInt Height);

	~HXExHostedBufferPainterImpl() override;
};

class HXContextImpl : public HXContext {
public:
	HXContextImpl();

	~HXContextImpl() override = default;

public:
	HXBufferPainter *BufferToPainter(void *Buffer) override;

	HXBufferPainter *DefaultPainter() override;

	HXBuffer *GetDeviceBuffer() override;

private:
	HXBufferPainterImpl *_defaultPainter;
};

namespace HX {
/**
 * Converting the EasyX message to the HX message format
 * @param Message The message to be converted
 * @return The converted HX message type
 */
void *GetHXMessage(ExMessage *Message);
}

class HXMessageSenderImpl : public HXMessageSender {
public:
	HXMessageSenderImpl() = default;

	~HXMessageSenderImpl() override = default;

public:
	HXMessage Message(void *Message) override;
};

class HXOSOperationImpl : public HXOSOperation {
public:
	~HXOSOperationImpl() override = default;

public:
	void SetCursorStyle(HXCursorStyle Style) override;
};