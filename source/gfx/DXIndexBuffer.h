#pragma once

#include "DXCore.h"

class DXIndexBuffer
{
    ID3D11Buffer* IBO = NULL;

public:

    DXIndexBuffer(DXCore* dx, const void* type, UINT size);
    ~DXIndexBuffer();
};

