
#include "GameEngine.h"

extern "C" Nc::Contrib::GameEngine	*GetPlugin()
{
  FrustumDemo::GameEngine *game = new FrustumDemo::GameEngine(NULL, NULL);
  game->LimitFrameRate(60);
  return game;
}

