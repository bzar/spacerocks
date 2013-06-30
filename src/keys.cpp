#include "keys.h"

bool actionKeyPush(KeyAction const action, ew::ControlContext* ctx)
{
  for(int const& k : KEY_ACTION_MAPPING[action])
  {
    if(ctx->keyPush(k))
    {
      return true;
    }
  }
  return false;
}

bool actionKeyDown(KeyAction const action, ew::ControlContext* ctx)
{
  for(int const& k : KEY_ACTION_MAPPING[action])
  {
    if(ctx->keyDown(k))
    {
      return true;
    }
  }
  return false;
}

