
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

    File Created At:        15/02/2012
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include <Nc/Graphics/Object/Sprite.h>
#include "WindowStyle.h"

using namespace Nc;
using namespace Nc::Graphic;
using namespace Nc::GUI;

const std::string   WindowStyle::LooksSpriteName::StripLeft = "StripLeft";
const std::string   WindowStyle::LooksSpriteName::StripRight = "StripRight";
const std::string   WindowStyle::LooksSpriteName::StripMiddle = "StripMiddle";

const std::string   WindowStyle::SpriteName::WidgetLeftEdge = "WidgetLeftEdge";
const std::string   WindowStyle::SpriteName::WidgetTopEdge = "WidgetTopEdge";
const std::string   WindowStyle::SpriteName::WidgetRightEdge = "WidgetRightEdge";
const std::string   WindowStyle::SpriteName::WidgetBottomEdge = "WidgetBottomEdge";
const std::string   WindowStyle::SpriteName::WidgetLeftTop = "WidgetLeftTop";
const std::string   WindowStyle::SpriteName::WidgetRightTop = "WidgetRightTop";
const std::string   WindowStyle::SpriteName::WidgetLeftBottom = "WidgetLeftBottom";
const std::string   WindowStyle::SpriteName::WidgetRightBottom = "WidgetRightBottom";
const std::string   WindowStyle::SpriteName::WidgetMiddle = "WidgetMiddle";
const std::string   WindowStyle::SpriteName::Button = "Button";
const std::string   WindowStyle::SpriteName::ComboBoxLeft = "ComboBoxLeft";
const std::string   WindowStyle::SpriteName::ComboBoxRight = "ComboBoxRight";
const std::string   WindowStyle::SpriteName::ComboBoxMiddle = "ComboBoxMiddle";
const std::string   WindowStyle::SpriteName::ComboBoxList = "ComboBoxList";
const std::string   WindowStyle::SpriteName::LineEdit = "LineEdit";
const std::string   WindowStyle::SpriteName::ScrollBarRightButton = "ScrollBarRightButton";
const std::string   WindowStyle::SpriteName::ScrollBarLeftButton = "ScrollBarLeftButton";
const std::string   WindowStyle::SpriteName::ScrollBarSliderBackground = "ScrollBarSliderBackground";
const std::string   WindowStyle::SpriteName::ScrollBarSliderLeft = "ScrollBarSliderLeft";
const std::string   WindowStyle::SpriteName::ScrollBarSliderMiddle = "ScrollBarSliderMiddle";
const std::string   WindowStyle::SpriteName::ScrollBarSliderRight = "ScrollBarSliderRight";
const std::string   WindowStyle::SpriteName::ScrollBarSliderExpand = "ScrollBarSliderExpand";



WindowStyle::WindowStyle()
{
    _spriteSheet = new SpriteSheet(Utils::FileName(CONFIG->Block("GUI")->Line("WindowStyle")->Param("SpriteSheet")));
}

WindowStyle::~WindowStyle()
{
    delete _spriteSheet;
}

Sprite  *WindowStyle::GetNewSprite(const std::string &name)
{
    if (_spriteSheet->SpriteExist(name))
    {
        const Box2i &box = _spriteSheet->SpriteBox(name);
        return new Sprite(Vector2f(box.Length(0), box.Length(1)), _spriteSheet->Texture(), box);
    }
    else
        return NULL;
}
