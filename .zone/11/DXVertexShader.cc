#include "DXVertexShader.h"

DXVertexShader::DXVertexShader(DXCore* dx, LPCWSTR source, LPCSTR entry, LPCSTR target)
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
        MessageBoxA(NULL, "Failed to compile vertex shader", "Error", MB_ICONERROR);
        DebugOutputW(L"\n\t\tFailed to create input layout: %s\n", source);

        if (errorBlob)
        {
            OutputDebugStringA((char*)errorBlob->GetBufferPointer());
        }

    }

    result = dx->device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), 0, &shader);

    if (FAILED(result))
    {
        MessageBoxA(NULL, "Failed to create vertex shader", "Error", MB_ICONERROR);
        DebugOutputW(L"\n\t\tFailed to create vertex shader: %s\n", source);
    }

    // Vertex Layout

    D3D11_INPUT_ELEMENT_DESC layoutDescriptor[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    UINT numElements = ARRAYSIZE(layoutDescriptor);

    result = dx->device->CreateInputLayout(layoutDescriptor, numElements, shaderBlob->GetBufferPointer(),
        shaderBlob->GetBufferSize(), &layout);

    if (FAILED(result))
    {
        MessageBoxA(NULL, "Failed to create input layout", "Error", MB_ICONERROR);
        DebugOutput("\n\t\tFailed to create input layout\n");
    }

    dx->context->IASetInputLayout(layout);
    dx->context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    SAFE_RELEASE(shaderBlob);
    SAFE_RELEASE(errorBlob);
}

DXVertexShader::~DXVertexShader()
{
    SAFE_RELEASE(layout);
    SAFE_RELEASE(shader);
}

void DXVertexShader::Bind(DXCore* dx)
{
    dx->context->VSSetShader(shader, NULL, 0);
}
