#pragma once

#include "DXCore.h"

class DXVertexBuffer
{
    ID3D11Buffer* VBO = NULL;

public:
    DXVertexBuffer(DXCore* dx, const void* type, UINT size, UINT stride);
    ~DXVertexBuffer();
};

