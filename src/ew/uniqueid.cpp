#include "uniqueid.h"

ew::UID ew::getUID()
{
  static UID next = 0;
  return next++;
}