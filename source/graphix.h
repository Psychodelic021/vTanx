#pragma once

#include <common.h>
#include <system.h>

#include <d3d11.h>
#include <dxgi1_3.h>
#include <dxgidebug.h>
#include <d3dcompiler.h>
#include <comdef.h>

#ifdef _DEBUG
#define HR(x, error) { \
        if (FAILED(x)) { \
            std::printf("Error (%i) in %s\n", __LINE__, __FILE__); \
            throw std::runtime_error(error); \
        } \
    }
#else
#define HR(x) x
#endif // _DEBUG

#define RELEASE(x) { if (x) { x->Release(); x = NULL; } }

struct GraphixSettings {
    
};

class Graphix {

    private:
        // DirectX variables
        ID3D11Device* Device = NULL;
        ID3D11DeviceContext* Context = NULL;
        IDXGISwapChain1* SwapChain = NULL;
        ID3D11RenderTargetView* RenderTargetView = NULL;
        ID3D11Texture2D* DepthStencilBuffer = NULL;
        ID3D11DepthStencilView* DepthStencilView = NULL;

    public:
        // Graphics initialization
        Graphix() = default;
        Graphix(const Graphix&) = delete;
        Graphix operator= (const Graphix&) = delete;
        ~Graphix();

        void Init(HWND hwnd);
        void Destroy();
};
