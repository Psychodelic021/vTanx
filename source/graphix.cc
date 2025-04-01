#include <graphix.h>

void Graphix::Init(HWND hwnd)
{   
    std::cout << "Graphix: Initializing..." << std::endl;
    // DXGI Factory
    IDXGIFactory1* factory = NULL;
    HR(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&factory), "Failed to create DXGI factory");

    // DXGI Adapter
    IDXGIAdapter* adapter = NULL;
    DXGI_ADAPTER_DESC adapter_info;
    factory->EnumAdapters(0, &adapter);
    adapter->GetDesc(&adapter_info);

    std::wcout << "GPU: " << adapter_info.Description << "\n";
    std::cout << "VideoMemory: " << adapter_info.DedicatedVideoMemory / 1024 / 1024 << " MB" << std::endl;

    std::cout << "Graphix: OK" << std::endl;
}

void Graphix::Destroy()
{
    if (RenderTargetView) RELEASE(RenderTargetView);
    if (DepthStencilView) RELEASE(DepthStencilView);
    if (DepthStencilBuffer) RELEASE(DepthStencilBuffer);
    if (SwapChain) RELEASE(SwapChain);
    if (Context) { Context->ClearState(); RELEASE(Context); }
    if (Device) RELEASE(Device);

    std::cout << "Graphix: Shutting down..." << std::endl;
}

Graphix::~Graphix()
{
    Destroy();
}