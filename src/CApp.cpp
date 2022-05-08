#include "CApp.h"
#include "SDL.h"
#include "draw/sdlimagecanvas.h"
#include "geometry/vector.h"
#include "ops/transform.h"
#include "scene/camera.h"
#include <iomanip>
#include <iostream>

CApp::CApp() : m_isRunning(true), m_pWindow(nullptr), m_pRenderer(nullptr) {}

bool CApp::OnInit() {
  // Initialize SDL2 and setup renderer

  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    return false;
  }

  m_pWindow =
      SDL_CreateWindow("RenGen", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       1280, 720, SDL_WINDOW_SHOWN);

  if (m_pWindow != NULL) {
    m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);
    m_Canvas.Initialize(1280, 720, m_pRenderer);

    rengen::scene::Camera testCamera;
    testCamera.SetCameraToWorld(
        rengen::ops::Translate(rengen::geometry::Vec3f(0, 0, 0)));

    testCamera.SetLookAt(rengen::geometry::Vec3f(0.0, 2.0, 0.0));
    testCamera.SetUp(rengen::geometry::Vec3f(0, 0, 1));
    testCamera.SetRenderPlaneDistance(1.0);
    testCamera.SetHorzSize(1.0);
    testCamera.SetAspect(1.0);
    testCamera.UpdateCameraGeometry();

    auto screenCenter = testCamera.GetScreenCenter();
    auto screenU = testCamera.GetU();
    auto screenV = testCamera.GetV();

    std::cout << "Camera Screen Centre" << std::endl;
    PrintVector(rengen::geometry::Vec3f(screenCenter));
    std::cout << "\nCamera U Vector;" << std::endl;
    PrintVector(screenU);
    std::cout << "\nCamera V Vector:" << std::endl;
    PrintVector(screenV);

  } else {
    return false;
  }

  return true;
}

int CApp::OnExecute() {
  SDL_Event event;

  if (OnInit() == false) {
    return -1;
  }

  while (m_isRunning) {
    while (SDL_PollEvent(&event) != 0) {
      OnEvent(&event);
    }

    OnLoop();
    OnRender();
  }

  return 0;
}

void CApp::OnEvent(SDL_Event *event) {
  if (event->type == SDL_QUIT) {
    m_isRunning = false;
  }
}

void CApp::OnLoop() {}

void CApp::OnRender() {
  // Set the background color to white.
  SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, 255);
  SDL_RenderClear(m_pRenderer);

  // Render the Scene
  m_Scene.Render(m_Canvas);

  m_Canvas.Display();

  // Show the result;
  SDL_RenderPresent(m_pRenderer);
}

void CApp::OnExit() {
  // Tidy up SDL2
  SDL_DestroyRenderer(m_pRenderer);
  SDL_DestroyWindow(m_pWindow);
  m_pWindow = nullptr;
  SDL_Quit();
}

// PRIVATE FUNCTIONS
void CApp::PrintVector(const rengen::geometry::Vec3f &inputVector) {
  for (int row = 0; row < 3; row++) {
    std::cout << std::fixed << std::setprecision(3) << inputVector[row]
              << std::endl;
  }
}