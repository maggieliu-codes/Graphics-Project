// SDLGraphicsProgram.cpp
#include "SDLGraphicsProgram.hpp"
#include "ObjectManager.hpp"
#include "Object.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

SDLGraphicsProgram::SDLGraphicsProgram(int w, int h)
    : m_screenWidth(w), m_screenHeight(h)
{
    // Initialization flag
    bool success = true;
    std::stringstream errorStream;
    m_window = NULL;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        errorStream << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";
        success = false;
    }
    else
    {
        // Use OpenGL 3.3 core
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        // Create window
        m_window = SDL_CreateWindow("Lab",
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    m_screenWidth,
                                    m_screenHeight,
                                    SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

        if (m_window == NULL)
        {
            errorStream << "Window could not be created! SDL Error: " << SDL_GetError() << "\n";
            success = false;
        }

        // Create OpenGL context
        m_openGLContext = SDL_GL_CreateContext(m_window);
        if (m_openGLContext == NULL)
        {
            errorStream << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << "\n";
            success = false;
        }

        // Initialize GLAD
        if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
        {
            errorStream << "Failed to initialize GLAD\n";
            success = false;
        }

        // Initialize OpenGL
        if (!InitGL())
        {
            errorStream << "Unable to initialize OpenGL!\n";
            success = false;
        }
    }

    if (!success)
    {
        errorStream << "SDLGraphicsProgram::SDLGraphicsProgram - Failed to initialize!\n";
        std::string errors = errorStream.str();
        SDL_Log("%s\n", errors.c_str());
    }
    else
    {
        SDL_Log("SDLGraphicsProgram::SDLGraphicsProgram - No SDL, GLAD, or OpenGL errors detected during initialization\n\n");
    }

    GetOpenGLVersionInfo();

    // We do not load objects here anymore.
    // Object loading is deferred to InitObjects.

    // Set initial camera position
    m_camera.SetCameraEyePosition(0.0f, 0.0f, 5.0f);
}

SDLGraphicsProgram::~SDLGraphicsProgram()
{
    ObjectManager::Instance().RemoveAll();
    SDL_DestroyWindow(m_window);
    m_window = nullptr;
    SDL_Quit();
}

bool SDLGraphicsProgram::InitGL()
{
    bool success = true;
    return success;
}

void SDLGraphicsProgram::InitObjects(const std::string &objFilePath)
{
    // Clear any existing objects
    ObjectManager::Instance().RemoveAll();

    // Create a new object and load the OBJ file
    Object *temp = new Object;
    std::cout << "Loading OBJ file: " << objFilePath << std::endl;
    if (!temp->LoadOBJ(objFilePath, ""))
    {
        std::cerr << "Error: Could not load the object. Exiting program." << std::endl;
        delete temp;
        exit(EXIT_FAILURE);
    }

    ObjectManager::Instance().AddObject(temp);
}

void SDLGraphicsProgram::Update()
{
    static float rot = 0;
    rot += 0.5f;
    if (rot > 360.0f)
    {
        rot -= 360.0f;
    }

    Object &obj = ObjectManager::Instance().GetObject(0);
    obj.GetTransform().LoadIdentity();
    obj.GetTransform().Translate(0.0f, -1.0f, -5.0f);
    // Uncomment if you want it to rotate
    // obj.GetTransform().Rotate(rot, 0.0f, 1.0f, 0.0f);
    obj.GetTransform().Scale(1.0f, 1.0f, 1.0f);

    ObjectManager::Instance().UpdateAll(m_screenWidth, m_screenHeight, m_camera.GetWorldToViewmatrix(),
                                        glm::vec3(m_camera.GetEyeXPosition(), m_camera.GetEyeYPosition(), m_camera.GetEyeZPosition()));
}

void SDLGraphicsProgram::Render()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    glViewport(0, 0, m_screenWidth, m_screenHeight);
    glClearColor(0.2f, 0.2f, 0.2f, 1.f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    // Render all objects
    ObjectManager::Instance().RenderAll();

    SDL_Delay(50);
}

void SDLGraphicsProgram::Loop()
{
    bool quit = false;
    SDL_Event e;
    SDL_StartTextInput();

    float cameraSpeed = 0.5f;

    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            if (e.type == SDL_MOUSEMOTION)
            {
                int mouseX = e.motion.x;
                int mouseY = e.motion.y;
                m_camera.MouseLook(mouseX, mouseY);
            }

            switch (e.type)
            {
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym)
                {
                case SDLK_LEFT:
                    m_camera.MoveLeft(cameraSpeed);
                    break;
                case SDLK_RIGHT:
                    m_camera.MoveRight(cameraSpeed);
                    break;
                case SDLK_UP:
                    m_camera.MoveForward(cameraSpeed);
                    break;
                case SDLK_DOWN:
                    m_camera.MoveBackward(cameraSpeed);
                    break;
                case SDLK_RSHIFT:
                    // MoveUp if desired
                    break;
                case SDLK_RCTRL:
                    // MoveDown if desired
                    break;
                case SDLK_q:
                    quit = true;
                    break;
                case SDLK_w:
                    m_wireframeMode = !m_wireframeMode;
                    if (m_wireframeMode)
                    {
                        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                        std::cout << "Wireframe mode enabled." << std::endl;
                    }
                    else
                    {
                        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                        std::cout << "Filled mode enabled." << std::endl;
                    }
                    break;
                }
                break;
            }
        }

        Update();
        Render();
        SDL_GL_SwapWindow(GetSDLWindow());
    }

    SDL_StopTextInput();
}

SDL_Window *SDLGraphicsProgram::GetSDLWindow()
{
    return m_window;
}

void SDLGraphicsProgram::GetOpenGLVersionInfo()
{
    SDL_Log("(Note: If you have two GPUs, make sure the correct one is selected)");
    SDL_Log("Vendor: %s", (const char *)glGetString(GL_VENDOR));
    SDL_Log("Renderer: %s", (const char *)glGetString(GL_RENDERER));
    SDL_Log("Version: %s", (const char *)glGetString(GL_VERSION));
    SDL_Log("Shading language: %s", (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
}
