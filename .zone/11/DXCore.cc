#include "DXCore.h"

// DEFAULT CONSTRUCTOR
DXCore::DXCore()
{
    factory = NULL;
    adapter = NULL;
    device = NULL;
    context = NULL;
    swapchain = NULL;
    backbuffer = NULL;
    renderTargetView = NULL;
    depthStencil = NULL;
    depthStencilView = NULL;
    rasterizerState = NULL;
}

// CLEAR SCREEN BUFFERS
void DXCore::ClearScreen(const float* backgroundColor)
{
    context->ClearRenderTargetView(renderTargetView, backgroundColor);
    context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

// CLEANUP
void DXCore::CleanUp()
{
    if (context) context->ClearState();

    SAFE_RELEASE(rasterizerState);
    SAFE_RELEASE(depthStencilView);
    SAFE_RELEASE(depthStencil);
    SAFE_RELEASE(renderTargetView);
    SAFE_RELEASE(backbuffer);
    SAFE_RELEASE(swapchain);
    SAFE_RELEASE(context);
    SAFE_RELEASE(device);
    SAFE_RELEASE(adapter);
    for (int i = 0; i < MAX_ADAPTERS; i++)
    {
        SAFE_RELEASE(adaptersList[i]);
    }
    SAFE_RELEASE(factory);
}

// DESTRUCTOR
DXCore::~DXCore()
{
    CleanUp();
}

bool DXCore::Init(HWND hwnd)
{
    // DXGI Factory
// MSDN NOTE: For Direct3D 11.0 and later, we recommend that apps always use IDXGIFactory1 and CreateDXGIFactory1 instead.
    if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory)))
    {
        MessageBoxA(NULL, "Failed to create DXGI factory", "Error", MB_ICONERROR);
        OutputDebugString("\n\t\tFailed to create DXGI factory\n");
    }

    // Adapters
    DXGI_ADAPTER_DESC adapterDescriptor[MAX_ADAPTERS];

    for (UINT i = 0; factory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND; i++)
    {
        adaptersList[i] = adapter;
        adapter->GetDesc(&adapterDescriptor[i]);

        OutputDebugString("\n");
        DebugOutput("Adapter: %i\n", i);
        DebugOutputW(L"Description: %s\n", adapterDescriptor[i].Description);
        DebugOutput("VendorId: %u\n", adapterDescriptor[i].VendorId);
        DebugOutput("DeviceId: %u\n", adapterDescriptor[i].DeviceId);
        DebugOutput("SubSysId: %u\n", adapterDescriptor[i].SubSysId);
        DebugOutput("Revision: %u\n", adapterDescriptor[i].Revision);
        DebugOutput("DedicatedVideoMemory: %zu\n", adapterDescriptor[i].DedicatedVideoMemory);
        DebugOutput("DedicatedSystemMemory: %zu\n", adapterDescriptor[i].DedicatedSystemMemory);
        DebugOutput("SharedSystemMemory: %zu\n", adapterDescriptor[i].SharedSystemMemory);
        DebugOutput("AdapterLuid.LowPart: %u\n", adapterDescriptor[i].AdapterLuid.LowPart);
        DebugOutput("AdapterLuid.HighPart: %i\n", adapterDescriptor[i].AdapterLuid.HighPart);
    }
    OutputDebugString("\n");

    //factory->Release();

    // Device
    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_0
    };

    D3D_FEATURE_LEVEL* featureLevelSuppored = NULL;

    if (FAILED(D3D11CreateDevice(adaptersList[0], D3D_DRIVER_TYPE_UNKNOWN, (HMODULE)NULL, D3D11_CREATE_DEVICE_DEBUG, featureLevels,
        ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &device, featureLevelSuppored, &context)))
    {
        MessageBoxA(NULL, "Failed to create device context", "Error", MB_ICONERROR);
        OutputDebugString("\n\t\tFailed to create device context\n");
    }

    // Swap Chain
    RECT canvas = {};
    GetClientRect(hwnd, &canvas);
    LONG canvasWidth = canvas.right - canvas.left;
    LONG canvasHeight = canvas.bottom - canvas.top;
    FLOAT aspect = (FLOAT)canvasWidth / canvasHeight;

    DXGI_RATIONAL rational = { // num per second (60 / 1)
        .Numerator = 165,
        .Denominator = 1
    };

    DXGI_MODE_DESC modeDescriptor = {
        .Width = (UINT)canvasWidth,
        .Height = (UINT)canvasHeight,
        .RefreshRate = rational,
        .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
        .ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
        .Scaling = DXGI_MODE_SCALING_UNSPECIFIED
    };

    DXGI_SAMPLE_DESC sampleDescriptor = {
        .Count = 1,
        .Quality = 0 // no anti-aliasing
    };

    DXGI_SWAP_CHAIN_DESC swapchainDescriptor = {
        .BufferDesc = modeDescriptor,
        .SampleDesc = sampleDescriptor,
        .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
        .BufferCount = 2,
        .OutputWindow = hwnd,
        .Windowed = TRUE,
        .SwapEffect = DXGI_SWAP_EFFECT_DISCARD,
        .Flags = NOT_DEFINED_YET
    };

    if (FAILED(factory->CreateSwapChain(device, &swapchainDescriptor, &swapchain)))
    {
        MessageBoxA(NULL, "Failed to create swap chain", "Error", MB_ICONERROR);
        OutputDebugString("\n\t\tFailed to create swap chain\n");
    }

    //factory->Release();

    // Render Target View (Framebuffer)
    swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backbuffer);

    if (FAILED(device->CreateRenderTargetView(backbuffer, (D3D11_RENDER_TARGET_VIEW_DESC*)NULL, &renderTargetView)))
    {
        MessageBoxA(NULL, "Failed to create render target view", "Error", MB_ICONERROR);
        OutputDebugString("\n\t\tailed to create render target view\n");
    }

    // Depth Stensil Buffer
    D3D11_TEXTURE2D_DESC depthBufferDescriptor = {
        .Width = (UINT)(canvasWidth),
        .Height = (UINT)(canvasHeight),
        .MipLevels = 1,
        .ArraySize = 1,
        .Format = DXGI_FORMAT_D24_UNORM_S8_UINT,
        .SampleDesc = {1, 0},
        .Usage = D3D11_USAGE_DEFAULT,
        .BindFlags = D3D11_BIND_DEPTH_STENCIL,
        .CPUAccessFlags = 0,
        .MiscFlags = 0
    };

    if (FAILED(device->CreateTexture2D(&depthBufferDescriptor, NULL, &depthStencil)))
    {
        MessageBoxA(NULL, "Failed to create depth stensil", "Error", MB_ICONERROR);
        OutputDebugString("\n\t\tFailed to create depth stensil\n");
    }

    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDescriptor = {};
    depthStencilViewDescriptor.Format = depthBufferDescriptor.Format;
    depthStencilViewDescriptor.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDescriptor.Texture2D.MipSlice = 0;


    if (FAILED(device->CreateDepthStencilView(depthStencil, &depthStencilViewDescriptor, &depthStencilView)))
    {
        MessageBoxA(NULL, "Failed to create depth stensil view", "Error", MB_ICONERROR);
        OutputDebugString("\n\t\tFailed to create depth stensil view\n");
    }

    context->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

    // Viewport
    D3D11_VIEWPORT viewport = {
        .TopLeftX = 0.0f,
        .TopLeftY = 0.0f,
        .Width = (FLOAT)(canvasWidth),
        .Height = (FLOAT)(canvasHeight),
        .MinDepth = 0.0f,
        .MaxDepth = 1.0f
    };

    context->RSSetViewports(1, &viewport);

    // Rasterizer
    D3D11_RASTERIZER_DESC rasterizerDescriptor = {
        .FillMode = D3D11_FILL_SOLID,
        .CullMode = D3D11_CULL_BACK,
        .FrontCounterClockwise = FALSE,
        .DepthBias = 0,
        .DepthBiasClamp = 0,
        .SlopeScaledDepthBias = 0,
        .DepthClipEnable = 0,
        .ScissorEnable = 0,
        .MultisampleEnable = 0,
        .AntialiasedLineEnable = 0
    };

    device->CreateRasterizerState(&rasterizerDescriptor, &rasterizerState);
    context->RSSetState(rasterizerState);

    return true;
}
