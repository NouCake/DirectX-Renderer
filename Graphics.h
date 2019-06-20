#pragma once

#include "NouWin.h"

#include <d3d12.h>
#include <dxgi.h>

class Graphics
{
public:
	Graphics(HWND hWnd);
	~Graphics();

private:
	ID3D12Device* pDevice = nullptr;
	//ID3D12DeviceContext pContext = nullptr;
	IDXGISwapChain* pSwap = nullptr;
	
};