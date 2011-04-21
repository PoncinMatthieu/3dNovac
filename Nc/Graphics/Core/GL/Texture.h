
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

    File Created At:        15/11/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHIC_CORE_GL_TEXTURE_H_
#define NC_GRAPHIC_CORE_GL_TEXTURE_H_

#include "GLObject.h"
#include "../Image.h"

namespace Nc
{
    namespace Graphic
    {
        namespace GL
        {
            ///< provide an opengl texture object
            class LGRAPHICS Texture : public Object
            {
                public:
                    Texture();
                    Texture(const Utils::FileName &file, bool useMipMap = true);
                    virtual ~Texture();

                    // Texture Opengl Object manip
                    /** Return the name of the texture */
                    std::string                     &Name()             {return _name;}
                    /** Return the index of the gl texture */
                    virtual inline unsigned int		GetIndex() const    {return _texture;}
                    /** Bind the texture */
                    virtual void					Enable() const;
                    /** Unbind the texture */
                    virtual void					Disable() const;

                    // Loading and auto-generation
                    /** Load the texture from a file */
                    void            LoadFromFile(const Utils::FileName &file, bool useMipMap = true);
                    /** Load the texture from a given Image */
                    void            LoadFromImage(const Image &image, bool useMipMap = true, const std::string &name = "");
                    /** Load a Cube map (to display a SkyBox) */
                    void            LoadCubeMap(const Utils::FileName Names[6]);
                    /** Generate a 3d sphere map (to create a light map in the DefaultLightingMaterialPolitic class) */
                    void            GenereSphereMap(unsigned int diametre);

                    // texture properties
                    /** Return the max size of an opengl texture */
                    static int              MaxSize();
                    /** Return the size of the texture */
                    inline const Vector2ui  &Size() const               {return _size;}
                    /** Set the no repeat mode to the texture */
                    void                    AddNoRepeatMode();

                    // current binded textures
                    /** Return true if the texture is currently binded */
                    inline bool         IsBind()                {return _currentBind == _texture;}
                    /** Reset the current bind texture statement */
                    static inline void  ResetCurrentBind()      {_currentBind = 0;}

                private:
                    /** Check if the image is correctly dimensioned */
                    void                CheckImage(const Image &image);

                    /** Destroy the gl texture object */
                    virtual  void       Release();

                    unsigned int            _texture;           ///< the gl texture object index
                    Vector2ui               _size;              ///< the size of the texture
                    int                     _type;              ///< the texture type
                    std::string             _name;              ///< the name of the texture

                    static unsigned int     _currentBind;       ///< the current bind texture
                    static int              _maxSize;           ///< the maximum size of a texture
            };
        }
    }
}

#endif
