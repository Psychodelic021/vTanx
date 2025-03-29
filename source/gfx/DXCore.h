// COMPILER OPTIONS
#pragma once

#pragma warning(disable : 6387)

// INCLUDES
#include <windows.h>
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <WICTextureLoader.h>
#include <DirectXMath.h>

#include "DebugLayer.h"
#include "Defines.h"

// DEFINES
#define MAX_ADAPTERS 4

using namespace DirectX;

// D3D11 API CLASS TYPE
class DXCore {
    
private:
    IDXGIFactory1* factory;
    IDXGIAdapter* adapter;
    IDXGIAdapter* adaptersList[MAX_ADAPTERS] = { NULL };
    ID3D11Texture2D* backbuffer;
    ID3D11RenderTargetView* renderTargetView;
    ID3D11Texture2D* depthStencil;
    ID3D11DepthStencilView* depthStencilView;
    ID3D11RasterizerState* rasterizerState;

public:
    ID3D11Device* device;
    ID3D11DeviceContext* context;
    IDXGISwapChain* swapchain;

    DXCore();
    ~DXCore();

    bool Init(HWND hwnd);

    void ClearScreen(const float* backgroundColor);
    void CleanUp();
};
