
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

    File Created At:        16/04/2012
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GUI_LOOKS_H_
#define NC_GUI_LOOKS_H_

#include "Define.h"

namespace Nc
{
    namespace GUI
    {
        /// Define the interface of a drawable object used to render the look of a widget
        struct LGUI ILook
        {
			ILook();
			virtual ~ILook();

            virtual ILook   *Clone() const = 0;

            virtual void    Draw(Graphic::SceneGraph *scene) = 0;
            virtual void    Update(const Vector2i &size) = 0;

            Color   color;
        };

        class LGUI StripLook : public ILook
        {
            public:
                StripLook(const std::string &name = "");
                StripLook(const StripLook &l);
                StripLook &operator = (const StripLook &l);
                ~StripLook();

                virtual ILook   *Clone() const                      {return new StripLook(*this);}

                virtual void    Draw(Graphic::SceneGraph *scene);
                virtual void    Update(const Vector2i &size);

            private:
                void    Copy(const StripLook &l);

            public:
                Graphic::Sprite     *spriteLeft;
                Graphic::Sprite     *spriteRight;
                Graphic::Sprite     *spriteMiddle;
        };

        class LGUI BoxLook : public ILook
        {
            public:
                BoxLook(const std::string &name = "");
                BoxLook(const BoxLook &l);
                BoxLook &operator = (const BoxLook &l);
                ~BoxLook();

                virtual ILook   *Clone() const                      {return new BoxLook(*this);}

                virtual void    Draw(Graphic::SceneGraph *scene);
                virtual void    Update(const Vector2i &size);


            private:
                void    Copy(const BoxLook &l);

            public:
                Graphic::Sprite     *spriteLeftEdge;
                Graphic::Sprite     *spriteTopEdge;
                Graphic::Sprite     *spriteRightEdge;
                Graphic::Sprite     *spriteBottomEdge;
                Graphic::Sprite     *spriteLeftTop;
                Graphic::Sprite     *spriteRightTop;
                Graphic::Sprite     *spriteLeftBottom;
                Graphic::Sprite     *spriteRightBottom;
                Graphic::Sprite     *spriteMiddle;
        };
    }
}

#endif
