#include "state.h"

State::State(Engine* engine, World* world, std::vector<Phase*> const& phases) :
  engine(engine), world(world), phases(phases)
{

}

void State::process(float const delta)
{
  for(Phase* phase : phases)
  {
    phase->execute(delta);
  }
}