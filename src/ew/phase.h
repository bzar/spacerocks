#ifndef PHASE_HH
#define PHASE_HH

namespace ew
{
  class Phase
  {
  public:
    virtual void execute(float const delta) = 0;

  };
};
#endif
