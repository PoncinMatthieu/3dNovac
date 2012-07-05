
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
#include "StyleSheet.h"

using namespace Nc;
using namespace Nc::Graphic;
using namespace Nc::GUI;

const std::string   StyleSheet::LooksName::StripLeft = "StripLeft";
const std::string   StyleSheet::LooksName::StripRight = "StripRight";
const std::string   StyleSheet::LooksName::StripMiddle = "StripMiddle";

const std::string   StyleSheet::LooksName::BoxLeftEdge = "BoxLeftEdge";
const std::string   StyleSheet::LooksName::BoxTopEdge = "BoxTopEdge";
const std::string   StyleSheet::LooksName::BoxRightEdge = "BoxRightEdge";
const std::string   StyleSheet::LooksName::BoxBottomEdge = "BoxBottomEdge";
const std::string   StyleSheet::LooksName::BoxLeftTop = "BoxLeftTop";
const std::string   StyleSheet::LooksName::BoxRightTop = "BoxRightTop";
const std::string   StyleSheet::LooksName::BoxLeftBottom = "BoxLeftBottom";
const std::string   StyleSheet::LooksName::BoxRightBottom = "BoxRightBottom";
const std::string   StyleSheet::LooksName::BoxMiddle = "BoxMiddle";

const std::string   StyleSheet::Name::Button = "Button";
const std::string   StyleSheet::Name::ButtonPressed = "ButtonPressed";
const std::string   StyleSheet::Name::ComboBox = "ComboBox";
const std::string   StyleSheet::Name::ComboBoxList = "ComboBoxList";
const std::string   StyleSheet::Name::LineEdit = "LineEdit";
const std::string   StyleSheet::Name::WindowBox = "Window";
const std::string   StyleSheet::Name::ScrollBarRightButton = "ScrollBarRightButton";
const std::string   StyleSheet::Name::ScrollBarLeftButton = "ScrollBarLeftButton";
const std::string   StyleSheet::Name::ScrollBarSliderBackground = "ScrollBarSliderBackground";
const std::string   StyleSheet::Name::ScrollBarSliderLeft = "ScrollBarSliderLeft";
const std::string   StyleSheet::Name::ScrollBarSliderMiddle = "ScrollBarSliderMiddle";
const std::string   StyleSheet::Name::ScrollBarSliderRight = "ScrollBarSliderRight";
const std::string   StyleSheet::Name::ScrollBarSliderExpand = "ScrollBarSliderExpand";



StyleSheet::StyleSheet()
{
    _spriteSheet = new Core::SpriteSheet(Utils::FileName(CONFIG->Block("GUI")->Line("StyleSheet")->Param("SpriteSheet")));
}

StyleSheet::~StyleSheet()
{
    delete _spriteSheet;
}

Sprite  *StyleSheet::GetNewSprite(const std::string &name)
{
    if (_spriteSheet->SpriteExist(name))
    {
        const Box2i &box = _spriteSheet->SpriteBox(name);
        return new Sprite(Vector2f(box.Length(0), box.Length(1)), _spriteSheet->Texture(), box);
    }
    else
        return NULL;
}
