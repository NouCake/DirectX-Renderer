#include "GraphicsD11.h"
#include "NouWindow.h"

#include "BaseMaterial.h"
#include "Cube.h"

#ifdef USE_IMGUI
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_dx11.h"
#include "ImGUI/imgui_impl_win32.h"
#endif


#include <vector>
namespace dx = DirectX;

GraphicsD11::GraphicsD11(HWND hWnd, const unsigned int width, const unsigned int height)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0u;
	sd.BufferDesc.RefreshRate.Denominator = 0u;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1u;
	sd.SampleDesc.Quality = 0u;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1u;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0u;

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

	D3D11_DEPTH_STENCIL_DESC dsd = {};
	dsd.DepthEnable = TRUE;
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsd.DepthFunc = D3D11_COMPARISON_LESS;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDSState;
	res = pDevice->CreateDepthStencilState(&dsd, &pDSState);
	CHECK_HR_EXCEPT();
	
	pContext->OMSetDepthStencilState(pDSState.Get(), 1u);

	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	res = pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil);
	CHECK_HR_EXCEPT();

	// create view of depth stensil texture
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	descDSV.Texture2D.MipSlice = 0u;
	res = pDevice->CreateDepthStencilView(
		pDepthStencil.Get(), &descDSV, &pDSV
	);
	CHECK_HR_EXCEPT();



	// CREATE SAMPLER
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = std::numeric_limits<float>::max();
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

	pDevice->CreateSamplerState(&samplerDesc, &pSampler);
	pContext->PSSetSamplers(0, 1, pSampler.GetAddressOf());

	D3D11_VIEWPORT vp = {};
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pContext->RSSetViewports(1u, &vp);

	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pDSV.Get());

#ifdef USE_IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.IniFilename = "ImGUI/imgui.ini";
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(pDevice.Get(), pContext.Get());
#endif
}

void GraphicsD11::OnFrameEnd()
{
#ifdef USE_IMGUI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif
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
	pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void GraphicsD11::DrawIndexed(UINT count)
{
	pContext->DrawIndexed(count, 0u, 0u);
}