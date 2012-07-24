#include "GL/glfw3.h"
#include "GL/glhck.h"
#include <cstdlib>

int const WIDTH = 800;
int const HEIGHT = 480;

struct Game {
  bool running;
};

static int windowCloseCallback(GLFWwindow window);
int gameloop(GLFWwindow& window);


int main(int argc, char** argv)
{
  if (!glfwInit())
    return EXIT_FAILURE;

  glfwOpenWindowHint(GLFW_DEPTH_BITS, 24);
  GLFWwindow window = glfwOpenWindow(WIDTH, HEIGHT, GLFW_WINDOWED, "Space Rocks!", NULL);
  
  if(!window)
    return EXIT_FAILURE;

  glfwSwapInterval(0);
  glfwSetWindowCloseCallback(windowCloseCallback);
  
  if(!glhckInit(argc, argv))
    return EXIT_FAILURE;

  if(!glhckDisplayCreate(WIDTH, HEIGHT, GLHCK_RENDER_OPENGL))
    return EXIT_FAILURE;
  
  int retval = gameloop(window);
  
  glhckTerminate();
  glfwTerminate();

  return retval;
}


static int windowCloseCallback(GLFWwindow window)
{
  Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
  game->running = false;
  return GL_FALSE;
}

int gameloop(GLFWwindow& window)
{
  glhckCamera *camera = glhckCameraNew();
  
  if(!camera)
    return EXIT_FAILURE;
  
  glhckCameraRange(camera, 0.1f, 1000.0f);
  kmVec3 cameraPos = { 0, 0, -40 };
  kmVec3 cameraRot = { 0, 0, 0 };
  
  glhckObject* sprite = glhckSpriteNew("glhck.png", 100, GLHCK_TEXTURE_DEFAULTS);
  
  if(!sprite)
    return EXIT_FAILURE;
  
  glhckObjectPositionf(sprite, 0, 0, 0);
  
  Game game = { true };
  glfwSetWindowUserPointer(window, &game);
  
  glhckText *text = glhckTextNew(512, 512);
  unsigned int font = glhckTextNewFont(text, "/usr/share/fonts/truetype/freefont/FreeSans.ttf");
  
  glhckMemoryGraph();

  float now = glfwGetTime();
  float total = 0;
  while(game.running)
  {
    float delta = glfwGetTime() - now;
    now += delta;
    total += delta;
    
    glfwPollEvents();
    
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
    {
      game.running = false;
    }
    
    glhckCameraUpdate(camera);
    glhckCameraPosition(camera, &cameraPos);
    glhckCameraTargetf(camera, cameraPos.x, cameraPos.y, cameraPos.z + 1);
    glhckCameraRotate(camera, &cameraRot);
    
    glhckObjectRotatef(sprite, total * 90, total * 360, total * 20);
    glhckObjectDraw(sprite);
    glhckRender();
    
    glhckTextDraw(text, font, 32, -64 + static_cast<int>(total * 64) % (WIDTH + 64), 16, "Tekstiä ", NULL);
    glhckTextRender(text);
    
    glfwSwapBuffers();
    glhckClear();
  }
 
  return EXIT_SUCCESS;
}
