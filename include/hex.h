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
 * The profile for a window
 */
struct WindowProfile {
	HXPoint Size     = {300, 200};
	HXPoint Position = {0, 0};
	bool    InDrag   = false;
	bool    Folded   = false;
	HXGInt  DeltaX   = 0;
	HXGInt  DeltaY   = 0;
};

/**
 * Creating a window, and select it into the working window,
 * the window will locate at the origin point by default.
 * @param Title The title of the window
 * @param Profile The profile for a window
 */
void Window(const HXString &Title, WindowProfile &Profile);

/**
 * The profile for a button
 */
struct ButtonProfile {
	bool OnHover   = false;
	bool OnPressed = false;
	// When the mouse button clicked down and didn't get up, OnHold
	// will be true
	bool    OnHold = false;
	HXPoint Size   = {300, 200};
};

/**
 * Creating a button, the size of the button is determined by the length of the title
 * @param Title The title of the button
 * @param Profile The profile of the button
 * @return If the button was pressed, returning true, nor returning false
 */
bool Button(const HXString &Title, ButtonProfile &Profile);

/**
 * The profile for a text label
 */
struct TextProfile {
	HXFont  Font;
	HXGInt  Height = 18;
	HXColor Color;

	TextProfile();
};

/**
 * Creating a text label
 * @param Title The title of the text
 */
void Text(const HXString &Title);

/**
 * Creating a text label with a text profile
 * @param Title The title of the text
 * @param Profile The pointer to the text profile
 */
void Text(const HXString &Title, TextProfile &Profile);

/**
 * Init the HiEasyX UI, preparing for UI layout
 * @param RenderContext The context for rendering
 */
void Begin(HXContext *RenderContext);

/**
 * Setting the message sender
 * @param Sender The sender to be set
 */
void MessageSender(HXMessageSender *Sender);

/**
 * Pushing the message to the message query
 * @param Message The message to be pushed
 * @param Size The size of the message structure
 */
void PushMessage(void *Message);

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
