#include "DXConstantBuffer.h"

DXConstantBuffer::DXConstantBuffer(DXCore* dx, UINT size)
{
    D3D11_BUFFER_DESC CBDescriptor = {
        .ByteWidth = size,
        .Usage = D3D11_USAGE_DEFAULT,
        .BindFlags = D3D11_BIND_CONSTANT_BUFFER,
        .CPUAccessFlags = 0,
        .MiscFlags = 0,
        .StructureByteStride = 0
    };
    
    dx->device->CreateBuffer(&CBDescriptor, NULL, &CBO);
}

DXConstantBuffer::~DXConstantBuffer()
{
    SAFE_RELEASE(CBO);
}

void DXConstantBuffer::Update(DXCore* dx, const void* pSrcData)
{
    dx->context->UpdateSubresource(CBO, 0, NULL, pSrcData, 0, 0);
}

void DXConstantBuffer::SetVS(DXCore* dx, UINT slot) const
{
    dx->context->VSSetConstantBuffers(slot, 1, &CBO);
}

void DXConstantBuffer::SetPS(DXCore* dx, UINT slot) const
{
    dx->context->PSSetConstantBuffers(slot, 1, &CBO);
}
