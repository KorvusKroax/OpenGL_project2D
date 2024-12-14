#include <open_gl.h>

int main()
{
    // OpenGL openGL = OpenGL(WINDOWED, 3, 400, 250);

    OpenGL openGL = OpenGL(FULLSCREEN, 4);//, 320, 200);
    glfwSetInputMode(openGL.window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    glfwSetInputMode(openGL.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    // glfwSetInputMode(openGL.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // for no mouse pointer

    while (!openGL.shouldClose())
    {
        openGL.canvas->clearCanvas();

        // keys
        if (glfwGetKey(openGL.window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(openGL.window, true);
        }

        // mouse
        openGL.canvas->drawCircle(openGL.mouseX, openGL.mouseY, 3, Color(0, 255, 255));
        openGL.canvas->setPixel(openGL.mouseX, openGL.mouseY, Color(255, 255, 255));

        openGL.update();
    }

    return 0;
}
