// COMPILER OPTIONS
#pragma once

#pragma warning(disable : 6387)

// INCLUDES
#include <windows.h>
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>

#include "DebugLayer.h"

// DEFINES
#define MAX_ADAPTERS 4

using namespace DirectX;
using Microsoft::WRL::ComPtr;

// D3D11 API CLASS TYPE
class DXCore {
    
private:
    ComPtr<IDXGIFactory1> factory;
    ComPtr<IDXGIAdapter> adapter;
    ComPtr<IDXGIAdapter> adaptersList[MAX_ADAPTERS] = { NULL };
    ComPtr<ID3D11Texture2D> backbuffer;
    ComPtr<ID3D11RenderTargetView> renderTargetView;
    ComPtr<ID3D11Texture2D> depthStencil;
    ComPtr<ID3D11DepthStencilView> depthStencilView;
    ComPtr<ID3D11RasterizerState> rasterizerState;

public:
    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> context;
    ComPtr<IDXGISwapChain> swapchain;

    DXCore();
    ~DXCore();

    bool Init(HWND hwnd);

    void ClearScreen(const float* backgroundColor);
    void CleanUp();
};
