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
 * \file hex_window.cpp
 * \brief The window for the HiEasyX
 */

#include <include/hex.h>
#include <include/hex_window.h>

namespace HX {
void Window(const HXString &Title, WindowProfile &Profile) {
	auto &context = GetContext();
	auto &theme   = GetTheme();
	auto  window  = new HXWindow{.Title = Title, .Size = Profile.Size, .Where = HXPoint{0, 0},
	                           .Folded = Profile.Folded};
	context.Windows.emplace_back(window);
	context.CurrentWindow = context.Windows.back();

	context.CurrentWindow->Where = Profile.Position;

	if (Profile.Folded) {
		context.CurrentWindow->Painter = context.RenderContext->DefaultPainter()->CreateSubPainter(
			context.CurrentWindow->Size.X, 40);
	} else {
		context.CurrentWindow->Painter = context.RenderContext->DefaultPainter()->CreateSubPainter(
			context.CurrentWindow->Size.X, context.CurrentWindow->Size.Y);
	}

	auto windowBarRectangle = HXRect{Profile.Position.X, Profile.Position.Y, Profile.Position.X + Profile.Size.X,
	                                 Profile.Position.Y + 40};
	auto windowFoldBar = HXRect{4 + Profile.Position.X, 15 + Profile.Position.Y,
	                            16 + Profile.Position.X,
	                            static_cast<HXGInt>(ceil(6 * sqrt(3) + 15)) + Profile.Position.Y};
	auto windowRectangle = HXRect{
		Profile.Position.X, Profile.Position.Y, Profile.Position.X + Profile.Size.X, Profile.Position.Y + Profile.Size.Y
	};

	// Judge the window drag operation
	for (auto &Message : context.MessageQuery) {
		if (Message.Processed) {
			continue;
		}

		if (Message.MouseLeftPressed &&
		    Message.MouseX >= windowBarRectangle.Left && Message.MouseX <= windowBarRectangle.Right &&
		    Message.MouseY >= windowBarRectangle.Top && Message.MouseY <= windowBarRectangle.Bottom) {
			if (Message.MouseX >= windowFoldBar.Left && Message.MouseX <= windowFoldBar.Right &&
			    Message.MouseY >= windowFoldBar.Top && Message.MouseY <= windowBarRectangle.Bottom) {
				Profile.Folded = !Profile.Folded;
			} else {
				Profile.DeltaX = Message.MouseX - windowBarRectangle.Left;
				Profile.DeltaY = Message.MouseY - windowBarRectangle.Top;

				Profile.InDrag = true;
			}

			Message.Processed = true;
		} else if (Message.MouseLeftRelease) {
			Profile.InDrag          = false;
		}

		if (Message.MouseAction && Profile.InDrag) {
			Message.Processed = true;

			context.CurrentWindow->Where = {Message.MouseX - Profile.DeltaX, Message.MouseY - Profile.DeltaY};
		}

		if (Profile.Folded) {
			continue;
		}

		bool needCursorStyle = false;

		if (Message.MouseLeftRelease && Profile.InCursorStyling) {
			Profile.InWidthSize  = false;
			Profile.InAllSize    = false;
			Profile.InHeightSize = false;

			Profile.InCursorStyling = false;
			context.OSAPI->SetCursorStyle(HXCursorStyle::Normal);
		}

		// Process All Size
		if (Message.MouseAction && Message.MouseX >= windowRectangle.Right - 10 && Message.MouseY >=
		    windowRectangle.Bottom - 10 &&
		    Message.MouseX <= windowRectangle.Right && Message.MouseY <= windowRectangle.Bottom) {
			Message.Processed = true;

			if (Message.MouseLeftPressed) {
				Profile.InAllSize = true;
				Profile.OriginX   = Message.MouseX;
				Profile.OriginY   = Message.MouseY;
				Profile.LastSize  = Profile.Size;
			}

			Profile.InCursorStyling = true;
			context.OSAPI->SetCursorStyle(HXCursorStyle::ResizeNW);

			needCursorStyle = true;
		}

		// Process Width Size
		if (Message.MouseAction && Message.MouseX >= windowRectangle.Right - 10 && Message.MouseY >=
		    windowRectangle.Top &&
		    Message.MouseX <= windowRectangle.Right && Message.MouseY <= windowRectangle.Bottom && !Profile.InAllSize) {
			Message.Processed = true;

			if (Message.MouseLeftPressed) {
				Profile.InWidthSize = true;
				Profile.OriginX     = Message.MouseX;
				Profile.OriginY     = Message.MouseY;
				Profile.LastSize    = Profile.Size;
			}

			Profile.InCursorStyling = true;
			if (!needCursorStyle) {
				context.OSAPI->SetCursorStyle(HXCursorStyle::ResizeE);
			}

			needCursorStyle = true;
		}

		// Process Height Size
		if (Message.MouseAction && Message.MouseX >= windowRectangle.Left && Message.MouseY >=
		    windowRectangle.Bottom - 10 &&
		    Message.MouseX <= windowRectangle.Right && Message.MouseY <= windowRectangle.Bottom && !Profile.InAllSize) {
			Message.Processed = true;

			if (Message.MouseLeftPressed) {
				Profile.InHeightSize = true;
				Profile.OriginX      = Message.MouseX;
				Profile.OriginY      = Message.MouseY;
				Profile.LastSize     = Profile.Size;
			}

			Profile.InCursorStyling = true;
			if (!needCursorStyle) {
				context.OSAPI->SetCursorStyle(HXCursorStyle::ResizeN);
			}

			needCursorStyle = true;
		}

		if (!needCursorStyle && !(Profile.InAllSize || Profile.InWidthSize || Profile.InHeightSize)) {
			Profile.InCursorStyling = false;
			context.OSAPI->SetCursorStyle(HXCursorStyle::Normal);
		}

		if (Message.MouseAction && Profile.InAllSize) {
			Message.Processed = true;

			context.CurrentWindow->Size = {Profile.LastSize.X - (Profile.OriginX - Message.MouseX),
			                               Profile.LastSize.Y - (Profile.OriginY - Message.MouseY)};
		}
		if (Message.MouseAction && Profile.InWidthSize) {
			Message.Processed = true;

			context.CurrentWindow->Size = {Profile.LastSize.X - (Profile.OriginX - Message.MouseX),
			                               Profile.LastSize.Y};
		}
		if (Message.MouseAction && Profile.InHeightSize) {
			Message.Processed = true;

			context.CurrentWindow->Size = {Profile.LastSize.X,
			                               Profile.LastSize.Y - (Profile.OriginY - Message.MouseY)};
		}
	}

	Profile.Size = context.CurrentWindow->Size;

	bool vertUseMax = Profile.MaxSize.X > 0;
	bool horUseMax  = Profile.MaxSize.Y > 0;
	bool vertUseMin = Profile.MinSize.X > 0;
	bool horUseMin  = Profile.MinSize.Y > 0;

	if (!vertUseMin) {
		vertUseMin        = true;
		Profile.MinSize.X = 50;
	}
	if (!horUseMin) {
		horUseMin         = true;
		Profile.MinSize.Y = 50;
	}

	if (Profile.Size.X > Profile.MaxSize.X && vertUseMax) {
		Profile.Size.X = Profile.MaxSize.X;
	}
	if (Profile.Size.Y > Profile.MaxSize.Y && horUseMax) {
		Profile.Size.Y = Profile.MaxSize.Y;
	}
	if (Profile.Size.X < Profile.MinSize.X) {
		Profile.Size.X = Profile.MinSize.X;
	}
	if (Profile.Size.Y < Profile.MinSize.Y) {
		Profile.Size.Y = Profile.MinSize.Y;
	}

	const auto           rectangleHeight   = static_cast<HXGInt>(ceil(6 * sqrt(3) + 15));
	std::vector<HXPoint> rectangleVertexes = {
		{4, 15},
		{16, 15},
		{10, rectangleHeight},
	};
	if (Profile.Folded) {
		rectangleVertexes = {
			{4, rectangleHeight},
			{16, rectangleHeight},
			{10, 15},
		};
	}
	windowBarRectangle = HXRect{0, 0, Profile.Size.X, 40};

	// Draw Title Bar
	context.CurrentWindow->Painter->Begin();
	context.CurrentWindow->Painter->Clear(theme.WindowBackground);
	context.CurrentWindow->Painter->DrawFilledRectangle(windowBarRectangle, theme.WindowTitleBackground,
	                                                    theme.WindowTitleBackground);
	context.CurrentWindow->Painter->DrawFilledPolygon(rectangleVertexes, theme.WindowTitleText);
	context.CurrentWindow->Painter->DrawText(Title, HXFont{}, {20, 10}, theme.WindowTitleText, 20);
	context.CurrentWindow->Painter->End();

	Profile.Position = context.CurrentWindow->Where;
}
}
