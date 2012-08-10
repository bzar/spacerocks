#ifndef STATE_HH
#define STATE_HH

class Engine;
class World;

class State
{
public:
  State(Engine* engine);
  virtual ~State() {};

  Engine* getEngine() const { return engine; }
  virtual void process(float const delta) = 0;
  virtual World* getWorld() = 0;

protected:
  Engine* engine;
};
#endif
