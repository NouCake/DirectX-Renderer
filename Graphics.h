#pragma once

#include "NouWin.h"

#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

class Graphics
{
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics() = default;

	void OnFrameEnd();
	void OnFrameStart();

private:
	HRESULT res;
	ID3D12Device* pDevice = nullptr;
	//ID3D12DeviceContext pContext = nullptr;
	IDXGISwapChain3* pSwap = nullptr;
	D3D12_CPU_DESCRIPTOR_HANDLE* pTarget = nullptr;

	void CreateSwapChain(HWND hWnd, IDXGIFactory4* pFactory);
	void CreateCommandQueue();
	void CreateDescHeap();
	void RenderTargetShit();
	void CommandShit();
	void FenceShit();

	Microsoft::WRL::ComPtr <ID3D12CommandQueue> pCommandQueue;
	Microsoft::WRL::ComPtr <ID3D12CommandAllocator> pCmdAlloc;
	Microsoft::WRL::ComPtr <ID3D12GraphicsCommandList> pCmdList;
	Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> rtViewHeap;
	Microsoft::WRL::ComPtr <ID3D12Resource> pBackBuffer[2];
	unsigned int mBufferIndex;

	Microsoft::WRL::ComPtr <ID3D12PipelineState> pPipeState;
	Microsoft::WRL::ComPtr <ID3D12Fence> pFence;
	HANDLE hFence = nullptr;
	unsigned long long mFenceVal;
	
};


#define CHECK_HR_EXCEPT() if(FAILED(res)) throw NouWindow::Exception( __LINE__, __FILE__, res);