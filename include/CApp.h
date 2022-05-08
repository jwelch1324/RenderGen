#ifndef CAPP_H_
#define CAPP_H_

#include "draw/sdlimagecanvas.h"
#include "geometry/vector.h"
#include "scene/camera.h"
#include "scene/scene.h"
#include <SDL.h>

class CApp {
public:
  CApp();

  int OnExecute();
  bool OnInit();
  void OnEvent(SDL_Event *event);
  void OnLoop();
  void OnRender();
  void OnExit();

private:
  static void PrintVector(const rengen::geometry::Vec3f &inputVector);

private:
  // Draw canvas
  rengen::draw::SDLImageCanvas m_Canvas;

  // Instance of a Scene
  rengen::scene::Scene m_Scene;

  // SDL2 Related
  bool m_isRunning;
  SDL_Window *m_pWindow;
  SDL_Renderer *m_pRenderer;
};

#endif