
#include "GameEngine.h"

extern "C" Nc::Contrib::GameEngine	*GetPlugin()
{
  SimpleParticleEffect::GameEngine *game = new SimpleParticleEffect::GameEngine(NULL);
  game->LimitFrameRate(60);
  return game;
}

