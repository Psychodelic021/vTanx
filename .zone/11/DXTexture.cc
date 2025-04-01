#include "DXTexture.h"

DXTexture::DXTexture(DXCore* dx, const wchar_t* filename, D3D11_FILTER filter)
{
    D3D11_SAMPLER_DESC samplerDescriptor = {
        .Filter = filter,
        .AddressU = D3D11_TEXTURE_ADDRESS_WRAP,
        .AddressV = D3D11_TEXTURE_ADDRESS_WRAP,
        .AddressW = D3D11_TEXTURE_ADDRESS_WRAP,
        .MipLODBias = 0,
        .MaxAnisotropy = 0,
        .ComparisonFunc = D3D11_COMPARISON_NEVER,
        .BorderColor = {},
        .MinLOD = 0,
        .MaxLOD = D3D11_FLOAT32_MAX
    };

    if (FAILED(dx->device->CreateSamplerState(&samplerDescriptor, &samplerState)))
    {
        MessageBoxA(NULL, "Failed to create sampler", "Error", MB_ICONERROR);
        DebugOutputW(L"\n\t\tFailed to create sampler: %s\n", filename);
    }

    if (FAILED(CreateWICTextureFromFileEx(dx->device, filename, 0, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE,
        0, 0, WIC_LOADER_DEFAULT, NULL, &shaderResourceView)))
    {
        MessageBoxA(NULL, "Failed to create texture", "Error", MB_ICONERROR);
        DebugOutputW(L"\n\t\tFailed to create texture: %s\n", filename);
    }
}

void DXTexture::Bind(DXCore* dx, UINT slot) const
{
    dx->context->PSSetShaderResources(slot, 1, &shaderResourceView);
    dx->context->PSSetSamplers(slot, 1, &samplerState);
}

DXTexture::~DXTexture()
{
    SAFE_RELEASE(samplerState);
    SAFE_RELEASE(shaderResourceView);
}