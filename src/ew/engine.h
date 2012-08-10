#ifndef ENGINE_HH
#define ENGINE_HH

#include <map>

#include "state.h"
#include "timer.h"
#include "controlcontext.h"

#include "GL/glfw3.h"

class Engine
{
public:
  Engine(GLFWwindow* window, ControlContext* controlContext);

  void run();
  void quit();
  void addState(int id, State* state);
  void setState(int id);
  GLFWwindow* getWindow() const;
  ControlContext* getControlContext() const;
  Timer const& getTimer() const;

private:
  ControlContext* controlContext;
  GLFWwindow* window;
  std::map<int, State*> states;
  State* current;

  Timer timer;
  bool running;
};
#endif
