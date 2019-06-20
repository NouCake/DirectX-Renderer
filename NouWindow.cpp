#include "NouWindow.h"
#include "resource.h"
#include <sstream>

NouWindow::WindowClass NouWindow::WindowClass::wndClass;
int NouWindow::LastQuitCode;

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
	wc.hIcon = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 128, 128, 0));
	wc.hIconSm = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 32, 32, 0));
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

// #### Exception

NouWindow::Exception::Exception(int line, const char* file, HRESULT hr) noexcept
	:
	NouException(line, file),
	hr(hr)
{}

std::string NouWindow::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
	char* pMsgBuf = nullptr;
	DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
	);
	if (nMsgLen == 0)
	{
		return "Unidentified error code";
	}
	std::string errorString = pMsgBuf;
	LocalFree(pMsgBuf);
	return errorString;
}

const char* NouWindow::Exception::GetType() const noexcept
{
	return "NouWindow Exception";
}

HRESULT NouWindow::Exception::GetErrorCode() const noexcept
{
	return hr;
}

std::string NouWindow::Exception::GetErrorString() const noexcept
{
	return TranslateErrorCode(hr);
}

const char* NouWindow::Exception::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] " << GetErrorCode() << std::endl
		<< "[Description] " << GetErrorString() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

// #### NouWindow


NouWindow::NouWindow(int width, int height, const char* name)
{
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;

	DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

	if (AdjustWindowRect(&wr, style, FALSE) == 0) {
		throw CHWND_LAST_EXCEPT();
	}

	this->width = wr.right - wr.left;
	this->height = wr.bottom - wr.top;

	hWnd = CreateWindow(
		WindowClass::GetName(), name,
		style,
		CW_USEDEFAULT, CW_USEDEFAULT,
		wr.right - wr.left,
		wr.bottom - wr.top,
		nullptr, nullptr,
		WindowClass::GetInstance(),
		this
	);
	if (hWnd == nullptr)
	{
		throw CHWND_LAST_EXCEPT();
	}


	ShowWindow(hWnd, SW_SHOWDEFAULT);


}

NouWindow::~NouWindow() {
	DestroyWindow(hWnd);
}

void NouWindow::SetTitle(const char* title)
{
	SetWindowText(hWnd, title);
}

bool NouWindow::ProcessMessage()
{
	MSG msg;

	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			NouWindow::LastQuitCode = (int)msg.wParam;
			return true;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}

	return false;
}

LRESULT WINAPI NouWindow::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (msg == WM_NCCREATE)
	{
		const CREATESTRUCTW* const cr = reinterpret_cast<CREATESTRUCTW*>(lParam);
		NouWindow* const window = static_cast<NouWindow*>(cr->lpCreateParams);

		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&NouWindow::HandleMsgThunk));

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
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;
	}
	

	return DefWindowProc(hWnd, msg, wParam, lParam);
}