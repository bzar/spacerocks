#ifndef STATE_HH
#define STATE_HH

#include "phase.h"
#include "world.h"

#include <vector>

class Engine;
class World;

class State
{
public:
  State(Engine* engine, World* world);
  virtual ~State() {};

  Engine* getEngine() const { return engine; }
  void setPhases(std::vector<Phase*> const& value);
  virtual void process(float const delta);
  virtual World* getWorld() { return world; }

protected:
  Engine* engine;
  World* world;
  std::vector<Phase*> phases;
};
#endif
