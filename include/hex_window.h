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
 * \file hex_window.h
 * \brief The window for the HiEasyX
 */
 
#pragma once

#include <include/hex_string.h>
#include <include/impl/hex_impl.h>

namespace HX {
/**
 * The profile for a window
 */
struct WindowProfile {
	HXPoint Size = {300, 200};

	// Set the size to a value less than 0(including 0) to put these setting unused
	// The MinSize, if the user does not set a minimum value, it will be put as a value
	// which can contain the folding icon of the window
	HXPoint MinSize         = {-1, -1};
	HXPoint MaxSize         = {-1, -1};

	HXPoint LastSize        = {0, 0};
	HXPoint Position        = {0, 0};
	bool    InDrag          = false;
	bool    Folded          = false;
	bool    InAllSize       = false;
	bool    InWidthSize     = false;
	bool    InHeightSize    = false;
	bool    InCursorStyling = false;
	HXGInt  DeltaX          = 0;
	HXGInt  DeltaY          = 0;
	HXGInt  OriginX         = 0;
	HXGInt  OriginY         = 0;
};

/**
 * Creating a window, and select it into the working window,
 * the window will locate at the origin point by default.
 * @param Title The title of the window
 * @param Profile The profile for a window
 */
void Window(const HXString &Title, WindowProfile &Profile);
}