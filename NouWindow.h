#pragma once

#include "NouWin.h"
#include "NouException.h"


class NouWindow
{
public:
	class Exception : public NouException
	{
	public:
		Exception(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept;
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT hr;
	};

private:
	class WindowClass
	{
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;

	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator = (const WindowClass&) = delete;
		static constexpr const char* wndClassName = "NouEngingeWindowClass";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};

public:
	NouWindow(int width, int height, const char* name);
	~NouWindow();
	NouWindow(const NouWindow&) = delete;
	NouWindow& operator = (const NouWindow&) = delete;
	void SetTitle(const char* title);

public:
	static bool ProcessMessage();
	static int LastQuitCode;
	 
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

private:
	int width;
	int height;
	HWND hWnd;
};

#define CHWND_EXCEPT( hr ) NouWindow::Exception( __LINE__,__FILE__,hr )
#define CHWND_LAST_EXCEPT() NouWindow::Exception( __LINE__,__FILE__,GetLastError() )