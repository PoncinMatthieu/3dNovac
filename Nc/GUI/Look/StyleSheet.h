
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

    File Created At:        14/02/2012
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GUI_LOOK_STYLESHEET_H_
#define NC_GUI_LOOK_STYLESHEET_H_

#include <Nc/Graphics/Core/SpriteSheet.h>
#include "../Define.h"

namespace Nc
{
    namespace GUI
    {
        /// Singleton to a set of properties related to the GUI. (eg: the sprite sheet of the GUI)
        class LGUI StyleSheet : public Utils::Singleton<StyleSheet>
        {
            public:
                struct LooksName
                {
                    static const std::string    StripLeft;
                    static const std::string    StripRight;
                    static const std::string    StripMiddle;

                    static const std::string    BoxLeftEdge;
                    static const std::string    BoxTopEdge;
                    static const std::string    BoxRightEdge;
                    static const std::string    BoxBottomEdge;
                    static const std::string    BoxLeftTop;
                    static const std::string    BoxRightTop;
                    static const std::string    BoxLeftBottom;
                    static const std::string    BoxRightBottom;
                    static const std::string    BoxMiddle;
                };

                struct Name
                {
                    static const std::string    Button;
                    static const std::string    ButtonPressed;
                    static const std::string    ComboBox;
                    static const std::string    ComboBoxList;
                    static const std::string    LineEdit;
                    static const std::string    WindowBox;
                    static const std::string    ScrollBarRightButton;
                    static const std::string    ScrollBarLeftButton;
                    static const std::string    ScrollBarSliderBackground;
                    static const std::string    ScrollBarSliderLeft;
                    static const std::string    ScrollBarSliderMiddle;
                    static const std::string    ScrollBarSliderRight;
                    static const std::string    ScrollBarSliderExpand;
                };

            public:
                /** \return a new sprite of the associated sprite recorded into the sprite sheet */
                Graphic::Sprite     *GetNewSprite(const std::string &name);

            protected:
                StyleSheet();
                ~StyleSheet();

            private:
                Graphic::SpriteSheet    *_spriteSheet;          ///< sprite sheet of the GUI

                friend class Utils::Singleton<StyleSheet>;
        };
    }

    // explicit instantiation
    template class Utils::Singleton<GUI::StyleSheet>;
}

#endif
