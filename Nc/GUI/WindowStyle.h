
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

#ifndef NC_GUI_WINDOWSTYLE_H_
#define NC_GUI_WINDOWSTYLE_H_

#include <Nc/Graphics/Core/SpriteSheet.h>
#include "Define.h"

namespace Nc
{
    namespace GUI
    {
        /// Singleton to a set of properties related to the GUI. (eg: the sprite sheet of the GUI)
        class LGUI WindowStyle : public Utils::Singleton<WindowStyle>
        {
            public:
                struct SpriteName
                {
                    static const std::string    WidgetLeftEdge;
                    static const std::string    WidgetTopEdge;
                    static const std::string    WidgetRightEdge;
                    static const std::string    WidgetBottomEdge;
                    static const std::string    WidgetLeftTop;
                    static const std::string    WidgetRightTop;
                    static const std::string    WidgetLeftBottom;
                    static const std::string    WidgetRightBottom;
                    static const std::string    WidgetMiddle;
                    static const std::string    Button;
                    static const std::string    ComboBoxLeft;
                    static const std::string    ComboBoxMiddle;
                    static const std::string    ComboBoxRight;
                    static const std::string    ComboBoxList;
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
                WindowStyle();
                ~WindowStyle();

            private:
                Graphic::SpriteSheet    *_spriteSheet;          ///< sprite sheet of the GUI

                friend class Utils::Singleton<WindowStyle>;
        };
    }

    // explicit instantiation
    template class Utils::Singleton<GUI::WindowStyle>;
}

#endif
