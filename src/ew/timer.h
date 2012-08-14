#ifndef TIMER_HH
#define TIMER_HH

#include "timecontext.h"

class Timer
{
public:
  Timer(TimeContext* context);

  void tick();
  double getTime() const;
  double getDeltaTime() const;
  double getStartTime() const;
  double getTotalTime() const;
  double getFPS() const;
  unsigned int getTicks() const;

private:
  TimeContext* context;
  double now;
  double start;
  double prev;
  double prevFPS;
  double fps;
  unsigned int ticks;

};

#endif