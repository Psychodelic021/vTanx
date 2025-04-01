#include "DX12Debug.h"

void DX12Debug::InitD3D12DebugInterface(UINT* FactoryFlags)
{
    // Enable the debug layer (requires the Graphics Tools "optional feature").
    // NOTE: Enabling the debug layer after device creation will invalidate the active device.
    DXCALL(D3D12GetDebugInterface(IID_PPV_ARGS(&d3dDebug)));
    d3dDebug->EnableDebugLayer();

    DXCALL(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiDebug)));
    dxgiDebug->EnableLeakTrackingForThread();

    // Enable additional debug layers.
    *FactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
}

void DX12Debug::InitDXGIDebugInterface()
{    
    DXCALL(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiDebug)));
    dxgiDebug->EnableLeakTrackingForThread();
}

void DX12Debug::CreateDXGIInfoQueue()
{
    DXCALL(dxgiDebug->QueryInterface(IID_PPV_ARGS(&dxgiInfoQueue)));
    dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, TRUE);
    dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, TRUE);
    dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_WARNING, TRUE);
}

void DX12Debug::CreateD3D12InfoQueue(ID3D12Device8* device)
{
    DXCALL(device->QueryInterface(IID_PPV_ARGS(&d3dInfoQueue)));
    d3dInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
    d3dInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
    d3dInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE);
}

void DX12Debug::CreateD3D12DebugDevice(ID3D12Device8* device)
{
    DXCALL(device->QueryInterface(IID_PPV_ARGS(&d3dDebugDevice)));
}

void DX12Debug::ReportOnShutdown()
{
    dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL,
        DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_SUMMARY | DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_IGNORE_INTERNAL));

    d3dDebugDevice->ReportLiveDeviceObjects(D3D12_RLDO_SUMMARY | D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
}

void DX12Debug::Terminate()
{
    dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, FALSE);
    dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, FALSE);
    dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_WARNING, FALSE);

    d3dInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, FALSE);
    d3dInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, FALSE);
    d3dInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, FALSE);

    RELEASE(d3dDebug);
    RELEASE(d3dInfoQueue);
    RELEASE(dxgiInfoQueue);

    dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_ALL));
    RELEASE(dxgiDebug);
    d3dDebugDevice->ReportLiveDeviceObjects(D3D12_RLDO_SUMMARY | D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
    RELEASE(d3dDebugDevice);
}