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
 * \file hex_text.cpp
 * \brief The text for HiEasyX
 */

#include <include/hex.h>
#include <include/hex_text.h>

namespace HX {
TextProfile::TextProfile() {
	auto& theme = GetTheme();
	Font  = HXFont{};
	Color = theme.WindowTitleText;
}

void Text(const HXString &Title) {
	auto& theme = GetTheme();
	auto& context = GetContext();
	constexpr HXGInt ControlGap = 5;

	if (context.CurrentWindow->Folded) {
		return;
	}

	constexpr HXGInt leftGap = 10;

	context.CurrentWindow->Painter->Begin();
	context.CurrentWindow->Painter->DrawText(Title, HXFont{}, {leftGap, context.CurrentWindow->BaseLine},
											 theme.WindowTitleText, 18);
	context.CurrentWindow->Painter->End();

	context.CurrentWindow->BaseLine += context.CurrentWindow->Painter->MeasureText(Title, HXFont{}, 18).Bottom +
		ControlGap;
}

void Text(const HXString &Title, TextProfile &Profile) {
	auto& context = GetContext();

	constexpr HXGInt ControlGap = 5;

	if (context.CurrentWindow->Folded) {
		return;
	}

	constexpr HXGInt leftGap = 10;

	context.CurrentWindow->Painter->Begin();
	context.CurrentWindow->Painter->DrawText(Title, Profile.Font, {leftGap, context.CurrentWindow->BaseLine},
											 Profile.Color, Profile.Height);
	context.CurrentWindow->Painter->End();

	context.CurrentWindow->BaseLine += context.CurrentWindow->Painter->MeasureText(Title, HXFont{}, Profile.Height).
		Bottom + ControlGap;
}
}