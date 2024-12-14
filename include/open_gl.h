#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader.h>
#include <canvas.h>

enum ScreenMode {
    WINDOWED,
    FULLSCREEN
};

class OpenGL
{
    private:
        unsigned int quadVAO;
        unsigned int quadVBO;
        unsigned int framebuffer;
        unsigned int textureColorbuffer;
        unsigned int renderbuffer;
        Shader* quadShader;

        int screenOffsetX, screenOffsetY;

        float quadVertices[24] =  {
            // positions    // texCoords
            -1.0f,  1.0f,   0.0f, 1.0f,     // left top
            -1.0f, -1.0f,   0.0f, 0.0f,     // left bottom
             1.0f, -1.0f,   1.0f, 0.0f,     // right bottom

            -1.0f,  1.0f,   0.0f, 1.0f,     // left top
             1.0f, -1.0f,   1.0f, 0.0f,     // right bottom
             1.0f,  1.0f,   1.0f, 1.0f      // right top
        };

        float currentTime, lastTime;

        void initWindowed(unsigned int width, unsigned int height);
        void initFullscreen(unsigned int width, unsigned int height);
        void initQuad();
        void terminate();

    public:
        Canvas* canvas;
        float pixelScale;
        const char* title;
        GLFWwindow* window;

        float deltaTime;
        int mouseX, mouseY;

        OpenGL(ScreenMode screenMode, float pixelScale = 1, unsigned int width = 0, unsigned int height = 0, const char* title = "OpenGL - 2D window");
        ~OpenGL();

        void update();
        bool shouldClose();
};
