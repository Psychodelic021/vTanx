#pragma once

#include "DX12Common.h"
#include "DX12Debug.h"

struct CommandFrame {

    ID3D12CommandAllocator* CommandAllocator = null;
    UINT64                  FenceValue = 0;
    DWORD                   Error = 0;

    inline void Release()
    {
        RELEASE(CommandAllocator);
    }

    inline void Wait(HANDLE fence_event, ID3D12Fence1* fence)
    {
        // WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
        // This is code implemented as such for simplicity. The D3D12HelloFrameBuffering
        // sample illustrates how to use fences for efficient resource usage and to
        // maximize GPU utilization.
        assert(fence && fence_event);
        // If the current fence value is still less then "fenceValue"
        // then we know the GPU has not finishing executing the command lists
        // scine it has not reached "CommandQueue->Signal()" command
        if (fence->GetCompletedValue() < FenceValue)
        {
            // Wait for command list / allocator to become free
            // We have the fence create an event which is signaled one the current fence value equals "fenceValue"
            DXCALL(fence->SetEventOnCompletion(FenceValue, fence_event));
            // Wait until the fence has triggered the event that its current value has reached "fenceValue"
            // indicating that command queue has finished executing
            if (WaitForSingleObject(fence_event, INFINITE) == WAIT_FAILED) Error = GetLastError();
        }
    }
};

struct DX12Command {

    ID3D12CommandQueue*             CommandQueue = null;
    ID3D12GraphicsCommandList7*     CommandList = null;
    ID3D12Fence1*                   Fence = null;
    CommandFrame                    CommandFrames[FRAME_BUFFER_COUNT] { };
    UINT                            FrameIndex = 0;
    UINT64                          FenceValue = 0;
    HANDLE                          FenceEvent = null;

    // Constructors
    DX12Command() = default;
    explicit DX12Command(ID3D12Device8* const device, D3D12_COMMAND_LIST_TYPE type);

    // Methods
    void Render();
    void BeginFrame();
    void EndFrame();
    void Flush();
    void Release();

    // Getters
    constexpr ID3D12CommandQueue* const GetCommandQueue() const { return CommandQueue; }
    constexpr ID3D12GraphicsCommandList7* const GetCommandList() const { return CommandList; }
    constexpr UINT GetFrameIndex() const { return FrameIndex; }

};