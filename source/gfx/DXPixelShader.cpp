#include "DXPixelShader.h"

// PIXEL SHADER
DXPixelShader::DXPixelShader(DXCore* dx, LPCWSTR source, LPCSTR entry, LPCSTR target)
{
    UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
    #if defined (DEBUG) || defined (_DEBUG)
        flags |= D3DCOMPILE_DEBUG;
    #endif

    ID3DBlob* shaderBlob = NULL;
    ID3DBlob* errorBlob = NULL;

    HRESULT result = D3DCompileFromFile(source, NULL, NULL, entry, target, flags, NULL, &shaderBlob, &errorBlob);

    if (FAILED(result))
    {
        MessageBoxA(NULL, "Failed to compile pixel shader", "Error", MB_ICONERROR);
        DebugOutputW(L"\n\t\tFailed to create pixel shader: %s\n", source);

        if (errorBlob)
        {
            OutputDebugStringA((char*)errorBlob->GetBufferPointer());
        }
    }

    result = dx->device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), 0, &shader);

    if (FAILED(result))
    {
        MessageBoxA(NULL, "Failed to create pixel shader", "Error", MB_ICONERROR);
        DebugOutputW(L"\n\t\tFailed to create pixel shader: %s\n", source);
    }

    SAFE_RELEASE(errorBlob);
    SAFE_RELEASE(shaderBlob);
}

DXPixelShader::~DXPixelShader()
{
    SAFE_RELEASE(shader);
}

void DXPixelShader::Bind(DXCore* dx)
{
    dx->context->PSSetShader(shader, NULL, 0);
}
