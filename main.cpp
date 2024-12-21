#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/freeglut.h>
#include <iostream>
#include "stb_image.h"  // Remove STB_IMAGE_IMPLEMENTATION from here

// Function to load texture using stb_image
GLuint LoadTexture(const char* filePath) {
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);
    if (!data) {
        std::cout << "Failed to load texture: " << stbi_failure_reason() << std::endl;
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, nrChannels == 4 ? GL_RGBA : GL_RGB,
        width, height, 0, nrChannels == 4 ? GL_RGBA : GL_RGB,
        GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    return textureID;
}

// Function to render the logo
void RenderLogo(GLuint textureID) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(-0.5f, 0.5f);
    glTexCoord2f(1, 0); glVertex2f(0.5f, 0.5f);
    glTexCoord2f(1, 1); glVertex2f(0.5f, -0.5f);
    glTexCoord2f(0, 1); glVertex2f(-0.5f, -0.5f);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

// Function to render text without using GLUT
void RenderText(const char* text, float x, float y) {
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(x, y);
    while (*text) {
        glBitmap(8, 13, 0, 0, 10, 0, basic_font[*text - 32]);
        ++text;
    }
}

// Function to render the "Play" button
void RenderButton(bool hovered) {
    if (hovered) {
        glColor3f(1.0f, 1.0f, 1.0f); // White border
    }
    else {
        glColor3f(0.5f, 0.5f, 0.5f); // Grey border
    }

    // Draw button border
    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.2f, -0.7f);
    glVertex2f(0.2f, -0.7f);
    glVertex2f(0.2f, -0.8f);
    glVertex2f(-0.2f, -0.8f);
    glEnd();

    // Render "Play" text
    RenderText("Play", -0.05f, -0.75f);
}

int main(int argc, char** argv) {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    // Create a GLFWwindow object
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Menu", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Load the logo texture
    GLuint logoTexture = LoadTexture("logo.png");
    if (!logoTexture) {
        std::cerr << "Failed to load texture" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // Render the logo
        RenderLogo(logoTexture);

        // Check if the mouse is hovering over the "Play" button
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        int windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        mouseX = (mouseX / windowWidth) * 2 - 1;
        mouseY = ((windowHeight - mouseY) / windowHeight) * 2 - 1;

        bool hovered = (mouseX >= -0.2 && mouseX <= 0.2 && mouseY >= -0.8 && mouseY <= -0.7);
        RenderButton(hovered);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    glDeleteTextures(1, &logoTexture);
    glfwTerminate();
    return 0;
}