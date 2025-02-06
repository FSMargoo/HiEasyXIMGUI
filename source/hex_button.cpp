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
 * \file hex_button.cpp
 * \brief The button for HiEasyX
 */

#include <include/hex.h>
#include <include/hex_button.h>

namespace HX {
bool Button(const HXString &Title, ButtonProfile &Profile) {
	auto &context = GetContext();
	auto &theme   = GetTheme();

	constexpr HXGInt ControlGap = 5;

	if (context.CurrentWindow->Folded) {
		return false;
	}

	const auto fontRect = context.CurrentWindow->Painter->MeasureText(Title, HXFont{}, 18);

	constexpr HXGInt leftGap    = 10;
	constexpr HXGInt contentGap = 10;

	const auto buttonRectangle = HXRect{leftGap, context.CurrentWindow->BaseLine,
	                                    leftGap + fontRect.Right + contentGap,
	                                    context.CurrentWindow->BaseLine + contentGap + fontRect.
	                                    Bottom};

	bool pressed = false;

	// Process the presse or hover
	for (auto &Message : context.MessageQuery) {
		auto mouse = ClipCoord({Message.MouseX, Message.MouseY});
		if (Message.Processed) {
			continue;
		}
		if (Message.MouseAction) {
			if (mouse.X >= buttonRectangle.Left && mouse.X <= buttonRectangle.Right &&
			    mouse.Y >= buttonRectangle.Top && mouse.Y <= buttonRectangle.Bottom) {
				Message.Processed = true;

				Profile.OnHover = true;
				if (Message.MouseLeftPressed) {
					Profile.OnHold = true;
				}
				if (Message.MouseLeftRelease) {
					Profile.OnHold = false;

					pressed = true;
				}
			} else {
				Profile.OnHover = false;
				Profile.OnHold  = false;
			}
		}
	}

	context.CurrentWindow->Painter->Begin();

	if (Profile.OnHold) {
		context.CurrentWindow->Painter->DrawFilledRectangle(buttonRectangle, theme.ButtonPressedBorder,
		                                                    theme.ButtonPressedBackground);
		context.CurrentWindow->Painter->DrawText(Title, HXFont{},
		                                         {leftGap + contentGap / 2,
		                                          context.CurrentWindow->BaseLine + contentGap / 2},
		                                         theme.ButtonPressedText, 18);
	} else if (Profile.OnHover) {
		context.CurrentWindow->Painter->DrawFilledRectangle(buttonRectangle, theme.ButtonOnHoverBorder,
		                                                    theme.ButtonOnHoverBackground);
		context.CurrentWindow->Painter->DrawText(Title, HXFont{},
		                                         {leftGap + contentGap / 2,
		                                          context.CurrentWindow->BaseLine + contentGap / 2},
		                                         theme.ButtonOnHoverText, 18);
	} else {
		context.CurrentWindow->Painter->
			DrawFilledRectangle(buttonRectangle, theme.ButtonBorder, theme.ButtonBackground);
		context.CurrentWindow->Painter->DrawText(Title, HXFont{},
		                                         {leftGap + contentGap / 2,
		                                          context.CurrentWindow->BaseLine + contentGap / 2}, theme.ButtonText,
		                                         18);
	}

	context.CurrentWindow->Painter->End();

	Profile.OnPressed = pressed;

	context.CurrentWindow->BaseLine += buttonRectangle.CalHeight() + ControlGap;

	return pressed;
}
}