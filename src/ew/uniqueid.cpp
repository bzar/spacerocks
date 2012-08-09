#include "uniqueid.h"

UID getUID()
{
  static UID next = 0;
  return next++;
}