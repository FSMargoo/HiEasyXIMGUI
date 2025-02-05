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

	while (true) {
		setbkcolor(RGB(0, 129, 129));
		cleardevice();

		ExMessage message;
		while (peekmessage(&message)) {

		}

		HX::Begin(&context);

		HX::Window("Hello World");

		HX::End();

		HX::SetBuffer(HX::GetHXBuffer(GetWorkingImage()));
		HX::Render();

		_flushall();

		FlushBatchDraw();

		Sleep(16);
	}

	return 0;
}
