#include "DX12Command.h"

DX12Command::DX12Command(ID3D12Device8* const device, D3D12_COMMAND_LIST_TYPE type)
{
    HRESULT hr = S_OK;
    // Describe command queue
    D3D12_COMMAND_QUEUE_DESC command_queue_descriptor = { // we will be using all the default values
        .Type = type,
        .Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,
        .Flags = D3D12_COMMAND_QUEUE_FLAG_NONE,
        .NodeMask = 0
    };

    // Create command queue
    DXCALL(hr = device->CreateCommandQueue(&command_queue_descriptor, IID_PPV_ARGS(&CommandQueue)));
    if (FAILED(hr)) goto _error;
    NAME_D3D12_OBJECT(CommandQueue, L"Command Queue");

    for (UINT i = 0; i < FRAME_BUFFER_COUNT; i++)
    {
        DXCALL(hr = device->CreateCommandAllocator(type, IID_PPV_ARGS(&CommandFrames[i].CommandAllocator)));
        if (FAILED(hr)) goto _error;
        NAME_D3D12_OBJECT_INDEXED(CommandFrames[i].CommandAllocator, i, L"CommandAllocator");
    }

    // Create command list
    DXCALL(hr = device->CreateCommandList(0, type, CommandFrames[0].CommandAllocator, null, IID_PPV_ARGS(&CommandList)));
    if (FAILED(hr)) goto _error;
    NAME_D3D12_OBJECT(CommandList, L"Command List");

    // Close command list's recording state
    DXCALL(hr = CommandList->Close());
    if (FAILED(hr)) goto _error;

    DXCALL(hr = device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fence)));
    if (FAILED(hr)) goto _error;
    NAME_D3D12_OBJECT(Fence, L"Fence");

    FenceEvent = CreateEventEx(null, null, 0, EVENT_ALL_ACCESS);
    assert(FenceEvent);

    return;

_error:
    Release();
}

// Wait for the current Frame to be signaled and reset command list/allocator
void DX12Command::BeginFrame()
{
    CommandFrames[FrameIndex].Wait(FenceEvent, Fence);
    DXCALL(CommandFrames[FrameIndex].CommandAllocator->Reset());
    DXCALL(CommandList->Reset(CommandFrames[FrameIndex].CommandAllocator, null));
}

// Signal the Fence with the new Fence value
void DX12Command::EndFrame()
{
    DXCALL(CommandList->Close());
    ID3D12CommandList* const command_lists[] { CommandList };
    CommandQueue->ExecuteCommandLists(_countof(command_lists), &command_lists[0]);
    FenceValue++;
    CommandFrames[FrameIndex].FenceValue = FenceValue;
    CommandQueue->Signal(Fence, FenceValue);
    FrameIndex = (FrameIndex + 1) % FRAME_BUFFER_COUNT;
}

void DX12Command::Render()
{
    BeginFrame();

    EndFrame();
}

void DX12Command::Flush()
{
    for (UINT i = 0; i < FRAME_BUFFER_COUNT; i++)
    {
        CommandFrames[i].Wait(FenceEvent, Fence);
    }
    FrameIndex = 0;
}

void DX12Command::Release()
{
    Flush();
    RELEASE(Fence);
    FenceValue = 0;

    CloseHandle(FenceEvent);
    FenceEvent = null;

    RELEASE(CommandQueue);
    RELEASE(CommandList);

    for (UINT i = 0; i < FRAME_BUFFER_COUNT; i++)
    {
        CommandFrames[i].Release();
    }
}
