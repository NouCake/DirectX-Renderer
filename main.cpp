#include "NouWindow.h"


int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR	lpCmdLine,
	int		cCmdShow)
{

	NouWindow window(640, 480, "Super Title");

	NouWindow* wn = &window;

	


	MSG msg;
	BOOL gResult;
	while ((gResult = GetMessage(&msg, 0, 0, 0)) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (gResult == -1) {
		return -1;
	}
	else
	{
		return msg.wParam;
	}

}