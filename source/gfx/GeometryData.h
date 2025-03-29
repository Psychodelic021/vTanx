#pragma once

#include <DirectXMath.h>

struct VertexPC {
    XMFLOAT3 position;
    XMFLOAT4 color;
};

struct VertexPN {
    XMFLOAT3 position;
    XMFLOAT3 normal;
};

struct VertexPNT {
    XMFLOAT3 position;
    XMFLOAT3 normal;
    XMFLOAT2 texture;
};

// Vertex Data

struct Color {
    XMFLOAT4 white = { 1.f, 1.f, 1.f, 1.f };
    XMFLOAT4 black = { 0.f, 0.f, 0.f, 1.f };
    XMFLOAT4 red = { 1.f, 0.f, 0.f, 1.f };
    XMFLOAT4 green = { 0.f, 1.f, 0.f, 1.f };
    XMFLOAT4 blue = { 0.f, 0.f, 1.f, 1.f };
    XMFLOAT4 purple = { 1.f, 0.f, 1.f, 1.f };
    XMFLOAT4 yellow = { 1.f, 1.f, 0.f, 1.f };
    XMFLOAT4 cyan = { 0.f, 1.f, 1.f, 1.f };
    XMFLOAT4 royalblue = { 0.254901975f, 0.411764741f, 0.882353008f, 1.f };
} color;

VertexPC triangle[3] = {

    {XMFLOAT3(0.5f, -0.5f, 0.0f), color.white},
    {XMFLOAT3(-0.5f, -0.5f, 0.0f), color.green},
    {XMFLOAT3(0.0f,  0.5f, 0.0f), color.blue},
};

VertexPC pyramid[5] = {

    {XMFLOAT3(0.0f, 1.5f, 0.0f), color.yellow},  // 0 - top
    {XMFLOAT3(-1.0f, 0.0f, -1.0f), color.red}, // 1 - left near
    {XMFLOAT3(1.0f,  0.0f, -1.0f), color.red},   // 2 - right near
    {XMFLOAT3(-1.0f, 0.0f, 1.0f), color.red}, // 3 - left far
    {XMFLOAT3(1.0f,  0.0f, 1.0f), color.red},   // 4 - right far
};

VertexPNT cube[] = {
    // Vertices                       // Normals                    // TexCoords
    { XMFLOAT3(-1.0f, 1.0f, -1.0f),   XMFLOAT3(0.0f, 1.0f, 0.0f),   XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(1.0f, 1.0f, -1.0f),   XMFLOAT3(0.0f, 1.0f, 0.0f),   XMFLOAT2(1.0f, 0.0f) },
    { XMFLOAT3(1.0f, 1.0f,  1.0f),   XMFLOAT3(0.0f, 1.0f, 0.0f),   XMFLOAT2(1.0f, 1.0f) },
    { XMFLOAT3(-1.0f, 1.0f,  1.0f),   XMFLOAT3(0.0f, 1.0f, 0.0f),   XMFLOAT2(0.0f, 1.0f) },

    { XMFLOAT3(-1.0f, -1.0f, -1.0f),  XMFLOAT3(0.0f, -1.0f, 0.0f),  XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(1.0f, -1.0f, -1.0f),  XMFLOAT3(0.0f, -1.0f, 0.0f),  XMFLOAT2(1.0f, 0.0f) },
    { XMFLOAT3(1.0f, -1.0f,  1.0f),  XMFLOAT3(0.0f, -1.0f, 0.0f),  XMFLOAT2(1.0f, 1.0f) },
    { XMFLOAT3(-1.0f, -1.0f,  1.0f),  XMFLOAT3(0.0f, -1.0f, 0.0f),  XMFLOAT2(0.0f, 1.0f) },

    { XMFLOAT3(-1.0f, -1.0f,  1.0f),  XMFLOAT3(-1.0f, 0.0f, 0.0f),  XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(-1.0f, -1.0f, -1.0f),  XMFLOAT3(-1.0f, 0.0f, 0.0f),  XMFLOAT2(1.0f, 0.0f) },
    { XMFLOAT3(-1.0f,  1.0f, -1.0f),  XMFLOAT3(-1.0f, 0.0f, 0.0f),  XMFLOAT2(1.0f, 1.0f) },
    { XMFLOAT3(-1.0f,  1.0f,  1.0f),  XMFLOAT3(-1.0f, 0.0f, 0.0f),  XMFLOAT2(0.0f, 1.0f) },

    { XMFLOAT3(1.0f, -1.0f,  1.0f),   XMFLOAT3(1.0f, 0.0f, 0.0f),   XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(1.0f, -1.0f, -1.0f),   XMFLOAT3(1.0f, 0.0f, 0.0f),   XMFLOAT2(1.0f, 0.0f) },
    { XMFLOAT3(1.0f,  1.0f, -1.0f),   XMFLOAT3(1.0f, 0.0f, 0.0f),   XMFLOAT2(1.0f, 1.0f) },
    { XMFLOAT3(1.0f,  1.0f,  1.0f),   XMFLOAT3(1.0f, 0.0f, 0.0f),   XMFLOAT2(0.0f, 1.0f) },

    { XMFLOAT3(-1.0f, -1.0f, -1.0f),  XMFLOAT3(0.0f, 0.0f, -1.0f),  XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(1.0f, -1.0f, -1.0f),  XMFLOAT3(0.0f, 0.0f, -1.0f),  XMFLOAT2(1.0f, 0.0f) },
    { XMFLOAT3(1.0f,  1.0f, -1.0f),  XMFLOAT3(0.0f, 0.0f, -1.0f),  XMFLOAT2(1.0f, 1.0f) },
    { XMFLOAT3(-1.0f,  1.0f, -1.0f),  XMFLOAT3(0.0f, 0.0f, -1.0f),  XMFLOAT2(0.0f, 1.0f) },

    { XMFLOAT3(-1.0f, -1.0f, 1.0f),   XMFLOAT3(0.0f, 0.0f, 1.0f),   XMFLOAT2(0.0f, 0.0f) },
    { XMFLOAT3(1.0f, -1.0f, 1.0f),   XMFLOAT3(0.0f, 0.0f, 1.0f),   XMFLOAT2(1.0f, 0.0f) },
    { XMFLOAT3(1.0f,  1.0f, 1.0f),   XMFLOAT3(0.0f, 0.0f, 1.0f),   XMFLOAT2(1.0f, 1.0f) },
    { XMFLOAT3(-1.0f,  1.0f, 1.0f),   XMFLOAT3(0.0f, 0.0f, 1.0f),   XMFLOAT2(0.0f, 1.0f) },
};

UINT indicesTriangle[] = { 0, 1, 2 };

UINT indicesPyramid[] = {
    0,2,1,
    0,3,4,
    0,1,3,
    0,4,2,
    1,2,3,
    2,4,3,
};

UINT indicesCube[] = {
     3,  1,  0,       2,  1,  3,
     6,  4,  5,       7,  4,  6,
    11,  9,  8,      10,  9, 11,
    14, 12, 13,      15, 12, 14,
    19, 17, 16,      18, 17, 19,
    22, 20, 21,      23, 20, 22
};