#include "GraphicsD11.h"
#include "NouWindow.h"

#define CHECK_HR_EXCEPT() if(FAILED(res)) throw NouWindow::Exception( __LINE__, __FILE__, res);

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



	SetBuffers();
	SetShaders(L"vertex.cso", L"frag.cso");
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
		throw NouWindow::Exception(__LINE__, __FILE__, res);
	}
}

void GraphicsD11::ClearBuffer(float r, float g, float b, float a)
{

	const float color[] = { r,g,b,1.0f };
	pContext->ClearRenderTargetView(pTarget.Get(), color);
}

void GraphicsD11::SetBuffers()
{
	//Generating Data
	VertexInput vi[1024] = {
		{{ -0.5f, 0.5f }, { 255,000,000,255 }},
		{{  0.5f, 0.5f }, { 255,255,000,255 }},
		{{ -0.5f,-0.5f }, { 000,255,255,255 }},
		{{  0.5f,-0.5f }, { 000,000,255,255 }},
	};

	UINT16 ind[1024] = {
		0, 1, 2, 3
	};

	//Creating Vertex Buffer
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(VertexInput) * 1024;
	bd.StructureByteStride = sizeof(VertexInput);

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vi;

	res = pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer);
	CHECK_HR_EXCEPT();

	//Binding VertexBuffer
	const UINT stride = sizeof(VertexInput);
	const UINT offset = 0u;
	pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

	bd = {};
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(UINT16) * 1024;
	bd.StructureByteStride = sizeof(UINT16);

	sd = {};
	sd.pSysMem = ind;

	res = pDevice->CreateBuffer(&bd, &sd, &pIndexBuffer);
	CHECK_HR_EXCEPT();

	pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	const ConstantBuffer cb = {
		dx::XMMatrixIdentity()
	};


	bd = {};
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(ConstantBuffer) * 1;
	bd.StructureByteStride = sizeof(ConstantBuffer);

	sd = {};
	sd.pSysMem = &cb;

	res = pDevice->CreateBuffer(&bd, &sd, &pConstBuffer);
	CHECK_HR_EXCEPT();

	pContext->VSSetConstantBuffers(0, 1, pConstBuffer.GetAddressOf());
}

void GraphicsD11::UpdateGeometry(ConstantBuffer cb)
{

	D3D11_MAPPED_SUBRESOURCE sr;
	res = pContext->Map(
		pConstBuffer.Get(),
		0u,
		D3D11_MAP_WRITE_DISCARD,
		0u, 
		&sr
		);
	memcpy(sr.pData, &cb, sizeof(ConstantBuffer));
	pContext->Unmap(pConstBuffer.Get(), 0u);

}

void GraphicsD11::SetShaders(LPCWSTR pathVertex, LPCWSTR pathFragment)
{
	//Creating PixelShader
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	res = D3DReadFileToBlob(pathFragment, &pBlob);
	CHECK_HR_EXCEPT();
	
	res = pDevice->CreatePixelShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		&pFS);
	CHECK_HR_EXCEPT();

	//Creating Vertex Shader
	res = D3DReadFileToBlob(L"vertex.cso", &pBlob);
	CHECK_HR_EXCEPT();

	res = pDevice->CreateVertexShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		&pVS);
	CHECK_HR_EXCEPT();

	//
	D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, sizeof(Vertex), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
	res = pDevice->CreateInputLayout(
		ied,
		2,
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInputLayout
	);
	CHECK_HR_EXCEPT();

	pContext->IASetInputLayout(pInputLayout.Get());


	//BIND SHADER
	pContext->VSSetShader(pVS.Get(), nullptr, 0);
	pContext->PSSetShader(pFS.Get(), nullptr, 0);
}

void GraphicsD11::DrawTriangle()
{

	pContext->DrawIndexed(4, 0, 0);

}