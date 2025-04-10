#include <directx.hh>

void DirectX11::Init(HWND hwnd)
{   
    //std::cout << "DirectX 11: Initializing..." << std::endl;
    
    // DXGI Factory
    IDXGIFactory1* Factory = NULL;
    DXCALL(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&Factory), "Failed to create DXGI factory");

    // DXGI Adapter
    IDXGIAdapter* TestAdapter = NULL;
    IDXGIAdapter* Adapter = NULL;
    DXGI_ADAPTER_DESC adapterInfo = {0};
    size_t maxVideoMemory = 0;

    for (uint32 i = 0; Factory->EnumAdapters(i, &TestAdapter) != DXGI_ERROR_NOT_FOUND; i++)
    {
        TestAdapter->GetDesc(&adapterInfo);
        if (adapterInfo.DedicatedVideoMemory > maxVideoMemory)
        {
            maxVideoMemory = adapterInfo.DedicatedVideoMemory;
            Adapter = TestAdapter;
        }
    }

    COM_RELEASE(TestAdapter);

    Adapter->GetDesc(&adapterInfo);
    std::wcout << "GPU: \t" << adapterInfo.Description << "\n";
    std::cout << "VRAM: \t" << adapterInfo.DedicatedVideoMemory / 1024 / 1024 << " MB" << std::endl;

    // D3D11 Device
    UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
    flags |= D3D11_CREATE_DEVICE_DEBUG; // Enable debug layer in debug mode
#endif

    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_1
    };

    DXCALL(D3D11CreateDevice(Adapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, flags, featureLevels,
        std::size(featureLevels), D3D11_SDK_VERSION, &Device, NULL, &Context), "Failed to create D3D11 device");

    COM_RELEASE(Adapter);

    // D3D11 Debug Layer
#ifdef _DEBUG
    DXCALL(Device->QueryInterface(IID_PPV_ARGS(&DX11Debug)), "Failed to create D3D11 debug layer");
    DXCALL(DX11Debug->QueryInterface(IID_PPV_ARGS(&DX11InfoQueue)), "Failed to create D3D11 info queue");
    typedef HRESULT (WINAPI* PFN_DXGIGetDebugInterface)(REFIID, void**);
    HMODULE hmodDXGIDebug = LoadLibraryA("Dxgidebug.dll");
    if (hmodDXGIDebug == NULL) {
        std::cerr << "Failed to load Dxgidebug.dll" << std::endl;
        throw std::exception();
    }
    const auto DXGIGetDebugInterface = (PFN_DXGIGetDebugInterface)GetProcAddress(hmodDXGIDebug,"DXGIGetDebugInterface");
    if (DXGIGetDebugInterface == NULL) {
        std::cerr << "Failed to get DXGI debug interface" << std::endl;
        throw std::exception();
    }
    DXCALL(DXGIGetDebugInterface(IID_PPV_ARGS(&DXGIDebug)), "Failed to create DXGI debug layer");
    DXCALL(DXGIDebug->QueryInterface(IID_PPV_ARGS(&DXGIInfoQueue)), "Failed to create DXGI info queue");
#endif

    // DXGI Swap Chain

    std::cout << "DirectX: ON" << std::endl;
}

void DirectX11::Destroy()
{
    if (RenderTargetView) COM_RELEASE(RenderTargetView);
    if (DepthStencilView) COM_RELEASE(DepthStencilView);
    if (DepthStencilBuffer) COM_RELEASE(DepthStencilBuffer);
    if (SwapChain) COM_RELEASE(SwapChain);
    if (Context) { Context->ClearState(); COM_RELEASE(Context); }
    if (Device) COM_RELEASE(Device);

    std::cout << "DirectX: OFF" << std::endl;
}