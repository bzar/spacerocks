#ifndef ENGINE_HH
#define ENGINE_HH

#include <map>

#include "state.h"
#include "timer.h"
#include "controlcontext.h"
#include "rendercontext.h"
#include "timecontext.h"

namespace ew
{
  class Engine
  {
  public:
    Engine(ControlContext* controlContext, RenderContext* renderContext, TimeContext* timeContext);

    void run();
    void quit();
    void addState(int id, State* state);
    void setState(int id);
    ControlContext* getControlContext() const;
    RenderContext* getRenderContext() const;
    TimeContext* getTimeContext() const;
    Timer const& getTimer() const;

  private:
    ControlContext* controlContext;
    RenderContext* renderContext;
    TimeContext* timeContext;
    std::map<int, State*> states;
    State* current;

    Timer timer;
    bool running;
  };
};
#endif
