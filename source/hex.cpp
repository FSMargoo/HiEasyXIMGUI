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

	~HXWindow() {
		delete Painter;
	}
};

struct HXTheme {
	HXColor WindowBackground;
};

/**
 * The runtime context, including all value for UI running
 */
struct HXRuntimeContext {
	std::vector<HXWindow> Windows;
	HXWindow *            CurrentWindow = nullptr;
	HXContext *           RenderContext = nullptr;
	void *                LocalBuffer   = nullptr;
	HXString              LastError;
	bool                  Initialized = false;
	bool                  Win         = true;
	HXTheme				  Theme;
};

namespace HX {
HXRuntimeContext Context;

void SetBuffer(void *Buffer) {
	Context.LocalBuffer = Buffer;
}

HXString GetLastError() {
	return Context.LastError;
}

void Window(const HXString &Title) {
	Context.Windows.push_back(HXWindow{.Title = Title, .Size = {100, 200}, .Where = HXPoint{0, 0}, .Controls{}});
	Context.CurrentWindow          = &Context.Windows.back();
	Context.CurrentWindow->Painter = Context.RenderContext->DefaultPainter()->CreateSubPainter(
		Context.CurrentWindow->Size.X, Context.CurrentWindow->Size.Y);

	Context.CurrentWindow->Painter->Begin();
	Context.CurrentWindow->Painter->Clear(Context.Theme.WindowBackground);
	Context.CurrentWindow->Painter->End();
}

void Begin() {
	if (Context.Initialized) {
		Context.Win       = false;
		Context.LastError = "End is needed for another UI layout progress";
	} else {
		Context = HXRuntimeContext{};
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

void Render() {

}

}
