#include "DXVertexBuffer.h"

DXVertexBuffer::DXVertexBuffer(DXCore* dx, const void* data, UINT size, UINT stride)
{
    D3D11_BUFFER_DESC  VBDescriptor = {
          .ByteWidth = size,
          .Usage = D3D11_USAGE_DEFAULT,
          .BindFlags = D3D11_BIND_VERTEX_BUFFER,
          .CPUAccessFlags = 0,
          .MiscFlags = 0,
          .StructureByteStride = 0
    };

    D3D11_SUBRESOURCE_DATA VBSubResourceData = {
          .pSysMem = data,
          .SysMemPitch = 0,
          .SysMemSlicePitch = 0
    };

    if (FAILED(dx->device->CreateBuffer(&VBDescriptor, &VBSubResourceData, &VBO)))
    {
        MessageBoxA(NULL, "Failed to create vertex buffer", "Error", MB_ICONERROR);
        OutputDebugString("\n\t\tFailed to create vertex buffer\n");
    }

    UINT offset = 0;
    dx->context->IASetVertexBuffers(0, 1, &VBO, &stride, &offset);
}

DXVertexBuffer::~DXVertexBuffer()
{
    SAFE_RELEASE(VBO);
}
