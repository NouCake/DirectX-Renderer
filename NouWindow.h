#pragma once
#include "settings.h"

#include "NouWin.h"
#include "NouException.h"
#include "GraphicsD11.h"


class NouWindow
{

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
	GraphicsD11& Gfx();

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
	GraphicsD11* pGfx;
};