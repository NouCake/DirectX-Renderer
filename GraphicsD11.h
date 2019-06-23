#pragma once

#include "NouWin.h"

#include <d3d11.h>
#include <dxgi1_4.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "dxgi.lib")

class GraphicsD11
{
	friend class Bindable;

public:

	struct Vertex
	{
		float x;
		float y;
		float z;
	};

	struct Color {
		UINT8 r;
		UINT8 g;
		UINT8 b;
		UINT8 a;
	};

	struct VertexInput
	{
		Vertex vert;
		Color color;
	};

	struct ConstantBuffer
	{
		DirectX::XMMATRIX transform;
	};

	GraphicsD11(HWND hWnd);
	GraphicsD11(const GraphicsD11&) = delete;
	GraphicsD11& operator=(const GraphicsD11&) = delete;
	~GraphicsD11() = default;

	void OnFrameEnd();
	void ClearBuffer(float r, float g, float b, float a);

	void SetBuffers();
	void SetShaders(LPCWSTR pathVertex, LPCWSTR pathFragment);

	void DrawTriangle();
	void UpdateGeometry(ConstantBuffer cb);


private:
	HRESULT res;

	Microsoft::WRL::ComPtr <ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr <IDXGISwapChain> pSwap;
	Microsoft::WRL::ComPtr <ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr <ID3D11RenderTargetView> pTarget;

	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstBuffer;
	UINT primitiveCount = 0;

};

