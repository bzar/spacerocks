#include "engine.h"
#include "world.h"

Engine::Engine(ControlContext* controlContext, RenderContext* renderContext, TimeContext* timeContext) :
  controlContext(controlContext), renderContext(renderContext), timeContext(timeContext),
  states(), current(nullptr), timer(timeContext), running(false)
{
}

void Engine::run()
{
  running = true;
  current->getWorld()->maintenance();

  while(running)
  {
    double delta = timer.getDeltaTime();
    controlContext->update();
    renderContext->preRender();
    current->process(delta);
    current->getWorld()->maintenance();
    renderContext->postRender();
    renderContext->waitUntilNextFrame();
    timer.tick();
  }
}

void Engine::quit()
{
  running = false;
}

void Engine::addState(int id, State* state)
{
  auto existing = states.find(id);
  if(existing != states.end())
  {
    if(existing->second == current)
      return;

    delete existing->second;
  }

  states[id] = state;
}

void Engine::setState(int id)
{
  auto existing = states.find(id);
  if(existing == states.end() || existing->second == current)
    return;

  current = existing->second;
}

ControlContext* Engine::getControlContext() const
{
  return controlContext;
}

RenderContext* Engine::getRenderContext() const
{
  return renderContext;
}

TimeContext* Engine::getTimeContext() const
{
  return timeContext;
}

Timer const& Engine::getTimer() const
{
  return timer;
}