#include <open_gl.h>

OpenGL::OpenGL(ScreenMode screenMode, float pixelScale, unsigned int width, unsigned int height, const char* title)
{
    this->pixelScale = pixelScale;
    this->title = title;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    switch (screenMode) {
        case WINDOWED: initWindowed(width, height); break;
        case FULLSCREEN: initFullscreen(width, height); break;
    }
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    initQuad();
    quadShader = new Shader("include/shaders/canvas.vert", "include/shaders/canvas.frag");
}

OpenGL::~OpenGL() { terminate(); }

void OpenGL::initWindowed(unsigned int width, unsigned int height)
{
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE); // frame on/off
    window = glfwCreateWindow(width * pixelScale, height * pixelScale, title, NULL, NULL);

    canvas = new Canvas(width, height);

    screenOffsetX = 0;
    screenOffsetY = 0;
}

void OpenGL::initFullscreen(unsigned int width = 0, unsigned int height = 0)
{
    GLFWmonitor* primaryMonitor =  glfwGetPrimaryMonitor();
    const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);
    window = glfwCreateWindow(videoMode->width, videoMode->height, title, primaryMonitor, NULL);

    width = (width) ? width : videoMode->width / pixelScale;
    height = (height) ? height : videoMode->height / pixelScale;
    canvas = new Canvas(width, height);

    screenOffsetX = (videoMode->width - width * pixelScale) * .5f;
    screenOffsetY = (videoMode->height - height * pixelScale) * .5f;

    for (int i = 0; i < 24; i += 4) {
        quadVertices[i + 0] *= (float)canvas->width / videoMode->width * pixelScale;
        quadVertices[i + 1] *= (float)canvas->height / videoMode->height * pixelScale;
    }
}

void OpenGL::initQuad()
{
    glGenVertexArrays(1, &quadVAO);
    glBindVertexArray(quadVAO);

    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, canvas->width, canvas->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, canvas->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

    glGenRenderbuffers(1, &renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, canvas->width, canvas->height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGL::update()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClearColor(.1f, .2f, .2f, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    quadShader->use();
    glBindVertexArray(quadVAO);
    glDisable(GL_DEPTH_TEST);

    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, canvas->width, canvas->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, canvas->pixels);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glfwSwapBuffers(window);
    glfwPollEvents();

    // misc
    currentTime = static_cast<float>(glfwGetTime());
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    double mX, mY;
    glfwGetCursorPos(window, &mX, &mY);
    mouseX = int((mX - screenOffsetX) / pixelScale);
    if (mouseX < 0) mouseX = 0;
    if (mouseX > canvas->width - 1) mouseX = canvas->width - 1;
    mouseY = int(canvas->height - (mY - screenOffsetY) / pixelScale);
    if (mouseY < 0) mouseY = 0;
    if (mouseY > canvas->height - 1) mouseY = canvas->height - 1;
}

bool OpenGL::shouldClose() { return glfwWindowShouldClose(window); }

void OpenGL::terminate()
{
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &quadVBO);
    glDeleteFramebuffers(1, &framebuffer);
    glDeleteTextures(1, &textureColorbuffer);
    glDeleteRenderbuffers(1, &renderbuffer);
    glfwTerminate();
}
