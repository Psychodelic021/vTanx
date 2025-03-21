
// Unity build includes
#include "win32.cpp"
#include "opengl.cpp"
#include "vmatx.cpp"

// Entry point
int main() {

    printf("\nApplication started...\n");
    
    WindowState window = {0};
    WindowInit(&window, 1600, 1000, "vTanx");

    InputState input = {0};

    OpenGLInit(&window, 4, 6);

    // DATA INITIALIZE
    vec3 vertices[] = {
        {.5f, -.5f, 0.f},
        {0.f, .5f, 0.f},
        {-.5f, -.5f, 0.f}
    };

    uint32 vertexarray = 0;
    glGenVertexArrays(1, &vertexarray);
    glBindVertexArray(vertexarray);

    uint32 vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    Shader basic_shader("shaders/basic_shader.vert", "shaders/basic_shader.frag");
    basic_shader.use();

    // MAIN LOOP
    while (window.is_running) {
        WindowUpdate(&window, &input);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        SwapBuffers(window.devcon);

        Sleep(1);
    }

    return 0;
}