#pragma once

#include "DXCore.h"

class DXTexture {

    ID3D11SamplerState* samplerState;
    ID3D11ShaderResourceView* shaderResourceView;

public:
    DXTexture(DXCore* dx, const wchar_t* filename, D3D11_FILTER filter);
    ~DXTexture();

    void Bind(DXCore* dx, UINT slot) const;
};
