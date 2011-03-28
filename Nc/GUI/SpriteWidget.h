
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
/*-----------------------------------------------------------------------------


                    Implementation de la classe "gMinimap"
                        permet l'affichage d'une minimap

Herite de gWindowBox

-----------------------------------------------------------------------------*/

#ifndef NOVAC_GRAPHIC_GUI_SPRITE_WIDGET_H_
#define NOVAC_GRAPHIC_GUI_SPRITE_WIDGET_H_

#include <Nc/Graphics/Object/2D/Sprite.h>
#include "Widget.h"

namespace Nc
{
    namespace GUI
    {
        class LGUI  SpriteWidget : public Widget
        {
            public:
                struct SpritePos
                {
                    SpritePos(Graphic::Sprite *s, const Vector2f &p) : sprite(s), pos(p)     {}

                    Graphic::Sprite     *sprite;
                    Vector2f            pos;
                };

                typedef std::list<SpritePos>  ListSpritePos;

            public:
                SpriteWidget(const Vector2f &pos = Vector2f(0, 0), const Vector2f &size = Vector2f(10, 10), Corner x = Top, Corner y = Left, Widget *parent = NULL);
                virtual ~SpriteWidget();

                SpriteWidget(const SpriteWidget &w);
                SpriteWidget &operator = (const SpriteWidget &w);
                virtual void    Copy(const SpriteWidget &w);
                virtual Widget* Clone() const                   {return new SpriteWidget(*this);}

                inline void             ClearSprites()              {_listSprite.clear(); _stateChange = true;}
                void                    AddSprite(Graphic::Sprite *s, const Vector2f &pos);
                inline SpritePos        PopBackSprite()            {SpritePos s = _listSprite.back(); _listSprite.pop_back(); _stateChange = true; return s;}
                inline SpritePos        PopFrontSprite()           {SpritePos s = _listSprite.front(); _listSprite.pop_front(); _stateChange = true; return s;}

//                    virtual Vector2f    GetReelSize() const; //TODO
//                    virtual Vector2f    GetReelPos() const; //TODO

            protected:
                virtual void Draw(Graphic::ISceneGraph *scene);

            private:
                ListSpritePos      _listSprite;
        };
    }
}

#endif
