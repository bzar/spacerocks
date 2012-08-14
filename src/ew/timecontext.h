#ifndef TIMECONTEXT_HH
#define TIMECONTEXT_HH

namespace ew
{
  class TimeContext
  {
  public:
    virtual double timeInSeconds() = 0;
  };
};
#endif
