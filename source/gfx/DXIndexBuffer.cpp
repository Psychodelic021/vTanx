#include "DXIndexBuffer.h"

DXIndexBuffer::DXIndexBuffer(DXCore* dx, const void* data, UINT size)
{
    D3D11_BUFFER_DESC IBDescriptor = {
        .ByteWidth = size,
        .Usage = D3D11_USAGE_DEFAULT,
        .BindFlags = D3D11_BIND_INDEX_BUFFER,
        .CPUAccessFlags = 0,
        .MiscFlags = 0,
        .StructureByteStride = 0
    };

    D3D11_SUBRESOURCE_DATA IBSubResourceData = {
        .pSysMem = data,
        .SysMemPitch = 0,
        .SysMemSlicePitch = 0
    };

    if (FAILED(dx->device->CreateBuffer(&IBDescriptor, &IBSubResourceData, &IBO)))
    {
        MessageBoxA(NULL, "Failed to create index buffer", "Error", MB_ICONERROR);
        OutputDebugString("\n\t\tFailed to create index buffer\n");
    }
    dx->context->IASetIndexBuffer(IBO, DXGI_FORMAT_R32_UINT, 0);
}

DXIndexBuffer::~DXIndexBuffer()
{
    SAFE_RELEASE(IBO);
}
