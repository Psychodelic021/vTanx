#pragma once

#include <common.h>
#include <system.h>

#include <glad\glad.h>
#include <glad\glad_wgl.h>

void InitOpenGL(System* system, int major, int minor);

struct Shader {

    uint32 ID;

    Shader(const char* vertex_filepath, const char* fragment_filepath);
    uint32 Compile(const char* source, const char* name, const char* file, uint32 type);
    uint32 Link(uint32 vertex, uint32 fragment);

    void use()
    {
        glUseProgram(ID);
    }
};