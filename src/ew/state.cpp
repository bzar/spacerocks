#include "state.h"

State::State(Engine* engine, World* world) :
  engine(engine), world(world), phases()
{

}

void State::setPhases(std::vector<Phase*> const& value)
{
  phases = value;
}

void State::process(float const delta)
{
  for(Phase* phase : phases)
  {
    phase->execute(delta);
  }
}