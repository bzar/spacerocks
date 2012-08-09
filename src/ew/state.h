#ifndef STATE_HH
#define STATE_HH

#include <vector>

class Engine;
class Phase;
class World;

class State
{
public:
  State(Engine* engine, std::vector<Phase*> phases);
  virtual ~State();

  Engine* getEngine() const { return engine; }
  void process(float const delta);
  virtual World* getWorld() = 0;

protected:
  Engine* engine;
  std::vector<Phase*> phases;
};
#endif
