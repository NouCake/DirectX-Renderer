#include "VertexBuffer.h"
#include "NouException.h"

VertexBuffer::VertexBuffer(GraphicsD11& gfx, UINT size, UINT stride, void* data)
	:
	stride(stride)
{
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = size;
	bd.StructureByteStride = stride;

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = data;

	HRESULT res;
	res = GetDevice(gfx)->CreateBuffer(&bd, &sd, &pVertexBuffer);
	CHECK_HR_EXCEPT();

}

void VertexBuffer::Bind(GraphicsD11& gfx) noexcept
{
	const UINT offset = 0u;
	GetContext(gfx)->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
}

