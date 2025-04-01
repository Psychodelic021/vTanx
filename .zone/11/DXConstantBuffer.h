#pragma once

#include "DXCore.h"

class DXConstantBuffer
{
    ID3D11Buffer* CBO = NULL;

public:

    DXConstantBuffer(DXCore* dx, UINT size);
    ~DXConstantBuffer();

    void Update(DXCore* dx, const void* pSrcData);
    void SetVS(DXCore* dx, UINT slot) const;
    void SetPS(DXCore* dx, UINT slot) const;
};

