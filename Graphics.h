#pragma once

#include "NouWin.h"

#include <d3d12.h>
#include <dxgi1_4.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

class Graphics
{
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();

	void OnFrameEnd();

private:
	ID3D12Device* pDevice = nullptr;
	//ID3D12DeviceContext pContext = nullptr;
	IDXGISwapChain* pSwap = nullptr;
	ID3D12Resource* pBackBuffer = nullptr;
	D3D12_CPU_DESCRIPTOR_HANDLE* pTarget = nullptr;

	ID3D12CommandQueue* pCommandQueue = nullptr;
	ID3D12CommandAllocator* pCmdAlloc = nullptr;
	ID3D12GraphicsCommandList* pCmdList = nullptr;
	ID3D12DescriptorHeap* rtViewHeap = nullptr;
	ID3D12PipelineState* pPipeState = nullptr;
	ID3D12Fence* pFence = nullptr;
	HANDLE hFence = nullptr;
	unsigned long long valFence;
	
};