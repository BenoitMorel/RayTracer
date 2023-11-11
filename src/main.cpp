#include <iostream>
#include <assert.h>
#include <chrono>
#include "Scene.hpp"
#include "Camera.hpp"
#include "scenes/SceneFramedMirror.hpp"





int main(int, char **)
{
  auto start = std::chrono::high_resolution_clock::now();
  unsigned int imageWidth = 1000;
  double raysPerPixel = 1;
  unsigned int cores = 10;
  std::cout << "Creating scene..." << std::endl;

  auto scene = createSceneFramedMirror(imageWidth, raysPerPixel, cores);

  scene->beforeRender();
  std::cout << "Start ray tracing..." << std::endl;
  scene->camera->render(scene->world);
  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "done in " << duration.count() << "ms" << std::endl;
  return 0;
}
