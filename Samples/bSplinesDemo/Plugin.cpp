
#include "GameEngine.h"

extern "C" Nc::Contrib::GameEngine	*GetPlugin()
{
  GameEngine *game = new GameEngine(NULL, NULL);
  game->LimitFrameRate(60);
  return game;
}

