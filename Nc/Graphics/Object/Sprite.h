
/*-----------------------------------------------------------------------------

	3dNovac Graphics
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

    File Created At:        22/12/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_2D_SPRITE_H_
#define NC_GRAPHICS_2D_SPRITE_H_

#include "../GL/Texture.h"
#include "Object.h"

namespace Nc
{
    namespace Graphic
    {
        /// Provide a way to display sprites.
        /**
            A sprite is defined by a size, a texture, a box that delimit the sprite in the texture and finaly a blend pattern.
        */
        class LGRAPHICS Sprite : public Object
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_VISITABLE(Graphic::Object, System::Object, Nc::Graphic::Sprite);

            public:
                Sprite(const Vector2f &size, const GL::Texture &t, const Box2i &textureBox, const GL::Blend::Pattern &blendPattern = GL::Blend::Alpha);
                /** Create the box of the sprite by using a percent value of the texture size. by using 100.f, you'll get the whole texture size as the box. */
                Sprite(const Vector2f &size, const GL::Texture &t, float percentBox, const GL::Blend::Pattern &blendPattern = GL::Blend::Alpha);
                /** Create the sprite by using the whole texture and using the size of the texture. */
                Sprite(const GL::Texture &t, const GL::Blend::Pattern &blendPattern = GL::Blend::Alpha);
                virtual ~Sprite() {};

                virtual ISceneNode          *Clone() const              {return new Sprite(*this);}

                // accessors
                /** Set the texture. */
                inline const void           SetTexture(const GL::Texture &t)    {_drawables[0]->Config->Textures[0] = t; _needUpdate = true;}
                /** \return the texture. */
                inline const GL::Texture    &GetTexture() const         {return _drawables[0]->Config->Textures[0];}
                /** \return the texture. */
                inline GL::Texture          &GetTexture()               {return _drawables[0]->Config->Textures[0];}
                /** \return the box texture. */
                inline const Box2i          &TextureBox() const         {return _textureBox;}
                /** Set the box texture. */
                inline void                 TextureBox(const Box2i &b)  {_textureBox = b; _needUpdate = true;}
                /** \return the size of the sprite. */
                inline const Vector2f       Size() const                {return _spriteSize;}
                /** Set the size of the sprite. */
                inline void                 Size(const Vector2f &v)     {_spriteSize = v; _needUpdate = true;}
                /** \return the color used to render the sprite. */
                inline const Color          &GetColor() const           {return _color;}
                /** Set the color used to render the sprite. */
                inline void                 SetColor(const Color &color) {_color = color; _needUpdate = true;}

            protected:
                /** Render the sprite. */
                virtual void                Draw(SceneGraph *scene);

                /** Update the geometry of the sprite. */
                void                        UpdateGeometry();

            private:
                void                        Initialize(const GL::Texture &t, const GL::Blend::Pattern &blendPattern);

            protected:
                bool        _needUpdate;        ///< set at true if the geometry needs to be updated.
                Vector2f    _spriteSize;        ///< size of the sprite.
                Box2i       _textureBox;        ///< the box in the texture.
                Color       _color;             ///< color of the vertexs.
        };
    }
}

#endif
