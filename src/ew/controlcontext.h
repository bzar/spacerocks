#ifndef CONTROLCONTEXT_HH
#define CONTROLCONTEXT_HH

class ControlContext
{
public:
  virtual bool key(int const key) { return false; }
};
#endif