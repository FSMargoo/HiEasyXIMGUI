#include <graphics.h>

#include <include/hex.h>
#include <include/impl/EasyX/hex_impl_easyx.h>

#include <format>

int main() {
	initgraph(640, 480);

	HXMessageSenderImpl sender;
	HXContextImpl		context;

	BeginBatchDraw();

	HX::CreateTheme();
	HX::MessageSender(&sender);

	HX::WindowProfile windowProfile;
	HX::WindowProfile windowProfile2;
	windowProfile2.Position = {300, 300};

	while (true) {
		setbkcolor(RGB(0, 129, 129));
		cleardevice();

		HX::Begin(&context);

		// Begin to process the message
		ExMessage message{};
		while (peekmessage(&message)) {
			HX::PushMessage(HX::GetHXMessage(&message));
		}

		HX::Window("Hello World", &windowProfile);
		HX::Window("Hello World 2", &windowProfile2);

		HX::End();

		HX::SetBuffer(HX::GetHXBuffer(GetWorkingImage()));
		HX::Render();

		if (windowProfile.InDrag) {
			settextcolor(GREEN);
			outtextxy(100, 100, "窗口正在被拖动");
		}
		else {
			settextcolor(RED);
			outtextxy(100, 100, "窗口没有被拖动");
		}
		if (windowProfile.Folded) {
			settextcolor(GREEN);
			outtextxy(100, 140, "窗口被折叠");
		}
		else {
			settextcolor(RED);
			outtextxy(100, 140, "窗口展开");
		}

		_flushall();

		FlushBatchDraw();

		Sleep(1);
	}

	return 0;
}
