#ifndef ENGINE_HH
#define ENGINE_HH

#include <map>

#include "state.h"
#include "timer.h"

#include "GL/glfw3.h"

class Engine
{
public:
  Engine(GLFWwindow* window);

  void run();
  void quit();
  void addState(int id, State* state);
  void setState(int id);
  GLFWwindow* getWindow() const;
  Timer const& getTimer() const;
private:
  GLFWwindow* window;
  std::map<int, State*> states;
  State* current;

  Timer timer;
  bool running;
};
#endif
