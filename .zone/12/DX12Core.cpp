#pragma once

#include "DX12Core.h"

void DX12Core::Init() {

    if (Device) Terminate();

    UINT FactoryFlags { 0 };

    #ifdef _DEBUG
        Debug.InitD3D12DebugInterface(&FactoryFlags);
        Debug.InitDXGIDebugInterface();
        Debug.CreateDXGIInfoQueue();
    #endif // _DEBUG

    // Create DXGI factory
    DXCALL(CreateDXGIFactory2(FactoryFlags, IID_PPV_ARGS(&Factory)));

    // Enumerate adapters and create device
    for (UINT index = 0;
         Factory->EnumAdapterByGpuPreference(index, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&Adapter)) != DXGI_ERROR_NOT_FOUND;
         index++)
    {
        DXGI_ADAPTER_DESC1 adapterDesc;
        Adapter->GetDesc1(&adapterDesc);

        DebugOutputW(L"Adapter %i: %s\n", index, adapterDesc.Description);

        if (adapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue; // skip software adapter
        if (SUCCEEDED(D3D12CreateDevice(Adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&Device)))) break;
    }

    NAME_D3D12_OBJECT(Device, L"Main D3D12 device");

    #ifdef _DEBUG
        Debug.CreateD3D12InfoQueue(Device);
        Debug.CreateD3D12DebugDevice(Device);
    #endif // _DEBUG

    new (&Command) DX12Command(Device, D3D12_COMMAND_LIST_TYPE_DIRECT);
}

void DX12Core::Terminate()
{
    RELEASE(Adapter);
    RELEASE(Factory);
    Command.Release();
    RELEASE(Device);

    #ifdef _DEBUG
        Debug.Terminate();
    #endif // _DEBUG
}

void DX12Core::Render()
{
    // Wait for the GPU to finish with command allocator
    // and reset the allocator once the GPU is done with it.
    // This frees the memory that was used to store commands
    Command.BeginFrame();

    // Record commands
    // ...
    // Done recording commands. Now execute commands,
    // signal and incrementthe fence value for next frame
    Command.EndFrame();
}

std::vector<string> DX12Core::RetrieveDXGIDebugMessages(IDXGIInfoQueue* dxgiInfoQueue)
{
    std::vector<string> messages;
    for (UINT64 i = 0; i < dxgiInfoQueue->GetNumStoredMessagesAllowedByRetrievalFilters(DXGI_DEBUG_ALL); i++)
    {
        SIZE_T length = 0;
        dxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, null, &length);
        auto msg = (DXGI_INFO_QUEUE_MESSAGE*)malloc(sizeof(length));
        if (SUCCEEDED(dxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, msg, &length)) && msg != null)
        {
            messages.push_back(msg->pDescription);
        }
        free(msg);
    }
    return messages;
}
