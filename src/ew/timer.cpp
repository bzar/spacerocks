#include "timer.h"
#include "GL/glfw3.h"

Timer::Timer() :
  now(0), start(0), prev(0), prevFPS(0), fps(0), ticks(0)
{
  double time = glfwGetTime();
  now = time;
  start = time;
  prev = time;
  prevFPS = time;
}
  
void Timer::tick()
{
  prev = now;
  now = glfwGetTime();

  if(ticks % 30 == 0) {
    double dt = now - prevFPS;
    fps = 30.0 / dt;
    prevFPS = now;
  }
  
  ++ticks;
}

double Timer::getTime() const
{
  return now;
}

double Timer::getDeltaTime() const
{
  return now - prev;
}

double Timer::getStartTime() const
{
  return start;
}

double Timer::getTotalTime() const
{
  return now - start;
}

double Timer::getFPS() const
{
  return fps;
}

unsigned int Timer::getTicks() const
{
  return ticks;
}