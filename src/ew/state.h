#ifndef STATE_HH
#define STATE_HH

class Engine;

class State
{
public:
  State(Engine* engine) : engine(engine) {}

  Engine* getEngine() const { return engine; }

  virtual void input() = 0;
  virtual void update(float const delta) = 0;
  virtual void render() = 0;

protected:
  Engine* engine;
};
#endif
