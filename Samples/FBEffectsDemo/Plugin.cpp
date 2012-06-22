
#include "GameEngine.h"

extern "C" Nc::Contrib::GameEngine	*GetPlugin()
{
  FBEffectsDemo::GameEngine *game = new FBEffectsDemo::GameEngine(NULL);
  game->LimitFrameRate(60);
  return game;
}

