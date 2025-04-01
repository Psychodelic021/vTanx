#pragma once

#include "DXCore.h"

class DXPixelShader
{
    ID3D11PixelShader* shader = NULL;

public:
    DXPixelShader(DXCore* dx, LPCWSTR source, LPCSTR entry, LPCSTR target);
    ~DXPixelShader();

    void Bind(DXCore* dx);
};

