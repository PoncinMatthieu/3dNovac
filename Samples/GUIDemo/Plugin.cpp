
#include "GameEngine.h"

extern "C" Nc::Contrib::GameEngine	*GetPlugin()
{
  GUIDemo::GameEngine *game = new GUIDemo::GameEngine(NULL, NULL);
  game->LimitFrameRate(60);
  return game;
}

