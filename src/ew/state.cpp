#include "state.h"

ew::State::State(Engine* engine, World* world) :
  engine(engine), world(world), phases()
{

}

void ew::State::setPhases(std::vector<Phase*> const& value)
{
  phases = value;
}

void ew::State::process(float const delta)
{
  for(Phase* phase : phases)
  {
    phase->execute(delta);
  }
}