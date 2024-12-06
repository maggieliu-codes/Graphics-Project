// SDLGraphicsProgram.hpp

#ifndef SDLGRAPHICSPROGRAM_HPP
#define SDLGRAPHICSPROGRAM_HPP

#if defined(LINUX) || defined(MINGW)
#include <SDL2/SDL.h>
#else // This works for Mac
#include <SDL.h>
#endif

#include <glad/glad.h>
#include "Camera.hpp"
#include <string>

class SDLGraphicsProgram
{
public:
    // Constructor
    SDLGraphicsProgram(int w, int h);
    // Destructor
    ~SDLGraphicsProgram();

    // Initialize objects based on the provided OBJ file path
    void InitObjects(const std::string &objFilePath);

    // loop that runs forever
    void Loop();

    // Get Pointer to Window
    SDL_Window *GetSDLWindow();

private:
    // Setup OpenGL
    bool InitGL();
    // Per frame update
    void Update();
    // Renders shapes to the screen
    void Render();
    // Helper Function to Query OpenGL information.
    void GetOpenGLVersionInfo();

    int m_screenWidth;
    int m_screenHeight;
    SDL_Window *m_window;
    SDL_GLContext m_openGLContext;
    bool m_wireframeMode = false;
    Camera m_camera;
};

#endif
