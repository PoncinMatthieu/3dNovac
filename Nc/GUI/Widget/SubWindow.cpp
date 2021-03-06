
/*-----------------------------------------------------------------------------

	3dNovac GUI
	Copyright (C) 2010-2011, The 3dNovac Team

    This file is part of 3dNovac.

    3dNovac is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    3dNovac is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with 3dNovac.  If not, see <http://www.gnu.org/licenses/>.

    File Created At:        22/06/2012
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include <Nc/Graphics/Object/Sprite.h>
#include "SubWindow.h"

using namespace Nc;
using namespace Nc::GUI;

SubWindow::SubWindow(Graphic::IWindow *windowParent, const AlignmentMask &alignment, const Vector2i &size)
    : Widget(alignment, size)
{
    _subWindow = new Graphic::SubWindow(windowParent);
    _subWindow->Create(_size);
    _subWindow->InitSceneGraphManager();
    _sprite = new Graphic::Sprite(_size, Graphic::GL::Texture(), Box2i(Vector2f(0,0), _size), Graphic::GL::Blend::Disabled);
}

SubWindow::SubWindow(const SubWindow &w)
    : Widget(w)
{
    _sprite = new Graphic::Sprite(_size, Graphic::GL::Texture(), Box2i(Vector2f(0,0), _size), Graphic::GL::Blend::Disabled);
    _subWindow = new Graphic::SubWindow(w._subWindow->Parent());
    _subWindow->Create(_size);
    _subWindow->InitSceneGraphManager();
}

SubWindow &SubWindow::operator = (const SubWindow &w)
{
    Widget::operator = (w);
    _sprite = new Graphic::Sprite(_size, Graphic::GL::Texture(), Box2i(Vector2f(0,0), _size), Graphic::GL::Blend::Disabled);
    _subWindow = new Graphic::SubWindow(w._subWindow->Parent());
    _subWindow->Create(_size);
    _subWindow->InitSceneGraphManager();
	return *this;
}

SubWindow::~SubWindow()
{
    delete _sprite;
    delete _subWindow;
}

void    SubWindow::Resize()
{
    Widget::Resize();

    // genere a resize event
    System::Event e(_subWindow->Input(), System::Event::Resized);
    e.size.width = _size.data[0];
    e.size.height = _size.data[1];
    _subWindow->Input()->GenereEvent(e);

    _sprite->Size(Vector2i(e.size.width, e.size.height));
    _sprite->TextureBox(Box2i(Vector2f(0,0), _sprite->Size()));

    Vector2i pos;
    AbsolutePos(pos);
    _subWindow->Pos(pos);
}

void    SubWindow::Reposed()
{
    Resized();
}

void    SubWindow::Draw(Graphic::SceneGraph *scene)
{
    Widget::Draw(scene);

    if (_sprite->GetTexture().GetIndex() != _subWindow->RenderingResult().GetIndex())
        _sprite->SetTexture(_subWindow->RenderingResult());
    _sprite->RenderNode(scene);
}
