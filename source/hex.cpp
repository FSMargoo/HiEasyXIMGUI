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
 * \file hex.cpp
 * \brief The HiEasyX major code
 */

#include <include/hex.h>

#include <map>
#include <vector>

namespace HX {
HXTheme          Theme;
HXRuntimeContext Context;
HXMessageSender *MsgSender;

HXRuntimeContext &GetContext() {
	return Context;
}

HXTheme &GetTheme() {
	return Theme;
}

HXPoint _ClipToLocalWindowCoord(HXPoint Coord) {
	return {Coord.X - Context.CurrentWindow->Where.X, Coord.Y - Context.CurrentWindow->Where.Y};
}

void SetBuffer(void *Buffer) {
	Context.LocalBuffer = Buffer;
}

void OSAPI(HXOSOperation *API) {
	Context.OSAPI = API;
}

HXString GetLastError() {
	return Context.LastError;
}gui

void MessageSender(HXMessageSender *Sender) {
	MsgSender = Sender;
	if (Sender == nullptr) {
		Context.Win       = false;
		Context.LastError = "Invalid message sender";
	}
}

void PushMessage(void *Message) {
	Context.MessageQuery.push_back(MsgSender->Message(Message));
}

void Begin(HXContext *RenderContext) {
	for (auto &window : Context.Windows) {
		delete window;
	}

	Context.Windows.clear();

	if (Context.Initialized) {
		Context.Win       = false;
		Context.LastError = "End is needed for another UI layout progress";
	} else {
		Context               = HXRuntimeContext{};
		Context.RenderContext = RenderContext;
		Context.Initialized   = true;
	}
}

void WindowLocate(HXPoint Where) {
	Context.CurrentWindow->Where = Where;
}

void End() {
	Context.Initialized = false;
}

bool Wined() {
	return Context.Win;
}

void CreateTheme() {
	Theme = HXTheme{
		.WindowBackground = HXColor{21, 22, 23, 255},
		.WindowTitleText = HXColor{255, 255, 255, 255},
		.WindowTitleBackground = HXColor{41, 74, 122, 255},
		.ButtonBorder = HXColor{39, 73, 114, 255},
		.ButtonBackground = HXColor{39, 73, 114, 255},
		.ButtonText = HXColor{255, 255, 255, 255},
		.ButtonOnHoverBorder = HXColor{49, 83, 124, 255},
		.ButtonOnHoverBackground = HXColor{49, 83, 124, 255},
		.ButtonOnHoverText = HXColor{255, 255, 255, 255},
		.ButtonPressedBorder = HXColor{59, 93, 134, 255},
		.ButtonPressedBackground = HXColor{59, 93, 134, 255},
		.ButtonPressedText = HXColor{255, 255, 255, 255},
	};
}

void Render() {
	HXBufferPainter *Painter = Context.RenderContext->DefaultPainter()->CreateFromBuffer(Context.LocalBuffer);
	for (auto window = Context.Windows.rbegin(); window != Context.Windows.rend(); ++window) {
		Painter->DrawPainter((*window)->Painter, (*window)->Where);
	}
}

HXPoint ClipCoord(HXPoint Point) {
	return { Point.X - Context.CurrentWindow->Where.X, Point.Y - Context.CurrentWindow->Where.Y };
}

}
