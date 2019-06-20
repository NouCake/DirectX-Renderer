#include "Graphics.h"
#include "NouException.h"

Graphics::Graphics(HWND hWnd)
{

	HRESULT res;
	res = D3D12CreateDevice(
		nullptr,
		D3D_FEATURE_LEVEL_12_0,
		__uuidof(ID3D12Device),
		(void**)&pDevice
	);

	if (FAILED(res)) {
		throw NouException(__LINE__, __FILE__);
	}


	IDXGIFactory4* pFactory = nullptr;
	res = CreateDXGIFactory1(__uuidof(IDXGIFactory4), (void**)&pFactory);
	if (FAILED(res)) {
		throw NouException(__LINE__, __FILE__);
	}


	D3D12_COMMAND_QUEUE_DESC cd = {};
	cd.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	cd.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	cd.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	cd.NodeMask = 0;

	res = pDevice->CreateCommandQueue(&cd, __uuidof(ID3D12CommandQueue), (void**)&pCommandQueue);
	if (FAILED(res)) {
		throw NouException(__LINE__, __FILE__);
	}

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


	res = pFactory->CreateSwapChain(pCommandQueue, &sd, &pSwap);
	if (FAILED(res)) {
		throw NouException(res, __FILE__);
	}

	pFactory->Release();

	D3D12_DESCRIPTOR_HEAP_DESC rc = {};
	rc.NumDescriptors = 2;
	rc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	res = pDevice->CreateDescriptorHeap(&rc, __uuidof(ID3D12DescriptorHeap), (void**)&rtViewHeap);
	if (FAILED(res)) {
		throw NouException(res, __FILE__);
	}

	res = pSwap->GetBuffer(0, __uuidof(ID3D12Resource), (void**)& pBackBuffer);
	if (FAILED(res)) {
		throw NouException(res, __FILE__);
	}

	D3D12_CPU_DESCRIPTOR_HANDLE rtHandle = rtViewHeap->GetCPUDescriptorHandleForHeapStart();
	//unsigned int rtSize = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	pDevice->CreateRenderTargetView(pBackBuffer, nullptr, rtHandle);

	res = pDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		__uuidof(ID3D12CommandAllocator),
		(void**)& pCmdAlloc);

	if (FAILED(res)) {
		throw NouException(res, __FILE__);
	}
	
	
	res = pDevice->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		pCmdAlloc,
		nullptr,
		__uuidof(ID3D12GraphicsCommandList),
		(void**)& pCmdList);
	if (FAILED(res)) {
		throw NouException(res, __FILE__);
	}

	res = pCmdList->Close();
	if (FAILED(res)) {
		throw NouException(res, __FILE__);
	}

	res = pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void**)&pFence);
	if (FAILED(res)) {
		throw NouException(res, __FILE__);
	}

	hFence = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);
	if (hFence == NULL) {
		throw NouException(__LINE__, __FILE__);
	}

	valFence = 1;
}

Graphics::~Graphics()
{

	if (pDevice != nullptr)
	{
		pDevice->Release();
	}
	

	if (pSwap != nullptr)
	{
		pSwap->Release();
	}

	if (pCommandQueue != nullptr)
	{
		pCommandQueue->Release();
	}

	if (rtViewHeap != nullptr)
	{
		rtViewHeap->Release();
	}

}

void Graphics::OnFrameEnd()
{
	HRESULT res;
	D3D12_RESOURCE_BARRIER barrier;
	D3D12_CPU_DESCRIPTOR_HANDLE renderTargetViewHandle;
	unsigned int renderTargetViewDescriptorSize;
	float color[4];
	ID3D12CommandList* ppCommandLists[1];
	unsigned long long fenceToWaitFor;


	res = pCmdAlloc->Reset();
	if (FAILED(res)) {
		throw NouException(__LINE__, __FILE__);
	}

	res = pCmdList->Reset(pCmdAlloc, pPipeState);
	if (FAILED(res)) {
		throw NouException(__LINE__, __FILE__);
	}


	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = pBackBuffer;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	pCmdList->ResourceBarrier(1, &barrier);

	renderTargetViewHandle = rtViewHeap->GetCPUDescriptorHandleForHeapStart();
	pCmdList->OMSetRenderTargets(1, &renderTargetViewHandle, FALSE, NULL);

	color[0] = 0.5;
	color[1] = 0.5;
	color[2] = 0.5;
	color[3] = 1.0;
	pCmdList->ClearRenderTargetView(renderTargetViewHandle, color, 0, NULL);

	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	pCmdList->ResourceBarrier(1, &barrier);

	// Close the list of commands.
	res = pCmdList->Close();
	if (FAILED(res)) {
		throw NouException(__LINE__, __FILE__);
	}

	// Load the command list array (only one command list for now).
	ppCommandLists[0] = pCmdList;

	// Execute the list of commands.
	pCommandQueue->ExecuteCommandLists(1, ppCommandLists);

	res = pSwap->Present(1u, 0u);
	if (FAILED(res)) {
		throw NouException(res, __FILE__);
	}

	// Signal and increment the fence value.
	fenceToWaitFor = valFence;
	res = pCommandQueue->Signal(pFence, fenceToWaitFor);
	if (FAILED(res)) {
		throw NouException(__LINE__, __FILE__);
	}
	valFence++;

	// Wait until the GPU is done rendering.
	if (pFence->GetCompletedValue() < fenceToWaitFor)
	{
		res = pFence->SetEventOnCompletion(fenceToWaitFor, hFence);
		if (FAILED(res)) {
			throw NouException(__LINE__, __FILE__);
		}

		WaitForSingleObject(hFence, INFINITE);
	}
}