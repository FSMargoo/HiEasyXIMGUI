#include <graphics.h>

#include <include/hex.h>
#include <include/impl/EasyX/hex_impl_easyx.h>

#include <format>

int main() {
	initgraph(640, 480);

	BeginBatchDraw();

	HX::HXInitForEasyX();
	if (!HX::Wined()) {
		printf("HX error : %s!", HX::GetLastError().c_str());

		exit(-1);
	}

	HX::WindowProfile windowProfile;

	while (true) {
		setbkcolor(RGB(0, 129, 129));
		cleardevice();

		HX::HXBegin();

		// Begin to process the message
		ExMessage message{};
		while (peekmessage(&message)) {
			HX::PushMessage(HX::GetHXMessage(&message));
		}

		HX::Window("Hello World", windowProfile);

		static HX::ButtonProfile btnProfile{};
		if (HX::Button("Hello World!", btnProfile)) {
			HX::Text("You clicked the button!");
		}
		if (btnProfile.OnHold) {
			auto profile        = HX::TextProfile{};
			profile.Height      = 20;
			profile.Color       = HXColor{255, 0, 0};
			profile.Font.Style  = HXFontStyle::Black;
			profile.Font.Family = "Times New Roman";
			profile.Font.Italic = true;

			HX::Text("You are holding the button!", profile);
		}
		if (btnProfile.OnHover) {
			HX::Text("You are hovering the button!");
		}

		HX::End();

		HX::SetBuffer(HX::GetHXBuffer(GetWorkingImage()));
		HX::Render();

		_flushall();

		FlushBatchDraw();

		Sleep(1);
	}

	return 0;
}
