
#include "GameEngine.h"

extern "C" Nc::Contrib::GameEngine	*GetPlugin()
{
  LightDemo::GameEngine *game = new LightDemo::GameEngine(NULL, NULL);
  game->LimitFrameRate(60);
  return game;
}

