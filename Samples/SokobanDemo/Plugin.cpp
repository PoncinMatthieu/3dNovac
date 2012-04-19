
#include "GameEngine.h"

extern "C" Nc::Contrib::GameEngine	*GetPlugin()
{
  SokobanDemo::GameEngine *game = new SokobanDemo::GameEngine(NULL, NULL);
  game->LimitFrameRate(60);
  return game;
}

