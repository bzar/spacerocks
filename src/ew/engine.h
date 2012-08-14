#ifndef ENGINE_HH
#define ENGINE_HH

#include <map>

#include "state.h"
#include "timer.h"
#include "controlcontext.h"
#include "rendercontext.h"


class Engine
{
public:
  Engine(ControlContext* controlContext, RenderContext* renderContext);

  void run();
  void quit();
  void addState(int id, State* state);
  void setState(int id);
  ControlContext* getControlContext() const;
  Timer const& getTimer() const;

private:
  ControlContext* controlContext;
  RenderContext* renderContext;
  std::map<int, State*> states;
  State* current;

  Timer timer;
  bool running;
};
#endif
