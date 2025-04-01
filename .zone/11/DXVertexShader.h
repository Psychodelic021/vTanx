#pragma once

#include "DXCore.h"

class DXVertexShader
{

    ID3D11InputLayout* layout = NULL;
    ID3D11VertexShader* shader = NULL;

public:
    DXVertexShader(DXCore* dx, LPCWSTR source, LPCSTR entry, LPCSTR target);
    ~DXVertexShader();

    void Bind(DXCore* dx);
};

