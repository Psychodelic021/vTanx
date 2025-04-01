#pragma once

// Windows includes
#include <wrl.h>

// DirectX includes
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dx12.h>
#include <d3d12sdklayers.h>
#include <dxgidebug.h>
#include <d3dcompiler.h>

// Libraries include
#include <vmath.h>

// Local includes
#include "../window.h"

using Microsoft::WRL::ComPtr;

#ifndef RELEASE
#define RELEASE(p) if (p) (p)->Release()
#endif // RELEASE

#define VS_VERSION "vs_5_1"
#define PS_VERSION "ps_5_1"

#ifdef _DEBUG
#define DXCALL(x) {                             \
        if (FAILED(x)) {                            \
            DebugOutput("ERROR: %s\n", __FILE__);   \
            DebugOutput("LINE: %i: ", __LINE__);    \
            OutputDebugString(#x);                  \
            OutputDebugString("\n");                \
        }                                           \
    }
#else
#define DXCALL(x) x
#endif // _DEBUG

#ifdef _DEBUG
#define ERROR_PTR_MSG(x) if (x) { DebugOutput("Error message: %s\n", (char*)x->GetBufferPointer()); }
#else
#define ERROR_PTR_MSG(x)
#endif // _DEBUG

#ifdef _DEBUG
#define NAME_D3D12_OBJECT(obj, name) { obj->SetName(name); DebugOutputW(L"::D3D12 Object Created: %s\n", name); }
#define NAME_D3D12_OBJECT_INDEXED(obj, idx, name) { obj->SetName(name); DebugOutputW(L"::D3D12 Object Created: %s[%i]\n", name, idx); }
#else
#define NAME_D3D12_OBJECT(obj, name)
#endif // _DEBUG

constexpr UINT FRAME_BUFFER_COUNT { 3 };