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

/**
 * The type of controls
 */
enum class HXControlType {
	Button
};

/**
 * The status of a control
 */
enum class HXControlStatus {
	Button_Hovering
};

/**
 * The structure to store controls
 */
struct HXControl {
	HXControlType   Type;
	HXString        Id;
	HXControlStatus Status;
};

/**
 * The structure to store window, including title, position and controls
 */
struct HXWindow {
	HXString                                    Title;
	HXPoint                                     Size;
	HXPoint                                     Where;
	HXBufferPainter *                           Painter;
	std::vector<std::map<HXString, HXControl> > Controls;
	bool                                        Folded;

	~HXWindow() {
		delete Painter;
	}
};

struct HXTheme {
	HXColor WindowBackground;
	HXColor WindowTitle;
	HXColor WindowTitleBackground;
};

HXTheme Theme;

/**
 * The runtime context, including all value for UI running
 */
struct HXRuntimeContext {
	std::vector<void*>	  MessageQuery;
	std::vector<HXWindow> Windows;
	HXWindow *            CurrentWindow = nullptr;
	HXContext *           RenderContext = nullptr;
	void *                LocalBuffer   = nullptr;
	HXString              LastError;
	bool                  Initialized = false;
	bool                  Win         = true;
};

namespace HX {
HXRuntimeContext Context;
HXMessageSender *MsgSender;

void SetBuffer(void *Buffer) {
	Context.LocalBuffer = Buffer;
}

HXString GetLastError() {
	return Context.LastError;
}

void Window(const HXString &Title, bool Folded, HXPoint Size) {
	Context.Windows.push_back(HXWindow{.Title = Title, .Size = Size, .Where = HXPoint{0, 0}, .Controls{},
	                                   .Folded = Folded});
	Context.CurrentWindow = &Context.Windows.back();

	if (Folded == true) {
		Context.CurrentWindow->Painter = Context.RenderContext->DefaultPainter()->CreateSubPainter(
			Context.CurrentWindow->Size.X, 40);
	} else {
		Context.CurrentWindow->Painter = Context.RenderContext->DefaultPainter()->CreateSubPainter(
			Context.CurrentWindow->Size.X, Context.CurrentWindow->Size.Y);
	}

	// Draw Title Bar
	Context.CurrentWindow->Painter->Begin();
	Context.CurrentWindow->Painter->Clear(Theme.WindowBackground);
	Context.CurrentWindow->Painter->DrawFilledRectangle({0, 0, Size.X, 40}, Theme.WindowTitleBackground,
	                                                    Theme.WindowTitleBackground);
	Context.CurrentWindow->Painter->DrawFilledPolygon({
		                                                  {4, 15},
		                                                  {16, 15},
		                                                  {10, static_cast<HXGInt>(ceil(6 * sqrt(3) + 15))}
	                                                  }, Theme.WindowTitle);
	Context.CurrentWindow->Painter->DrawText(Title, HXFont{}, {20, 10}, Theme.WindowTitle, 20);
	Context.CurrentWindow->Painter->End();
}

void MessageSender(HXMessageSender *Sender) {
	MsgSender = Sender;
	if (Sender == nullptr) {
		Context.Win       = false;
		Context.LastError = "Invalid message sender";
	}
}

void PushMessage(void *Message) {
	Context.MessageQuery.push_back(Message);
}

void Begin(HXContext *RenderContext) {
	if (Context.Initialized) {
		Context.Win       = false;
		Context.LastError = "End is needed for another UI layout progress";
	} else {
		Context               = HXRuntimeContext{};
		Context.RenderContext = RenderContext;
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
		.WindowTitle = HXColor{255, 255, 255, 255},
		.WindowTitleBackground = HXColor{41, 74, 122, 255},
	};
}

void Render() {
	HXBufferPainter *Painter = Context.RenderContext->DefaultPainter()->CreateFromBuffer(Context.LocalBuffer);
	for (const auto &window : Context.Windows) {
		Painter->DrawPainter(window.Painter, window.Where);
	}
}

}
