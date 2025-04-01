#pragma once

#include "DX12Common.h"

struct DX12Debug {

    ID3D12Debug6* d3dDebug { null };
    ID3D12InfoQueue* d3dInfoQueue { null };
    IDXGIDebug1* dxgiDebug { null };
    IDXGIInfoQueue* dxgiInfoQueue { null };
    ID3D12DebugDevice2* d3dDebugDevice { null };

    DX12Debug() = default;

    void InitD3D12DebugInterface(UINT* FactoryFlags);
    void InitDXGIDebugInterface();
    void CreateDXGIInfoQueue();
    void CreateD3D12InfoQueue(ID3D12Device8* device);
    void CreateD3D12DebugDevice(ID3D12Device8* device);
    void ReportOnShutdown();
    void Terminate();

};

