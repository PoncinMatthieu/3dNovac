
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

    File Created At:        21/02/2012
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include <Nc/Graphics/Object/Sprite.h>
#include "ComboBox.h"
#include "WindowStyle.h"
#include "Looks.h"

using namespace Nc;
using namespace Nc::Graphic;
using namespace Nc::GUI;

ComboBox::ComboBox(GUI::SceneGraph *scene, Corner x, Corner y, const Vector2i &pos, const Vector2i &size, float fontSize, const Color &fontColor, const std::string &fontName, const std::string &looksName)
    : Widget(x, y, pos, size), _scene(scene), _fontSize(fontSize), _fontColor(fontColor), _fontName(fontName), _currentItem(NULL), _listUnrolled(false), _currentUnfoldList(NULL)
{
    StripLook *l = new StripLook(looksName + WindowStyle::SpriteName::ComboBox);
    UseLook(l);

    if (l->spriteLeft == NULL)
        throw Utils::Exception("ComboBox", "Cannot get the sprite '" + looksName + WindowStyle::SpriteName::ComboBox + "Left" + "' from the WindowStyle");
    if (l->spriteRight == NULL)
        throw Utils::Exception("ComboBox", "Cannot get the sprite '" + looksName + WindowStyle::SpriteName::ComboBox + "Right" + "' from the WindowStyle");
    if (l->spriteMiddle == NULL)
        throw Utils::Exception("ComboBox", "Cannot get the sprite '" + looksName + WindowStyle::SpriteName::ComboBox + "Middle" + "' from the WindowStyle");

    _spriteList = WindowStyle::Instance().GetNewSprite(looksName + WindowStyle::SpriteName::ComboBoxList);
    if (_spriteList == NULL)
        throw Utils::Exception("ComboBox", "Cannot get the sprite '" + looksName + WindowStyle::SpriteName::ComboBoxList + "' from the WindowStyle");

    _margin[0] = 5;
    _size[1] = l->spriteMiddle->Size()[1];
}

ComboBox::ComboBox(const ComboBox &cb)
    : Widget(cb)
{
    Copy(cb);
}

ComboBox &ComboBox::operator = (const ComboBox &cb)
{
    Widget::operator=(cb);
    Copy(cb);
    return *this;
}

void    ComboBox::Copy(const ComboBox &cb)
{
    _scene = cb._scene;
    _fontSize = cb._fontSize;
    _fontColor = cb._fontColor;
    _fontName = cb._fontName;
    _listUnrolled = cb._listUnrolled;
    _spriteList = new Sprite(*cb._spriteList);

    for (ListItem::const_iterator it = cb._itemList.begin(); it != cb._itemList.end(); ++it)
        _itemList.push_back(std::pair<Item*, Graphic::String*>(it->first->Clone(), new Graphic::String(*it->second)));
    _currentItem = (!_itemList.empty()) ? &(*_itemList.begin()) : NULL;
    _listUnrolled = false;
    _currentUnfoldList = NULL;
}


ComboBox::~ComboBox()
{
    delete _spriteList;

    for (ListItem::iterator it = _itemList.begin(); it != _itemList.end(); ++it)
    {
        delete it->first;
        delete it->second;
    }
}

void    ComboBox::Update()
{
    Widget::Update();

    // get the size of the widget, and update the size of the sprites
    Vector2i size;
    GetReelSize(size);

    _spriteList->Size(Vector2f(size[0] - static_cast<StripLook*>(_widgetLook)->spriteLeft->Size()[0] - static_cast<StripLook*>(_widgetLook)->spriteRight->Size()[0], _spriteList->Size()[1]));

    if (!_listUnrolled && _currentUnfoldList != NULL)
    {
        _scene->RemoveWidget(_currentUnfoldList);
        _currentUnfoldList = NULL;
    }
}

void    ComboBox::Draw(Graphic::SceneGraph *scene)
{
    Widget::Draw(scene);

    if (!_listUnrolled)
    {
        scene->PushModelMatrix();
        scene->ModelMatrix().AddTranslation(static_cast<StripLook*>(_widgetLook)->spriteLeft->Size()[0], (static_cast<StripLook*>(_widgetLook)->spriteMiddle->Size()[1] - _spriteList->Size()[1]) / 2, 0);
        if (_currentItem != NULL)
            _currentItem->second->RenderNode(scene);
        scene->PopModelMatrix();
    }
}

void    ComboBox::MouseButtonEvent(const System::Event &event)
{
    if (event.mouseButton.button == System::Mouse::Left && event.type == System::Event::MouseButtonReleased)
    {
        // test si la souris est sur le bouton
        Vector2i pos;
        Vector2i mousePos = static_cast<WindowInput*>(event.emitter)->MousePositionInGLCoord();
        GetReelPosRecursif(pos);

        if (!_listUnrolled)
        {
            Vector2i size;
            GetReelSize(size);
            if (Math::InRect(pos, size, mousePos))
            {
                pos[0] += static_cast<StripLook*>(_widgetLook)->spriteLeft->Size()[0];
                pos[1] += (static_cast<StripLook*>(_widgetLook)->spriteMiddle->Size()[1] - _spriteList->Size()[1]) / 2;
                pos[1] += _spriteList->Size()[1];

                _currentUnfoldList = new ComboBoxUnfoldList(this, Left, Bottom, pos, Vector2i(_spriteList->Size()[0], -_spriteList->Size()[1] * _itemList.size()));
                _scene->AddChild(_currentUnfoldList);
                _scene->Focus(_currentUnfoldList);
                _listUnrolled = true;
            }
        }
    }
}

void    ComboBox::AddItem(Item *item)
{
    Graphic::String *s = new Graphic::String(item->Data(), _fontSize, _fontColor, _fontName);
    s->Matrix.AddTranslation(_margin[0], s->Size()[1] / 2, 0);

    _itemList.push_back(std::pair<Item*, Graphic::String*>(item, s));
    _currentItem = &(*_itemList.begin());
    _stateChanged = true;
}

ComboBox::ComboBoxUnfoldList::ComboBoxUnfoldList(ComboBox *cb, Corner x, Corner y, const Vector2i &pos, const Vector2i &size)
    : Widget(x, y, pos, size), _cb(cb)
{
}

void        ComboBox::ComboBoxUnfoldList::MouseButtonEvent(const System::Event &event)
{
    if (event.mouseButton.button == System::Mouse::Left && event.type == System::Event::MouseButtonReleased)
    {
        Vector2i pos;
        Vector2i mousePos = static_cast<WindowInput*>(event.emitter)->MousePositionInGLCoord();
        _cb->GetReelPosRecursif(pos);

        Vector2i size = _cb->_spriteList->Size();
        pos[0] += static_cast<StripLook*>(_cb->_widgetLook)->spriteLeft->Size()[0];
        pos[1] += ((static_cast<StripLook*>(_cb->_widgetLook)->spriteMiddle->Size()[1] - _cb->_spriteList->Size()[1]) / 2);
        for (ListItem::iterator it = _cb->_itemList.begin(); it != _cb->_itemList.end(); ++it)
        {
            if (Math::InRect(pos, size, mousePos))
                _cb->_currentItem = &(*it);
            pos[1] -= _cb->_spriteList->Size()[1];
        }

        _cb->_listUnrolled = false;
        _cb->_stateChanged = true;
    }
}

void        ComboBox::ComboBoxUnfoldList::Draw(Graphic::SceneGraph *scene)
{
    scene->ModelMatrix().AddTranslation(0, -_cb->_spriteList->Size()[1], 0);
    for (ListItem::iterator it = _cb->_itemList.begin(); it != _cb->_itemList.end(); ++it)
    {
        _cb->_spriteList->RenderNode(scene);
        it->second->RenderNode(scene);
        scene->ModelMatrix().AddTranslation(0, -_cb->_spriteList->Size()[1], 0);
    }
}

void        ComboBox::ComboBoxUnfoldList::LeaveFocus()
{
    _cb->_listUnrolled = false;
    _cb->_stateChanged = true;
}

