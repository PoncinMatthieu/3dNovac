
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

#ifndef NC_GUI_SPRITE_WIDGET_H_
#define NC_GUI_SPRITE_WIDGET_H_

#include <Nc/Graphics/Object/2D/Sprite.h>
#include "Widget.h"

namespace Nc
{
    namespace GUI
    {
        /// To manage a widget that contain a list of sprites
        class LGUI  SpriteWidget : public Widget
        {
            public:
                /// To define a sprite with his position in the widget
                struct SpritePos
                {
                    SpritePos(Graphic::Sprite *s, const Vector2f &p) : sprite(s), pos(p)     {}

                    Graphic::Sprite     *sprite;
                    Vector2f            pos;
                };

                typedef std::list<SpritePos>  ListSpritePos;    ///< Define a list of sprite/position

            public:
                SpriteWidget(const Vector2f &pos = Vector2f(0, 0), const Vector2f &size = Vector2f(10, 10), Corner x = Top, Corner y = Left, Widget *parent = NULL);
                virtual ~SpriteWidget();

                SpriteWidget(const SpriteWidget &w);
                SpriteWidget &operator = (const SpriteWidget &w);
                virtual void    Copy(const SpriteWidget &w);
                virtual Widget* Clone() const                   {return new SpriteWidget(*this);}

                /** clear the list of sprites */
                inline void             ClearSprites()              {_listSprite.clear(); _stateChange = true;}
                /** Add a sprite in the list with his position */
                void                    AddSprite(Graphic::Sprite *s, const Vector2f &pos);
                /** Unstack a sprite and return it */
                inline SpritePos        PopBackSprite()            {SpritePos s = _listSprite.back(); _listSprite.pop_back(); _stateChange = true; return s;}
                /** Unstack a sprite and return it */
                inline SpritePos        PopFrontSprite()           {SpritePos s = _listSprite.front(); _listSprite.pop_front(); _stateChange = true; return s;}

//                    virtual Vector2f    GetReelSize() const; ///\todo
//                    virtual Vector2f    GetReelPos() const; ///\todo

            protected:
                /** Render the sprite list */
                virtual void Draw(Graphic::ISceneGraph *scene);

            private:
                ListSpritePos      _listSprite;         ///< The list of SpritePos
        };
    }
}

#endif
