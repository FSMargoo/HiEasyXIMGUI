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

		HX::End();

		HX::SetBuffer(HX::GetHXBuffer(GetWorkingImage()));
		HX::Render();

		_flushall();

		FlushBatchDraw();

		Sleep(1);
	}

	return 0;
}
