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
 * \file hex.h
 * \brief The HiEasyX major code
 */

#pragma once

#include <include/hex_string.h>
#include <include/impl/hex_impl.h>

namespace HX {
/**
 * Create the theme for HiEasyX
 */
void CreateTheme();

/**
 * Setting the buffer where the rendering result will be displayed
 * @param Buffer The buffer to be rendered
 */
void SetBuffer(void *Buffer);

/**
 * Getting the last error in string format
 * @return The last error in string format
 */
HXString GetLastError();

/**
 * Creating a window, and select it into the working window,
 * the window will locate at the origin point by default. The
 * size of the window by default is 200x300
 * @param Title The title of the window
 * @param Folded Whether the window is folded, setting as true to fold the window
 * @param Size The size of the window
 */
void Window(const HXString &Title, bool Folded = false, HXPoint Size = {200, 300});

/**
 * Relocating the window
 * @param Where Where the window will locate
 */
void WindowLocate(HXPoint Where);

void Button(const HXString &Title);

/**
 * Init the HiEasyX UI, preparing for UI layout
 * @param RenderContext The context for rendering
 */
void Begin(HXContext *RenderContext);

/**
 * End the UI layout progress
 */
void End();

/**
 * Check whether the operation has any error
 * @return If there is an error, returning true, nor returning false
 */
bool Wined();

/**
 * Render the UI to the buffer
 */
void Render();
}
