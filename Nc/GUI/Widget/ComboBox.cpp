
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
#include "../Look/StyleSheet.h"
#include "../Look/Looks.h"

using namespace Nc;
using namespace Nc::Graphic;
using namespace Nc::GUI;

ComboBox::ComboBox(GUI::SceneGraph *scene, const AlignmentMask &alignment, const Vector2i &size, const std::string &looksName)
    : Widget(alignment, size), _scene(scene), _currentItem(NULL), _listUnrolled(false), _currentUnfoldList(NULL)
{
    StripLook *l = new StripLook(looksName + StyleSheet::Name::ComboBox);
    UseLook(l);

    if (l->spriteLeft == NULL)
        throw Utils::Exception("ComboBox", "Cannot get the sprite '" + looksName + StyleSheet::Name::ComboBox + "Left" + "' from the StyleSheet");
    if (l->spriteRight == NULL)
        throw Utils::Exception("ComboBox", "Cannot get the sprite '" + looksName + StyleSheet::Name::ComboBox + "Right" + "' from the StyleSheet");
    if (l->spriteMiddle == NULL)
        throw Utils::Exception("ComboBox", "Cannot get the sprite '" + looksName + StyleSheet::Name::ComboBox + "Middle" + "' from the StyleSheet");

    _spriteList = StyleSheet::Instance().GetNewSprite(looksName + StyleSheet::Name::ComboBoxList);
    if (_spriteList == NULL)
        throw Utils::Exception("ComboBox", "Cannot get the sprite '" + looksName + StyleSheet::Name::ComboBoxList + "' from the StyleSheet");

    PaddingH(5);
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
    _listUnrolled = cb._listUnrolled;
    _spriteList = new Sprite(*cb._spriteList);

    for (ListPItem::const_iterator it = cb._itemList.begin(); it != cb._itemList.end(); ++it)
        _itemList.push_back(static_cast<Item*>((*it)->Clone()));
    _currentItem = (!_itemList.empty()) ? (*_itemList.begin()) : NULL;
    _listUnrolled = false;
    _currentUnfoldList = NULL;
}


ComboBox::~ComboBox()
{
    delete _spriteList;

    for (ListPItem::iterator it = _itemList.begin(); it != _itemList.end(); ++it)
        delete *it;
}

void    ComboBox::UpdateState()
{
    Widget::UpdateState();

    _spriteList->Size(Vector2f(_size[0] - static_cast<StripLook*>(_widgetLook)->spriteLeft->Size()[0] - static_cast<StripLook*>(_widgetLook)->spriteRight->Size()[0], _spriteList->Size()[1]));

    for (ListPItem::iterator it = _itemList.begin(); it != _itemList.end(); ++it)
    {
        (*it)->Pos(Vector2i(PaddingLeft(), ((_spriteList->Size()[1] - (*it)->TextSize()[1]) / 2)));
    }

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
        if (_currentItem != NULL)
        {
            scene->PushModelMatrix();
            scene->ModelMatrix().AddTranslation(static_cast<StripLook*>(_widgetLook)->spriteLeft->Size()[0],
                                                (static_cast<StripLook*>(_widgetLook)->spriteMiddle->Size()[1] / 2) - (_currentItem->TextSize()[1] / 2) - ((_spriteList->Size()[1] / 2) - (_currentItem->TextSize()[1] / 2)), 0);
            _currentItem->RenderNode(scene);
            scene->PopModelMatrix();
        }
    }
}

void    ComboBox::MouseButtonEvent(const System::Event &event)
{
    if (event.mouseButton.button == System::Mouse::Left && event.type == System::Event::MouseButtonReleased)
    {
        // test si la souris est sur le bouton
        Vector2i pos;
        Vector2i mousePos = static_cast<WindowInput*>(event.emitter)->MousePositionInGLCoord();
        AbsolutePos(pos);

        if (!_listUnrolled)
        {
            if (Math::Test::PointInRect(mousePos, pos, _size))
            {
                pos[0] += static_cast<StripLook*>(_widgetLook)->spriteLeft->Size()[0];
                pos[1] += (static_cast<StripLook*>(_widgetLook)->spriteMiddle->Size()[1] - _spriteList->Size()[1]) / 2;
                pos[1] += _spriteList->Size()[1];

                _currentUnfoldList = new ComboBoxUnfoldList(this, Left | Bottom, pos, Vector2i(_spriteList->Size()[0], -_spriteList->Size()[1] * _itemList.size()));
                _scene->AddChild(_currentUnfoldList);
                _scene->Focus(_currentUnfoldList);
                _listUnrolled = true;
            }
        }
    }
}

void    ComboBox::AddItem(Item *item)
{
    // init the matrix of the text
    item->Text()->Matrix.AddTranslation(PaddingLeft(), (_spriteList->Size()[1] / 2) - (item->Text()->Size()[1] / 2), 0);

    _itemList.push_back(item);
    _currentItem = *_itemList.begin();
    _stateChanged = true;
}

void    ComboBox::ItemChoosed(Item *item)
{
    if (_currentItem != item)
    {
        if (_currentItem)
            _currentItem->Selected(false);
        _currentItem = item;
        _currentItem->Selected(true);
        SendEvent(Event::ItemSelected);
    }
}

ComboBox::ComboBoxUnfoldList::ComboBoxUnfoldList(ComboBox *cb, const AlignmentMask &alignment, const Vector2i &pos, const Vector2i &size)
    : Widget(alignment, size), _cb(cb)
{
    _pos = pos;
}

void        ComboBox::ComboBoxUnfoldList::MouseButtonEvent(const System::Event &event)
{
    if (event.mouseButton.button == System::Mouse::Left && event.type == System::Event::MouseButtonReleased)
    {
        Vector2i pos;
        Vector2i mousePos = static_cast<WindowInput*>(event.emitter)->MousePositionInGLCoord();
        _cb->AbsolutePos(pos);

        Vector2i size = _cb->_spriteList->Size();
        pos[0] += static_cast<StripLook*>(_cb->_widgetLook)->spriteLeft->Size()[0];
        pos[1] += ((static_cast<StripLook*>(_cb->_widgetLook)->spriteMiddle->Size()[1] - _cb->_spriteList->Size()[1]) / 2);
        for (ListPItem::iterator it = _cb->_itemList.begin(); it != _cb->_itemList.end(); ++it)
        {
            if (Math::Test::PointInRect(mousePos, pos, size))
            {
                _cb->ItemChoosed(*it);
            }
            pos[1] -= _cb->_spriteList->Size()[1];
        }

        _cb->_listUnrolled = false;
        _cb->_stateChanged = true;
    }
}

void        ComboBox::ComboBoxUnfoldList::Draw(Graphic::SceneGraph *scene)
{
    scene->ModelMatrix().AddTranslation(0, -_cb->_spriteList->Size()[1], 0);
    for (ListPItem::iterator it = _cb->_itemList.begin(); it != _cb->_itemList.end(); ++it)
    {
        _cb->_spriteList->RenderNode(scene);
        (*it)->RenderNode(scene);
        scene->ModelMatrix().AddTranslation(0, -_cb->_spriteList->Size()[1], 0);
    }
}

void        ComboBox::ComboBoxUnfoldList::LeaveFocus()
{
    _cb->_listUnrolled = false;
    _cb->_stateChanged = true;
}

