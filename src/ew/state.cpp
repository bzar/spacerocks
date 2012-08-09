#include "state.h"
#include "world.h"
#include "phase.h"

State::State(Engine* engine, World* world, std::vector<Phase*> phases) :
  engine(engine), world(world), phases(phases)
{

}

State::~State()
{
  for(Phase* p : phases)
  {
    delete p;
  }
}

void State::process(float const delta)
{
  for(Phase* phase : phases)
  {
    phase->execute(getWorld(), delta);
  }

  getWorld()->maintenance();
}