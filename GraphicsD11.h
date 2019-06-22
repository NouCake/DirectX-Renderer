#pragma once

#include "NouWin.h"

#include <d3d11.h>
#include <dxgi1_4.h>
#include <wrl.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

class GraphicsD11
{
public:
	GraphicsD11(HWND hWnd);
	GraphicsD11(const GraphicsD11&) = delete;
	GraphicsD11& operator=(const GraphicsD11&) = delete;
	~GraphicsD11() = default;

	void OnFrameEnd();
	void ClearBuffer(float r, float g, float b, float a);

private:
	HRESULT res;

	Microsoft::WRL::ComPtr <ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr <IDXGISwapChain> pSwap;
	Microsoft::WRL::ComPtr <ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr <ID3D11RenderTargetView> pTarget;

};

