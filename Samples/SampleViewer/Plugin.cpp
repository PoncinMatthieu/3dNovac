
#include "GameEngine.h"

extern "C" Nc::Contrib::GameEngine	*GetPlugin()
{
  SampleViewer::GameEngine *game = new SampleViewer::GameEngine(NULL, NULL);
  game->LimitFrameRate(60);
  return game;
}
