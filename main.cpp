#include <graphics.h>

#include <include/hex.h>
#include <include/impl/EasyX/hex_impl_easyx.h>

#include <format>

int main() {
	initgraph(640, 480);

	HXMessageSenderImpl sender;
	HXContextImpl		context;

	setbkcolor(RGB(21, 22, 23));

	BeginBatchDraw();

	while (true) {
		cleardevice();

		ExMessage message;
		while (peekmessage(&message)) {

		}

		HX::Begin();


		HX::End();

		HX::SetBuffer(HX::GetHXBuffer(GetWorkingImage()));
		HX::Render();

		_flushall();

		FlushBatchDraw();

		Sleep(16);
	}

	return 0;
}
