#include "GraphicsD11.h"
#include "NouWindow.h"


#include <vector>

namespace dx = DirectX;

GraphicsD11::GraphicsD11(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	res = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwap,
		&pDevice,
		nullptr,
		&pContext
	);
	CHECK_HR_EXCEPT();

	Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;
	res = pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);
	CHECK_HR_EXCEPT();

	res = pDevice->CreateRenderTargetView(
		pBackBuffer.Get(),
		nullptr,
		pTarget.GetAddressOf()
	);
	CHECK_HR_EXCEPT();


	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);
	//SET VIEWPORT
	D3D11_VIEWPORT vp = {};
	vp.Width = 720;
	vp.Height = 480;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pContext->RSSetViewports(1u, &vp);
}

void GraphicsD11::OnFrameEnd()
{
	res = pSwap->Present(1u, 0u);
	if (FAILED(res)) {
		res = pDevice->GetDeviceRemovedReason();
		throw NouException::HrException(__LINE__, __FILE__, res);
	}
}

void GraphicsD11::ClearBuffer(float r, float g, float b, float a)
{

	const float color[] = { r,g,b,1.0f };
	pContext->ClearRenderTargetView(pTarget.Get(), color);
}

/*
VertexInput vi[512] = {
	{{ -0.5f, 0.5f }, { 255,000,000,255 }},		//0
	{{  0.5f, 0.5f }, { 255,255,000,255 }},		//1
	{{ -0.5f,-0.5f }, { 255,255,000,255 }},		//2
	{{  0.5f,-0.5f }, { 255,000,000,255 }},		//3

	{{ 0.0f , 0.0f  }, { 255,255,000,255 }},	//4

	{{ 0.375f, 0.375f }, { 255,255,000,255 }},	//5
	{{ 0.000f, 0.375f  }, { 255,000,000,255 }}, //6

	{{-0.375f,-0.375f }, { 255,255,000,255 }},	//7
	{{ 0.000f,-0.375f  }, { 255,000,000,255 }}, //8
};

UINT16 ind[512] = {
	0, 1, 2,
	1, 3, 2,
	4, 6, 5,
	4, 8, 7
};
*/