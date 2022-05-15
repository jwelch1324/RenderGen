#include "CApp.h"
#include "SDL.h"
#include "SDL_render.h"
#include "draw/sdlimagecanvas.h"
#include "geometry/point.h"
#include "geometry/shapes/sphere.h"
#include "geometry/vector.h"
#include "ops/transform.h"
#include "scene/camera.h"
#include "scene/scene.h"
#include "system/render_subsystem/serial_subsystem.h"
#include <iomanip>
#include <iostream>
#include <memory>

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

    // Set the background color to white.
    SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, 255);
    SDL_RenderClear(m_pRenderer);

    // // Render the Scene
    // auto moveMe = rengen::ops::Translate(rengen::geometry::Vec3f(1, 0, 0));
    // auto other = moveMe.GetInverse();
    // auto sphere2 = std::make_shared<rengen::geometry::shapes::Sphere>(
    //     0.5, &moveMe, &other);
    // sphere2->SetBaseColor(rengen::geometry::Point3f(0, 255, 0));
    // m_Scene.AddShape(sphere2);

    // auto sphere = std::make_shared<rengen::geometry::shapes::Sphere>(
    //     rengen::geometry::shapes::Sphere());
    // m_Scene.AddShape(sphere);
    m_pRenderSubsystem = new rengen::system::render::SerialRenderer();
    m_Scene.SetRenderSubSystem(m_pRenderSubsystem);
    m_Scene.Render(m_Canvas);
    m_Canvas.Display();

    // Show the result
    SDL_RenderPresent(m_pRenderer);

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
  //   // Set the background color to white.
  //   SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, 255);
  //   SDL_RenderClear(m_pRenderer);

  //   // Render the Scene
  //   m_Scene.Render(m_Canvas);

  //   m_Canvas.Display();

  //   // Show the result;
  //   SDL_RenderPresent(m_pRenderer);
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