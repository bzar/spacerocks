#include "timer.h"

ew::Timer::Timer(TimeContext* context) :
  context(context), now(0), start(0), prev(0), prevFPS(0), fps(0), ticks(0)
{
  double time = context->timeInSeconds();
  now = time;
  start = time;
  prev = time;
  prevFPS = time;
}

void ew::Timer::tick()
{
  prev = now;
  now = context->timeInSeconds();

  if(ticks % 30 == 0) {
    double dt = now - prevFPS;
    fps = 30.0 / dt;
    prevFPS = now;
  }

  ++ticks;
}

double ew::Timer::getTime() const
{
  return now;
}

double ew::Timer::getDeltaTime() const
{
  return now - prev;
}

double ew::Timer::getStartTime() const
{
  return start;
}

double ew::Timer::getTotalTime() const
{
  return now - start;
}

double ew::Timer::getFPS() const
{
  return fps;
}

unsigned int ew::Timer::getTicks() const
{
  return ticks;
}