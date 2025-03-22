#pragma once

#include "opengl.h"

// OPENGL

void InitOpenGL(System::Window* wnd, int major, int minor)
{
    PIXELFORMATDESCRIPTOR pfd = {
        .nSize = sizeof(PIXELFORMATDESCRIPTOR),
        .nVersion = 1,
        .dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER,
        .iPixelType = PFD_TYPE_RGBA,
        .cColorBits = 32,
        .cDepthBits = 24,
        .cStencilBits = 8,
        .iLayerType = PFD_MAIN_PLANE,
    };

    int pixel_format = ChoosePixelFormat(wnd->devcon, &pfd);
    if (pixel_format == 0) {
        printf("OpenGL Error: Failed to choose pixel format\n");
        __debugbreak();
    }

    if (!SetPixelFormat(wnd->devcon, pixel_format, &pfd)) {
        printf("OpenGL Error: Failed to set pixel format\n");
        __debugbreak();
    }

    wnd->rndcon = wglCreateContext(wnd->devcon);
    if (!wnd->rndcon) {
        printf("OpenGL Error: Failed to create OpenGL context\n");
        __debugbreak();
    }

    if (!wglMakeCurrent(wnd->devcon, wnd->rndcon)) {
        printf("OpenGL Error: Failed to make OpenGL context current\n");
        __debugbreak();
    }

    int32 gl_status = gladLoadGL();
    int32 wgl_status = gladLoadWGL(wnd->devcon);
    if (gl_status == 0 && wgl_status == 0)
    {
        printf("OpenGL Error: Failed to initialize GLAD and WGL\n");
        printf("gladLoadGL status: %d\n", gl_status);
        printf("gladLoadWGL status: %d\n", wgl_status);
        __debugbreak();
    }

    printf("\nGraphics system:\n");
    printf("GPU:    %s\n", glGetString(GL_RENDERER));
    printf("Vendor: %s\n", glGetString(GL_VENDOR));
    printf("OpenGL: %s\n", glGetString(GL_VERSION));
    printf("\n");

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(wnd->rndcon);

    const int32 format_attrib_list[] = {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
        WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB, 32,
        WGL_DEPTH_BITS_ARB, 24,
        WGL_STENCIL_BITS_ARB, 8,
        0, // End of attributes list
    };

    pixel_format = 0;
    uint32 num_formats = 0;

    bool valid_pixel_format = wglChoosePixelFormatARB(wnd->devcon, format_attrib_list, NULL, 1, &pixel_format, &num_formats);
    if (!valid_pixel_format && !num_formats)
    {
        printf("OpenGL Error: Invalid pixel format\n");
        __debugbreak();
    }

    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    DescribePixelFormat(wnd->devcon, pixel_format, sizeof(pfd), &pfd);
    if (!SetPixelFormat(wnd->devcon, pixel_format, &pfd))
    {
        printf("OpenGL Error: Failed to set pixel format\n");
        __debugbreak();
    }

    const int32 context_attrib_list[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, major,
        WGL_CONTEXT_MINOR_VERSION_ARB, minor,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0, // End of attributes list
    };

    wnd->rndcon = wglCreateContextAttribsARB(wnd->devcon, NULL, context_attrib_list);
    if (!wnd->rndcon)
    {
        printf("OpenGL Error: Failed to create OpenGL context \n");
        __debugbreak();
    }

    wglMakeCurrent(wnd->devcon, wnd->rndcon);

    glViewport(0, 0, wnd->screen_width, wnd->screen_height);
}

// SHADERS

#define BUFFER_SIZE 1024

Shader::Shader(const char* vertex_filepath, const char* fragment_filepath)
{
    ID = 0;
    uint32 vertex = 0, fragment = 0;

    char* vertex_source = System::LoadTextFileToString(vertex_filepath);
    if (!vertex_source) {
        printf("Error: invalid vertex source!\n");
    }

    char* fragment_source = System::LoadTextFileToString(fragment_filepath);
    if (!fragment_source) {
        printf("Error: invalid fragment source!\n");
    }

    vertex = Compile(vertex_source, "VERTEX", vertex_filepath, GL_VERTEX_SHADER);
    fragment = Compile(fragment_source, "FRAGMENT", fragment_filepath, GL_FRAGMENT_SHADER);
    ID = Link(vertex, fragment);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    System::DeallocateString(vertex_source);
    System::DeallocateString(fragment_source);
}

uint32 Shader::Compile(const char* source, const char* name, const char* file, uint32 type)
{
    uint32 shader = 0;
    int status = 0;
    char info[BUFFER_SIZE] = {0};

    shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        glGetShaderInfoLog(shader, BUFFER_SIZE, NULL, info);
        printf("%s SHADER ERROR: COMPILATION FAILED! FILE: %s\n", name, file);
        printf("INFO: %s\n", info);
    }

    return shader;
}

uint32 Shader::Link(uint32 vertex, uint32 fragment)
{
    uint32 program = 0;
    int status = 0;
    char info[BUFFER_SIZE] = {0};

    program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    glValidateProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        glGetShaderInfoLog(program, BUFFER_SIZE, NULL, info);
        printf("SHADER PROGRAM ERROR: LINKING FAILED!\n");
        printf("INFO: %s\n", info);
    }

    glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
    if (status == GL_FALSE) {
        glGetProgramInfoLog(program, BUFFER_SIZE, NULL, info);
        printf("SHADER PROGRAM ERROR: VALIDATION FAILED!\n");
        printf("INFO: %s\n", info);
    }

    return program;
}


