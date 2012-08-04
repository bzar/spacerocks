#include "engine.h"
#include "GL/glhck.h"

int const UPDATE_ITERATIONS = 10;

Engine::Engine(GLFWwindow* window) :
  window(window), states(), current(nullptr), timer(), running(false)
{
}

void Engine::run()
{
  running = true;

  while(running)
  {
    double totalDelta = timer.getDeltaTime();

    for(int iteration = 0; iteration < UPDATE_ITERATIONS; ++iteration)
    {
      double delta = totalDelta / UPDATE_ITERATIONS;
      current->input();
      current->update(delta);
    }

    current->render();

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

Timer const& Engine::getTimer() const
{
  return timer;
}