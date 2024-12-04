#include "SDLGraphicsProgram.hpp"
#include "ObjectManager.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

// Initialization function
// Returns a true or false value based on successful completion of setup.
// Takes in dimensions of window.
SDLGraphicsProgram::SDLGraphicsProgram(int w, int h) : m_screenWidth(w), m_screenHeight(h)
{
    // Initialization flag
    bool success = true;
    // String to hold any errors that occur.
    std::stringstream errorStream;
    // The window we'll be rendering to
    m_window = NULL;
    // Render flag

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
        // We want to request a double buffer for smooth updating.
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        // Create window
        m_window = SDL_CreateWindow("Lab",
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    m_screenWidth,
                                    m_screenHeight,
                                    SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

        // Check if Window did not create.
        if (m_window == NULL)
        {
            errorStream << "Window could not be created! SDL Error: " << SDL_GetError() << "\n";
            success = false;
        }

        // Create an OpenGL Graphics Context
        m_openGLContext = SDL_GL_CreateContext(m_window);
        if (m_openGLContext == NULL)
        {
            errorStream << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << "\n";
            success = false;
        }

        // Initialize GLAD Library
        if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
        {
            errorStream << "Failed to iniitalize GLAD\n";
            success = false;
        }

        // Initialize OpenGL
        if (!InitGL())
        {
            errorStream << "Unable to initialize OpenGL!\n";
            success = false;
        }
    }

    // If initialization did not work, then print out a list of errors in the constructor.
    if (!success)
    {
        errorStream << "SDLGraphicsProgram::SDLGraphicsProgram - Failed to initialize!\n";
        std::string errors = errorStream.str();
        SDL_Log("%s\n", errors.c_str());
    }
    else
    {
        SDL_Log("SDLGraphicsProgram::SDLGraphicsProgram - No SDL, GLAD, or OpenGL, errors detected during initialization\n\n");
    }

    // SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN); // Uncomment to enable extra debug support!
    GetOpenGLVersionInfo();

    // Setup our objects
    for (int i = 0; i < 1; ++i)
    {
        Object *temp = new Object;
        temp->LoadOBJ("../common/objects/bunny_centered.obj", "");
        ObjectManager::Instance().AddObject(temp);
    }

    // Create an instance of the Camera
    m_camera.SetCameraEyePosition(0.0f, 0.0f, 5.0f); // Set initial camera position
}

// Proper shutdown of SDL and destroy initialized objects
SDLGraphicsProgram::~SDLGraphicsProgram()
{
    // Reclaim all of our objects
    ObjectManager::Instance().RemoveAll();

    // Destroy window
    SDL_DestroyWindow(m_window);
    // Point m_window to NULL to ensure it points to nothing.
    m_window = nullptr;
    // Quit SDL subsystems
    SDL_Quit();
}

// Initialize OpenGL
// Setup any of our shaders here.
bool SDLGraphicsProgram::InitGL()
{
    // Success flag
    bool success = true;

    return success;
}

// Update OpenGL
void SDLGraphicsProgram::Update()
{
    // Rotate the object
    static float rot = 0;
    rot += 0.5f;
    if (rot > 360.0f)
    {
        rot -= 360.0f;
    }

    Object &obj = ObjectManager::Instance().GetObject(0);
    obj.GetTransform().LoadIdentity();
    obj.GetTransform().Translate(0.0f, -1.0f, -5.0f);
    // stop the object from rotating, uncomment to make it rotate
    // obj.GetTransform().Rotate(rot, 0.0f, 1.0f, 0.0f);
    obj.GetTransform().Scale(1.0f, 1.0f, 1.0f);

    // Update all of the objects
    ObjectManager::Instance().UpdateAll(m_screenWidth, m_screenHeight, m_camera.GetWorldToViewmatrix(), glm::vec3(m_camera.GetEyeXPosition(), m_camera.GetEyeYPosition(), m_camera.GetEyeZPosition()));
}

// Render
// The render function gets called once per loop
void SDLGraphicsProgram::Render()
{
    // Setup our OpenGL State machine
    // TODO: Read this
    // The below command is new!
    // What we are doing, is telling opengl to create a depth(or Z-buffer)
    // for us that is stored every frame.
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    // Initialize clear color
    // This is the background of the screen.
    glViewport(0, 0, m_screenWidth, m_screenHeight);
    glClearColor(0.2f, 0.2f, 0.2f, 1.f);
    // TODO: Read this
    // Clear color buffer and Depth Buffer
    // Remember that the 'depth buffer' is our
    // z-buffer that figures out how far away items are every frame
    // and we have to do this every frame!
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    // Nice way to debug your scene in wireframe!
    // glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    // Render all of our objects in a simple loop
    ObjectManager::Instance().RenderAll();

    // Delay to slow things down just a bit!
    SDL_Delay(50);
}

// Loops forever!
void SDLGraphicsProgram::Loop()
{
    // Main loop flag
    // If this is quit = 'true' then the program terminates.
    bool quit = false;
    // Event handler that handles various events in SDL
    // that are related to input and output
    SDL_Event e;
    // Enable text input
    SDL_StartTextInput();

    // Set the camera speed for how fast we move.
    float cameraSpeed = 0.5f;

    // While application is running
    while (!quit)
    {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0)
        {
            // User posts an event to quit
            // An example is hitting the "x" in the corner of the window.
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            // Handle keyboard input for the camera class
            if (e.type == SDL_MOUSEMOTION)
            {
                // Handle mouse movements
                int mouseX = e.motion.x;
                int mouseY = e.motion.y;
                m_camera.MouseLook(mouseX, mouseY);
            }
            switch (e.type)
            {
            // Handle keyboard presses
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
                    //    Camera::Instance().MoveUp(cameraSpeed);
                    break;
                case SDLK_RCTRL:
                    //    Camera::Instance().MoveDown(cameraSpeed);
                    break;
                case SDLK_q:
                    quit = true;
                    break;
                case SDLK_w:
                    m_wireframeMode = !m_wireframeMode; // Toggle wireframe mode
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
        } // End SDL_PollEvent loop.

        // Update our scene
        Update();
        // Render using OpenGL
        Render(); // TODO: potentially move this depending on your logic
                  // for how you handle drawing a triangle or rectangle.
        // Update screen of our specified window
        SDL_GL_SwapWindow(GetSDLWindow());
    }
    // Disable text input
    SDL_StopTextInput();
}

// Get Pointer to Window
SDL_Window *SDLGraphicsProgram::GetSDLWindow()
{
    return m_window;
}

// Helper Function to get OpenGL Version Information
void SDLGraphicsProgram::GetOpenGLVersionInfo()
{
    SDL_Log("(Note: If you have two GPU's, make sure the correct one is selected)");
    SDL_Log("Vendor: %s", (const char *)glGetString(GL_VENDOR));
    SDL_Log("Renderer: %s", (const char *)glGetString(GL_RENDERER));
    SDL_Log("Version: %s", (const char *)glGetString(GL_VERSION));
    SDL_Log("Shading language: %s", (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
}
