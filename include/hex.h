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
#include <include/hex_button.h>
#include <include/hex_window.h>
#include <include/hex_text.h>

struct HXWindow;
struct HXRuntimeContext;
struct HXTheme;

namespace HX {
/**
 * Get the runtime context of HiEasyX
 * @return The runtime context of HiEasyX
 */
HXRuntimeContext &GetContext();

/**
 * Get the theme of the HiEasyX
 * @return The theme of the HiEasyX
 */
HXTheme &GetTheme();

/**
 * Create the theme for HiEasyX
 */
void CreateTheme();

/**
 * Setting the API of the current OS
 * @param API The API of the OS
 */
void OSAPI(HXOSOperation *API);

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

/**
 * Clipping the coord into the relative coord
 * @param Point The point needed to clip
 * @return The clipped coord
 */
HXPoint ClipCoord(HXPoint Point);
}

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
	HXString         Title;
	HXPoint          Size;
	HXPoint          Where;
	HXBufferPainter *Painter;
	bool             Folded;
	HXGInt           BaseLine = 50;

	~HXWindow() {
		delete Painter;
	}
};

/**
 * The theme structure for HiEasyX
 */
struct HXTheme {
	HXColor WindowBackground;
	HXColor WindowTitleText;
	HXColor WindowTitleBackground;
	HXColor ButtonBorder;
	HXColor ButtonBackground;
	HXColor ButtonText;
	HXColor ButtonOnHoverBorder;
	HXColor ButtonOnHoverBackground;
	HXColor ButtonOnHoverText;
	HXColor ButtonPressedBorder;
	HXColor ButtonPressedBackground;
	HXColor ButtonPressedText;
};

/**
 * The runtime context, including all value for UI running
 */
struct HXRuntimeContext {
	std::vector<HXMessage>  MessageQuery;
	std::vector<HXWindow *> Windows;
	HXWindow *              CurrentWindow = nullptr;
	HXContext *             RenderContext = nullptr;
	HXOSOperation *         OSAPI         = nullptr;
	void *                  LocalBuffer   = nullptr;
	HXString                LastError;
	bool                    Initialized = false;
	bool                    Win         = true;
};
