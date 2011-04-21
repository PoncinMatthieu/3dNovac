
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

#include "../../Core/GL/Texture.h"
#include "Object2d.h"

namespace Nc
{
    namespace Graphic
    {
        /// Provide a way to display sprites
        /**
            A sprite is defined by a size, a texture, a box that delimit the sprite in the texture and finaly a blend pattern
        */
        class LGRAPHICS Sprite : public Object2d
        {
            public:
                Sprite(const Vector2f &size, const GL::Texture &t, const Box2f &textureBox, const GL::Blend::Pattern &blendPattern = GL::Blend::Alpha);
                virtual ~Sprite() {};

                // accessors
                /** Return the texture */
                inline const GL::Texture    &GetTexture() const         {return _drawable.texture;}
                /** Return the box texture */
                inline const Box2f          &TextureBox() const         {return _textureBox;}
                /** Set the box texture */
                inline void                 TextureBox(const Box2f &b)  {_needUpdate = true; _textureBox = b;}
                /** Return the size of the sprite */
                inline const Vector2f       Size() const                {return _spriteSize;}
                /** Set the size of the sprite */
                inline void                 Size(const Vector2f &v)     {_needUpdate = true; _spriteSize = v;}

                /** Render the sprite */
                virtual void Render(ISceneGraph *scene);

            protected:
                /** Update the geometry of the sprite */
                void    UpdateGeometry();

            protected:
                Material<BasicVertexType::Textured2d>                   *_material;     ///< the instance of the rendering material
                Drawable<BasicVertexType::Textured2d, false>            _drawable;      ///< the drawable used to render the sprite

                bool        _needUpdate;        ///< set at true if the geometry needs to be updated
                Vector2f    _spriteSize;        ///< size of the sprite
                Box2f       _textureBox;        ///< the box in the texture
                Color       _color;             ///< color of the vertexs
        };
    }
}

#endif
