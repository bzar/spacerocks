#include "GLFW/glfw3.h"
#include "glhck/glhck.h"

#include "ew/engine.h"
#include "ew/integration/glfwcontrolcontext.h"
#include "ew/integration/glfwtimecontext.h"
#include "ew/integration/glhckglfwrendercontext.h"
#include "states.h"
#include "titlestate.h"
#include "highscorestate.h"
#include "gamestate.h"
#include "gameoverstate.h"
#include "sound.h"

#include <cstdlib>
#include <random>
#include <iostream>

int const WIDTH = 800;
int const HEIGHT = 480;

void windowCloseCallback(GLFWwindow* window);
void windowResizeCallback(GLFWwindow* window, int width, int height);
int gameloop(GLFWwindow* window);

#ifdef PANDORA
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#ifndef FBIO_WAITFORVSYNC
#define FBIO_WAITFORVSYNC _IOW('F', 0x20, __u32)
#endif

// Custom render context to include pandora's vsync
class PandoraGlhckGLFWRenderContext : public GlhckGLFWRenderContext
{
public:
  PandoraGlhckGLFWRenderContext(GLFWwindow* window) : GlhckGLFWRenderContext(window)
  {
    fbdev = open ("/dev/fb0", O_RDONLY /* O_RDWR */ );
    if ( fbdev < 0 )
    {
      std::cerr << "EGL Couldn't open /dev/fb0 for vsync" << std::endl;
    }
  }

  virtual void postRender()
  {
#ifdef VSYNC
    int arg = 0;
    ioctl( fbdev, FBIO_WAITFORVSYNC, &arg );
#endif
    GlhckGLFWRenderContext::postRender();
  }

private:
  int fbdev = -1;
};
#endif


int main(int argc, char** argv)
{
  if (!glfwInit())
    return EXIT_FAILURE;

  glhckCompileFeatures features;
  glhckGetCompileFeatures(&features);
  if (features.render.glesv1 || features.render.glesv2) {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_DEPTH_BITS, 16);
  }
  if (features.render.glesv2) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  }
  if (features.render.opengl) {
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
  }

  GLFWmonitor* monitor = nullptr;
#ifdef FULLSCREEN
  int monitors = -1;
  GLFWmonitor* monitor = glfwGetMonitors(&monitors)[0];
#endif
  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Space Rocks!", monitor, nullptr);
  glfwMakeContextCurrent(window);

  if(!window)
  {
    // FIXME: use error callback
    // std::cerr << "GLFW error: " << glfwErrorString(glfwGetError()) << std::endl;
    return EXIT_FAILURE;
  }

#ifdef VSYNC
  glfwSwapInterval(1);
#endif

  glfwSetWindowCloseCallback(window, windowCloseCallback);

  if(!glhckContextCreate(argc, argv))
  {
    std::cerr << "GLHCK initialization error" << std::endl;
    return EXIT_FAILURE;
  }

  if(!glhckDisplayCreate(WIDTH, HEIGHT, GLHCK_RENDER_AUTO))
  {
    std::cerr << "GLHCK display create error" << std::endl;
    return EXIT_FAILURE;
  }

  Sound::init();

  int retval = gameloop(window);

  Sound::exit();

  glhckContextTerminate();
  glfwTerminate();

  return retval;
}


void windowCloseCallback(GLFWwindow* window)
{
  ew::Engine* engine = static_cast<ew::Engine*>(glfwGetWindowUserPointer(window));
  engine->quit();
}

void windowResizeCallback(GLFWwindow* window, int width, int height)
{
  glhckDisplayResize(width, height);

  kmMat4 proj;
  kmMat4Scaling(&proj, 2.0f/WIDTH, 2.0f/HEIGHT, 0);
  glhckRenderProjectionOnly(&proj);
  glhckRenderFlip(0);
}

int gameloop(GLFWwindow* window)
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<unsigned> seedGenerator;
  srand(seedGenerator(gen));

  kmMat4 proj;
  kmMat4Scaling(&proj, 2.0f/WIDTH, 2.0f/HEIGHT, 0);
  glhckRenderProjectionOnly(&proj);
  glhckRenderFlip(0);

  GLFWControlContext controlContext(window);
#ifdef PANDORA
  PandoraGlhckGLFWRenderContext renderContext(window);
#else
  GlhckGLFWRenderContext renderContext(window);
#endif
  GLFWTimeContext timeContext;
  ew::Engine engine(&controlContext, &renderContext, &timeContext);

  glfwSetWindowUserPointer(window, &engine);
  glfwSetWindowSizeCallback(window, windowResizeCallback);

  TitleState::init();
  TitleState title(&engine);

  HighScoreState::init();
  HighScoreState highscores(&engine);

  GameState::init();
  GameState game(&engine);

  GameOverState::init();
  GameOverState gameover(&engine);

  engine.addState(States::TITLE, &title);
  engine.addState(States::HIGHSCORES, &highscores);
  engine.addState(States::GAME, &game);
  engine.addState(States::GAMEOVER, &gameover);

  engine.setState(States::TITLE);

  glhckMemoryGraph();

  engine.run();

  GameState::term();
  HighScoreState::term();
  TitleState::term();

  return EXIT_SUCCESS;
}
