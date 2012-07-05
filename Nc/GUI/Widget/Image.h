
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

    File Created At:        10/06/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GUI_WIDGET_IMAGE_H_
#define NC_GUI_WIDGET_IMAGE_H_

#include <Nc/Graphics/Object/Sprite.h>
#include "Widget.h"

namespace Nc
{
    namespace GUI
    {
        /// To manage a widget that contain a sprite
        class LGUI  Image : public Widget
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_VISITABLE(Widget, System::Object, Nc::GUI::Image);

            public:
                Image(const AlignmentMask &alignment = Left | Top, const Vector2i &size = Vector2i(0, 0));
                virtual ~Image();

                virtual ISceneNode  *Clone() const                  {return new Image(*this);}

                /** \return the sprite of the widget. */
                Graphic::Sprite     *Sprite() const                 {return _sprite;}
                /** Set the sprite of the widget. */
                void                Sprite(Graphic::Sprite *sp);

            protected:
                /** Update the geometry of the image. */
                virtual void    Update();
                /** Render the sprite list. */
                virtual void    Draw(Graphic::SceneGraph *scene);

                /** \return a vector to translate the childs when we call the RelativePos method. */
                virtual void    PosChild(const Widget *, Vector2i &) const;
                /** \return the size that the child should use to set up it's percent size when we call the Resized method. */
                virtual void    SizeChild(const Widget *, Vector2i &size) const;

            private:
                Graphic::Sprite     *_sprite;           ///< the sprite used to render the image.
                AlignmentMask       _spriteAlignment;   ///< alignment of the sprite inside the widget.
                bool                _spriteResizable;   ///< if true, we will try to resize the sprite to the size of the widget.
                bool                _keepRatio;         ///< if true, we will keep the initial ratio of the sprite while resizing it.
                System::Mutex       _spriteMutex;       ///< protect the sprite acess.
        };
    }
}

#endif
