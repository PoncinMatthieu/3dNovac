
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

#ifndef NC_GUI_LOOK_LOOKS_H_
#define NC_GUI_LOOK_LOOKS_H_

#include <Nc/Graphics/Object/Sprite.h>
#include "../Define.h"
#include "StyleSheet.h"

namespace Nc
{
    namespace GUI
    {
        /// Interface of a drawable object used to render the look of a widget.
        /**
            To Create your own Look object, you'll need to redefine the following methods:
                - Clone, to be able to create a copy of the look.
                - Draw, to render the look object.
                - Update, to update the geometry of the look, this will called when the widget size property has changed.
        */
        struct LIB_NC_GUI ILook
        {
			ILook();
			virtual ~ILook();

            /** \return a copy of the look object. */
            virtual ILook   *Clone() const = 0;

            /** Draw the look using the given \p scene. */
            virtual void    Draw(Graphic::SceneGraph *scene) = 0;
            /** Update the look using the given widget \p size. */
            virtual void    Update(const Vector2i &size) = 0;

            Color       color;      ///< color used to render the widget look.
            BoxEdges    edges;      ///< box registering the size of the edges of the widget that the look render.
        };

        /// Define a strip look for a widget.
        /**
            Draw the widget look using three sprite (left/middle/right).

            The sprite of the middle will be stretched depending on the size of the widget.

            It is possible to define partially the sprite. For example you can define only the middle sprite,
            this one will then be drawn with the entire widget size.

            Sprites are loaded using the StyleSheet class and using the following sprite names:
                - constructor name + StyleSheet::LooksName::StripLeft
                - constructor name + StyleSheet::LooksName::StripMiddle
                - constructor name + StyleSheet::LooksName::StripRight

            \sa
                - StyleSheet
        */
        class LIB_NC_GUI StripLook : public ILook
        {
            public:
                StripLook(const std::string &name = "");
                StripLook(const StripLook &l);
                StripLook &operator = (const StripLook &l);
                ~StripLook();

                /** \return a copy of the look object. */
                virtual ILook   *Clone() const                      {return new StripLook(*this);}

                /** Draw the look using the given \p scene. */
                virtual void    Draw(Graphic::SceneGraph *scene);

                /** Update the look using the given widget \p size. */
                virtual void    Update(const Vector2i &size);

            private:
                void    Copy(const StripLook &l);

            public:
                Graphic::Sprite     *spriteLeft;
                Graphic::Sprite     *spriteRight;
                Graphic::Sprite     *spriteMiddle;
        };

        /// Define a Box look for a widget.
        /**
            Draw the widget using a full sprite box:
                - edges (leftEdge, topEdge, rightEdge, bottomEdge)
                - corner (leftTop, rightTop, leftBottom, rightBottom)
                - middle

            The sprites of the middle and edges will be stretched depending on the size of the widget.

            It is possible to define partially the sprite. For example you can define only the middle sprite,
            this one will then be drawn with the entire widget size.

            Sprites are loaded using the StyleSheet class and using the following sprite names:
                - constructor name + StyleSheet::LooksName::BoxLeftEdge
                - constructor name + StyleSheet::LooksName::BoxTopEdge
                - constructor name + StyleSheet::LooksName::BoxRightEdge
                - constructor name + StyleSheet::LooksName::BoxBottomEdge
                - constructor name + StyleSheet::LooksName::BoxLeftTop
                - constructor name + StyleSheet::LooksName::BoxRightTop
                - constructor name + StyleSheet::LooksName::BoxLeftBottom
                - constructor name + StyleSheet::LooksName::BoxRightBottom
                - constructor name + StyleSheet::LooksName::BoxMiddle

            \sa
                - StyleSheet
        */
        class LIB_NC_GUI BoxLook : public ILook
        {
            public:
                BoxLook(const std::string &name = "");
                BoxLook(const BoxLook &l);
                BoxLook &operator = (const BoxLook &l);
                ~BoxLook();

                /** \return a copy of the look object. */
                virtual ILook   *Clone() const                      {return new BoxLook(*this);}

                /** Draw the look using the given \p scene. */
                virtual void    Draw(Graphic::SceneGraph *scene);

                /** Update the look using the given widget \p size. */
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
