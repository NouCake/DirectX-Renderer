#include "IndexBuffer.h"
#include "NouException.h"

IndexBuffer::IndexBuffer(GraphicsD11& gfx, UINT count, void* data)
{
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(UINT16) * count;
	bd.StructureByteStride = sizeof(UINT16);

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = data;

	HRESULT res = GetDevice(gfx)->CreateBuffer(&bd, &sd, &pIndexBuffer);
	CHECK_HR_EXCEPT();
}

void IndexBuffer::Bind(GraphicsD11& gfx) noexcept
{
	GetContext(gfx)->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}

