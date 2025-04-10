#pragma once

#include <common.h>
#include <system.h>

#include <d3d11.h>
#include <dxgi1_3.h>
#include <dxgidebug.h>
#include <d3dcompiler.h>
#include <d3d11sdklayers.h>
#include <comdef.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")

// DXCALL ERROR (54): "file/path/to/error.h" 
// Failed to create DXGI factory - DXGI_ERROR_NOT_FOUND 

#ifdef _DEBUG
#define DXCALL(x, message) { \
        HRESULT hr = x; \
        if (FAILED(hr)) { \
            _com_error error(hr); \
            printf("[DXCALL ERROR] (line %d): %s\n%s", __LINE__, __FILE__, message); \
            wprintf(L" >> %hs", error.ErrorMessage()); \
            throw std::exception(); \
        } \
    }
#else
#define DXCALL(x, message) x
#endif // _DEBUG

#define COM_RELEASE(x) { if (x) { x->Release(); x = NULL; } }

struct DXSettings {
    
};

class DirectX11 {

    private:
        // Debug layer interfaces
        ID3D11Debug* DX11Debug = NULL;
        ID3D11InfoQueue* DX11InfoQueue = NULL;
        IDXGIDebug* DXGIDebug = NULL;
        IDXGIInfoQueue* DXGIInfoQueue = NULL;
        
        // DirectX interfaces
        ID3D11Device* Device = NULL;
        ID3D11DeviceContext* Context = NULL;
        IDXGISwapChain1* SwapChain = NULL;
        ID3D11RenderTargetView* RenderTargetView = NULL;
        ID3D11Texture2D* DepthStencilBuffer = NULL;
        ID3D11DepthStencilView* DepthStencilView = NULL;

    public:
        void Init(HWND hwnd);
        void Destroy();
};
