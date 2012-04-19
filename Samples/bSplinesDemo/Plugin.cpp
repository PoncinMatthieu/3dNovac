
#include "GameEngine.h"

extern "C" Nc::Contrib::GameEngine	*GetPlugin()
{
  bSplinesDemo::GameEngine *game = new bSplinesDemo::GameEngine(NULL, NULL);
  game->LimitFrameRate(60);
  return game;
}

