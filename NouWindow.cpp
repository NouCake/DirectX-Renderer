#include "NouWindow.h"

NouWindow::WindowClass NouWindow::WindowClass::wndClass;

NouWindow::WindowClass::WindowClass() noexcept
	: hInst(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hCursor = nullptr;
	wc.hIcon = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();

	RegisterClassEx(&wc);
}


NouWindow::WindowClass::~WindowClass() noexcept
{
	UnregisterClass(GetName(), GetInstance());
}

const char* NouWindow::WindowClass::GetName() noexcept
{
	return wndClassName;
}

HINSTANCE NouWindow::WindowClass::GetInstance() noexcept
{
	return wndClass.hInst;
}


NouWindow::NouWindow(int width, int height, const char* name) noexcept
{
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;

	DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
	AdjustWindowRect(&wr, style, FALSE);
	hWnd = CreateWindow(WindowClass::GetName(), name,
		style,
		CW_USEDEFAULT, CW_USEDEFAULT,
		wr.right - wr.left,
		wr.bottom - wr.top,
		nullptr, nullptr,
		WindowClass::GetInstance(),
		this
	);


	ShowWindow(hWnd, SW_SHOWDEFAULT);

	NouWindow* tt = this;

}

NouWindow::~NouWindow() {
	DestroyWindow(hWnd);
}

LRESULT WINAPI NouWindow::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (msg == WM_NCCREATE)
	{
		const CREATESTRUCTW* const cr = reinterpret_cast<CREATESTRUCTW*>(lParam); 
		NouWindow* const window = static_cast<NouWindow*>(cr->lpCreateParams);

		SetWindowLongPtr(window->hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
		SetWindowLongPtr(window->hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&NouWindow::HandleMsgThunk));

		return window->HandleMsg(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI NouWindow::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	NouWindow* window = reinterpret_cast<NouWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return window->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT NouWindow::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg) {
		case WM_KEYDOWN:
			SetWindowText(hWnd, "SuperDuper");
			return 0;
	}
	

	return DefWindowProc(hWnd, msg, wParam, lParam);
}