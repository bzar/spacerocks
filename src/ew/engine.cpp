#include "engine.h"
#include "GL/glhck.h"
#include "world.h"
#include <iostream>

int const UPDATE_ITERATIONS = 10;

Engine::Engine(GLFWwindow* window, ControlContext* controlContext) :
  controlContext(controlContext), window(window), states(),
  current(nullptr), timer(), running(false)
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
    current->process(delta);
    current->getWorld()->maintenance();

    glfwSwapBuffers();
    glhckClear();

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

GLFWwindow* Engine::getWindow() const
{
  return window;
}

ControlContext* Engine::getControlContext() const
{
  return controlContext;
}

Timer const& Engine::getTimer() const
{
  return timer;
}