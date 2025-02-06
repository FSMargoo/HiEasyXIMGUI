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
	std::vector<HXMessage> MessageQuery;
	std::vector<HXWindow>  Windows;
	HXWindow *             CurrentWindow = nullptr;
	HXContext *            RenderContext = nullptr;
	void *                 LocalBuffer   = nullptr;
	HXString               LastError;
	bool                   Initialized = false;
	bool                   Win         = true;
};

namespace HX {
HXRuntimeContext Context;
HXMessageSender *MsgSender;

HXPoint _ClipToLocalWindowCoord(HXPoint Coord) {
	return {Coord.X - Context.CurrentWindow->Where.X, Coord.Y - Context.CurrentWindow->Where.Y};
}

void SetBuffer(void *Buffer) {
	Context.LocalBuffer = Buffer;
}

HXString GetLastError() {
	return Context.LastError;
}

void Window(const HXString &Title, WindowProfile *Profile) {
	Context.Windows.push_back(HXWindow{.Title = Title, .Size = Profile->Size, .Where = HXPoint{0, 0}, .Controls{},
	                                   .Folded = Profile->Folded});
	Context.CurrentWindow = &Context.Windows.back();

	Context.CurrentWindow->Where = Profile->Position;

	if (Profile->Folded) {
		Context.CurrentWindow->Painter = Context.RenderContext->DefaultPainter()->CreateSubPainter(
			Context.CurrentWindow->Size.X, 40);
	} else {
		Context.CurrentWindow->Painter = Context.RenderContext->DefaultPainter()->CreateSubPainter(
			Context.CurrentWindow->Size.X, Context.CurrentWindow->Size.Y);
	}


	auto windowBarRectangle = HXRect{Profile->Position.X, Profile->Position.Y, Profile->Position.X + Profile->Size.X, Profile->Position.Y + 40};
	auto windowFoldBar = HXRect{ 4 + Profile->Position.X, 15 + Profile->Position.Y,
		16 + Profile->Position.X, static_cast<HXGInt>(ceil(6 * sqrt(3) + 15)) + Profile->Position.Y };

	// Judge the window drag
	for (auto &Message : Context.MessageQuery) {
		if (Message.MouseLeftPressed &&
		    Message.MouseX >= windowBarRectangle.Left && Message.MouseX <= windowBarRectangle.Right &&
		    Message.MouseY >= windowBarRectangle.Top && Message.MouseY <= windowBarRectangle.Bottom) {
			if (Message.MouseX >= windowFoldBar.Left && Message.MouseX <= windowFoldBar.Right &&
				Message.MouseY >= windowFoldBar.Top && Message.MouseY <= windowBarRectangle.Bottom) {
				Profile->Folded = !Profile->Folded;
			}
			else {
				Profile->DeltaX = Message.MouseX - windowBarRectangle.Left;
				Profile->DeltaY = Message.MouseY - windowBarRectangle.Top;

				Profile->InDrag = true;
			}
		}
		else if (Message.MouseLeftRelease && Profile->InDrag) {
			Profile->InDrag = false;
		}

		if (Message.MouseAction && Profile->InDrag) {
			Context.CurrentWindow->Where = { Message.MouseX - Profile->DeltaX, Message.MouseY - Profile->DeltaY };
		}
	}

	const auto rectangleHeight = static_cast<HXGInt>(ceil(6 * sqrt(3) + 15));
	std::vector<HXPoint> rectangleVertexes = {
		{4, 15 },
		{16, 15 },
		{10, rectangleHeight },
	};
	if (Profile->Folded) {
		rectangleVertexes = {
			{4, rectangleHeight },
			{16, rectangleHeight },
			{10, 15 },
		};
	}
	windowBarRectangle = HXRect{0, 0, Profile->Size.X, 40};

	// Draw Title Bar
	Context.CurrentWindow->Painter->Begin();
	Context.CurrentWindow->Painter->Clear(Theme.WindowBackground);
	Context.CurrentWindow->Painter->DrawFilledRectangle(windowBarRectangle, Theme.WindowTitleBackground,
	                                                    Theme.WindowTitleBackground);
	Context.CurrentWindow->Painter->DrawFilledPolygon(rectangleVertexes, Theme.WindowTitle);
	Context.CurrentWindow->Painter->DrawText(Title, HXFont{}, {20, 10 }, Theme.WindowTitle, 20);
	Context.CurrentWindow->Painter->End();

	Profile->Position = Context.CurrentWindow->Where;
}

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
