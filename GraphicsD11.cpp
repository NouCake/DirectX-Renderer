#include "GraphicsD11.h"
#include "NouWindow.h"

#define CHECK_HR_EXCEPT() if(FAILED(res)) throw NouWindow::Exception( __LINE__, __FILE__, res);

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

void GraphicsD11::DrawTriangle()
{

	struct Vertex
	{
		float x;
		float y;
	};

	const Vertex verts[] = {
		{ -.5f, 0.5f},
		{ 0.5f, 0.5f},
		{ -.5f, -.5f},
		{ 0.5f, -.5f},
		{ -1, -1},
		{ 1, -1}
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(verts);
	bd.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = verts;

	res = pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer);
	CHECK_HR_EXCEPT();

	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;

	pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);


	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVS;
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;

	//LOAD PIXEL SHADER
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pFS;
	res = D3DReadFileToBlob(L"frag.cso", &pBlob);
	CHECK_HR_EXCEPT();

	res = pDevice->CreatePixelShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		&pFS);
	CHECK_HR_EXCEPT();

	//LOAD VERTEX SHADER
	res = D3DReadFileToBlob(L"vertex.cso", &pBlob);
	CHECK_HR_EXCEPT();

	res = pDevice->CreateVertexShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		&pVS);
	CHECK_HR_EXCEPT();

	//CREATE INPUT LAYOUT OF VERTEX & BIND
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
	D3D11_INPUT_ELEMENT_DESC ied = {};
	ied.SemanticName = "Position";
	ied.SemanticIndex = 0;
	ied.Format = DXGI_FORMAT_R32G32_FLOAT;
	ied.InputSlot = 0;
	ied.AlignedByteOffset = 0;
	ied.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	ied.InstanceDataStepRate = 0;

	res = pDevice->CreateInputLayout(
		&ied,
		1,
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInputLayout
	);
	CHECK_HR_EXCEPT();

	pContext->IASetInputLayout(pInputLayout.Get());


	//BIND SHADER
	pContext->VSSetShader(pVS.Get(), nullptr, 0);
	pContext->PSSetShader(pFS.Get(), nullptr, 0);

	//SET RENDER TARGET
	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


	//SET VIEWPORT
	D3D11_VIEWPORT vp = {};
	vp.Width = 640;
	vp.Height = 480;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pContext->RSSetViewports(1u, &vp);

	pContext->Draw((UINT)std::size(verts), 0u);

}