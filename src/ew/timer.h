#ifndef TIMER_HH
#define TIMER_HH

class Timer
{
public:
  Timer();
  
  void tick();
  double getTime() const;
  double getDeltaTime() const;
  double getStartTime() const;
  double getTotalTime() const;
  double getFPS() const;
  unsigned int getTicks() const;
  
private:
  double now;
  double start;
  double prev;
  double prevFPS;
  double fps;
  unsigned int ticks;

};

#endif