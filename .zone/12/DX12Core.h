#pragma once

#include "DX12Common.h"
#include "DX12Debug.h"
#include "DX12Command.h"

struct DX12Core {

    IDXGIFactory7* Factory { null };
    IDXGIAdapter4* Adapter { null };
    ID3D12Device8* Device { null };

    DX12Debug Debug { };
    DX12Command Command { };

    // Constructors
    DX12Core() = default;

    // Methods
    void Init();
    void Render();
    std::vector<string> RetrieveDXGIDebugMessages(IDXGIInfoQueue* dxgiInfoQueue);
    void Terminate();
};
