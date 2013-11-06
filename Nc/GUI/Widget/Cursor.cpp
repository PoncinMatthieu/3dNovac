
#include "Cursor.h"

using namespace Nc;
using namespace Nc::GUI;

GUI::Cursor::Cursor(Graphic::IWindow *window, Graphic::Sprite *sprite, const Math::Vector2i &center)
    : Image(sprite, Left | Top), _window(window), _center(center)
{
}

void    GUI::Cursor::Update(float elapsedTime)
{
    const Math::Vector2i &pos = _window->Input()->MousePosition();
    _pos = pos - _center;
}
