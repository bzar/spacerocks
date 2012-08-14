#ifndef CONTROLCONTEXT_HH
#define CONTROLCONTEXT_HH

#include <vector>

namespace ew
{
  class ControlContext
  {
  public:
    ControlContext() : keyStates(INITIAL_MAX_KEY_CODE, false) {}
    virtual bool keyDown(int const key) { return false; }
    virtual bool keyPush(int const key)
    {
      if(key >= keyStates.size())
      {
        keyStates.resize(key - 1, false);
      }

      bool pushed = !keyStates[key] && keyDown(key);
      keyStates[key] = keyDown(key);

      return pushed;
    }
    virtual void update() {}

  private:
    static int const INITIAL_MAX_KEY_CODE = 350;
    std::vector<bool> keyStates;
  };
};
#endif