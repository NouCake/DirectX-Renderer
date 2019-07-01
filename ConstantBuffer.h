#pragma once

#include "Bindable.h"
#include "NouException.h"

class ConstantBuffer : public Bindable
{
public:
	ConstantBuffer(GraphicsD11& gfx, UINT size, void* data)
	{
		OutputDebugString(("ConstantBuffer size: " + std::to_string(size) + "\n").c_str());
		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0u;
		bd.ByteWidth = size;
		bd.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = data;

		HRESULT res = GetDevice(gfx)->CreateBuffer(&bd, &sd, &pConstBuffer);
		CHECK_HR_EXCEPT();
	}
	void Update(GraphicsD11& gfx, UINT size, void* data)
	{
		HRESULT res;

		D3D11_MAPPED_SUBRESOURCE sr;
		res = GetContext(gfx)->Map(
			pConstBuffer.Get(),
			0u,
			D3D11_MAP_WRITE_DISCARD,
			0u,
			&sr
		);
		CHECK_HR_EXCEPT();

		memcpy(sr.pData, data, size);
		GetContext(gfx)->Unmap(pConstBuffer.Get(), 0u);
	}

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstBuffer;
};

class VertexConstantBuffer : public ConstantBuffer
{
public:
	using ConstantBuffer::ConstantBuffer;
	void Bind(GraphicsD11& gfx) noexcept override
	{
		this->GetContext(gfx)->VSSetConstantBuffers(0u, 1u, pConstBuffer.GetAddressOf());
	}
};

class FragmentConstantbuffer : public ConstantBuffer
{
public:
	using ConstantBuffer::ConstantBuffer;
	void Bind(GraphicsD11& gfx) noexcept override
	{
		this->GetContext(gfx)->PSSetConstantBuffers(0u, 1u, pConstBuffer.GetAddressOf());
	}
};