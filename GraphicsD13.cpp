#include "GraphicsD13.h"
#include "NouException.h"
#include "NouWindow.h"

GraphicsD13::GraphicsD13(HWND hWnd)
{
	res = D3D12CreateDevice(
		nullptr,
		D3D_FEATURE_LEVEL_12_1,
		__uuidof(ID3D12Device),
		(void**)&pDevice
	);
	CHECK_HR_EXCEPT();

	CreateCommandQueue();

	IDXGIFactory4* pFactory = nullptr;
	res = CreateDXGIFactory1(__uuidof(IDXGIFactory4), (void**)&pFactory);
	CHECK_HR_EXCEPT();

	CreateSwapChain(hWnd, pFactory);

	pFactory->Release();

	CreateDescHeap();
	RenderTargetShit();
	CommandShit();
	FenceShit();

}


void GraphicsD13::CreateSwapChain(HWND hWnd, IDXGIFactory4* pFactory)
{
	DXGI_SWAP_CHAIN_DESC sd = {};

	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.Flags = 0;

	IDXGISwapChain* swap;
	res = pFactory->CreateSwapChain(pCommandQueue.Get(), &sd, &swap);
	CHECK_HR_EXCEPT();

	res = swap->QueryInterface(__uuidof(IDXGISwapChain3), (void**)&pSwap);
	CHECK_HR_EXCEPT();
}

void GraphicsD13::CreateCommandQueue()
{
	D3D12_COMMAND_QUEUE_DESC cd = {};
	cd.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	cd.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	cd.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	cd.NodeMask = 0;

	res = pDevice->CreateCommandQueue(&cd, __uuidof(ID3D12CommandQueue), (void**)& pCommandQueue);
	CHECK_HR_EXCEPT();
}

void GraphicsD13::CreateDescHeap()
{
	D3D12_DESCRIPTOR_HEAP_DESC rc = {};
	rc.NumDescriptors = 2;
	rc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	res = pDevice->CreateDescriptorHeap(&rc, __uuidof(ID3D12DescriptorHeap), (void**)& rtViewHeap);
	CHECK_HR_EXCEPT();
}

void GraphicsD13::RenderTargetShit()
{
	D3D12_DESCRIPTOR_HEAP_DESC rc = {};
	rc.NumDescriptors = 2;
	rc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	res = pDevice->CreateDescriptorHeap(
		&rc,
		__uuidof(ID3D12DescriptorHeap),
		(void**)& rtViewHeap
	);
	CHECK_HR_EXCEPT();

	D3D12_CPU_DESCRIPTOR_HANDLE rtHandle;
	rtHandle = rtViewHeap->GetCPUDescriptorHandleForHeapStart();

	unsigned int rtSize;
	rtSize = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	res = pSwap->GetBuffer(0, __uuidof(ID3D12Resource), (void**)& pBackBuffer[0]);
	CHECK_HR_EXCEPT();
	pDevice->CreateRenderTargetView(pBackBuffer[0].Get(), nullptr, rtHandle);

	rtHandle.ptr += rtSize;

	res = pSwap->GetBuffer(1, __uuidof(ID3D12Resource), (void**)& pBackBuffer[1]);
	CHECK_HR_EXCEPT();
	pDevice->CreateRenderTargetView(pBackBuffer[1].Get(), nullptr, rtHandle);

	mBufferIndex = pSwap->GetCurrentBackBufferIndex();
}

void GraphicsD13::CommandShit()
{
	res = pDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		__uuidof(ID3D12CommandAllocator),
		(void**)& pCmdAlloc);
	CHECK_HR_EXCEPT();

	res = pDevice->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		pCmdAlloc.Get(),
		nullptr,
		__uuidof(ID3D12GraphicsCommandList),
		(void**)& pCmdList);
	CHECK_HR_EXCEPT();

	res = pCmdList->Close();
	CHECK_HR_EXCEPT();
}

void GraphicsD13::FenceShit()
{
	res = pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void**)& pFence);
	CHECK_HR_EXCEPT();

	hFence = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);
	CHECK_HR_EXCEPT();

	mFenceVal = 1;
}

void GraphicsD13::OnFrameStart()
{

	D3D12_RESOURCE_BARRIER barrier;
	unsigned int rtSize;

	res = pCmdAlloc->Reset();
	CHECK_HR_EXCEPT();

	res = pCmdList->Reset(pCmdAlloc.Get(), pPipeState.Get());
	CHECK_HR_EXCEPT();

	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = pBackBuffer[mBufferIndex].Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	pCmdList->ResourceBarrier(1, &barrier);

	curRtHandle = rtViewHeap->GetCPUDescriptorHandleForHeapStart();
	rtSize = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	if (mBufferIndex == 1)
	{
		curRtHandle.ptr += rtSize;
	}

	pCmdList->OMSetRenderTargets(1, &curRtHandle, FALSE, NULL);


	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	pCmdList->ResourceBarrier(1, &barrier);

}

struct Vertex
{
	float x;
	float y;
};

const Vertex verts[] =
{
	{0.0f, 0.5f},
	{0.5f,-0.5f},
	{-0.5f, -0.5f}
};

void GraphicsD13::ClearBuffer(float r, float g, float b, float a)
{

	const float color[4] = { r, g, b, a };
	pCmdList->ClearRenderTargetView(curRtHandle, color, 0, NULL);



}

void GraphicsD13::OnFrameEnd()
{



	ID3D12CommandList* ppCommandLists[1];
	unsigned long long fenceToWaitFor;

	res = pCmdList->Close();
	CHECK_HR_EXCEPT();

	ppCommandLists[0] = pCmdList.Get();

	pCommandQueue->ExecuteCommandLists(1, ppCommandLists);

	res = pSwap->Present(0, 0);
	if (FAILED(res)) {
		if (res == DXGI_ERROR_DEVICE_REMOVED) {
			res = pDevice->GetDeviceRemovedReason();
		}
		throw NouWindow::Exception(__LINE__, __FILE__, res);
	}

	fenceToWaitFor = mFenceVal;
	res = pCommandQueue->Signal(pFence.Get(), fenceToWaitFor);
	CHECK_HR_EXCEPT();
	mFenceVal++;

	if (pFence->GetCompletedValue() < fenceToWaitFor)
	{
		res = pFence->SetEventOnCompletion(fenceToWaitFor, hFence);
		CHECK_HR_EXCEPT();
		WaitForSingleObject(hFence, INFINITE);
	}

	mBufferIndex == 0 ? mBufferIndex = 1 : mBufferIndex = 0;
}