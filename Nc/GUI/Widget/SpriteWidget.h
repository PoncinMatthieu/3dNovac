
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

#ifndef NC_GUI_WIDGET_SPRITE_WIDGET_H_
#define NC_GUI_WIDGET_SPRITE_WIDGET_H_

#include <Nc/Graphics/Object/Sprite.h>
#include "Widget.h"

namespace Nc
{
    namespace GUI
    {
        /// To manage a widget that contain a list of sprites
        class LGUI  SpriteWidget : public Widget
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_VISITABLE(Widget, System::Object, Nc::GUI::SpriteWidget);

                /// To define a sprite with his position in the widget
                struct SpritePos
                {
                    SpritePos(Graphic::Sprite *s, const Vector2i &p) : sprite(s), pos(p)     {}

                    Graphic::Sprite     *sprite;
                    Vector2i            pos;
                };

                typedef std::list<SpritePos>  ListSpritePos;    ///< Define a list of sprite/position

            public:
                SpriteWidget(const AlignmentMask &alignment = Left | Top, const Vector2i &size = Vector2i(0, 0));
                virtual ~SpriteWidget();

                virtual ISceneNode  *Clone() const                  {return new SpriteWidget(*this);}

                /** clear the list of sprites */
                inline void             ClearSprites()              {_listSprite.clear(); _stateChanged = true;}
                /** Add a sprite in the list with his position */
                void                    AddSprite(Graphic::Sprite *s, const Vector2i &pos);
                /** Unstack a sprite and return it */
                inline SpritePos        PopBackSprite()            {SpritePos s = _listSprite.back(); _listSprite.pop_back(); _stateChanged = true; return s;}
                /** Unstack a sprite and return it */
                inline SpritePos        PopFrontSprite()           {SpritePos s = _listSprite.front(); _listSprite.pop_front(); _stateChanged = true; return s;}

            protected:
                /** Render the sprite list */
                virtual void Draw(Graphic::SceneGraph *scene);

            private:
                ListSpritePos      _listSprite;         ///< The list of SpritePos
        };
    }
}

#endif
