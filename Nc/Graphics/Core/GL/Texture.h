
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
/*-----------------------------------------------------------------------------

    Provide a GLTexture

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
            class LGRAPHICS Texture : public Object
            {
                public:
                    Texture();
                    Texture(const Utils::FileName &file, bool useMipMap = true);
                    virtual ~Texture();

                    /// Texture Opengl Object manip
                    virtual inline unsigned int		GetIndex() const    {return _texture;}
                    virtual void					Enable() const;
                    virtual void					Disable() const;

                    /// Loading and auto-generation
                    void            LoadFromFile(const Utils::FileName &file, bool useMipMap = true);
                    void            LoadFromImage(const Image &image, bool useMipMap = true, const std::string &name = "");
                    void            LoadCubeMap(const Utils::FileName Names[6]);
                    void            GenereSphereMap(unsigned int diametre);
                    void            GenereCubeMapNormalize(unsigned int diametre); // TODO

                    /// texture properties
                    static int              MaxSize();
                    inline const Vector2ui  &Size() const               {return _size;}
                    void                    AddNoRepeatMode();

                    /// current binded textures
                    inline bool         IsBind()                {return _currentBind == _texture;}
                    static inline void  ResetCurrentType()      {_currentType = 0;}
                    static inline void  ResetCurrentBind()      {_currentBind = 0;}

                private:
                    void                CheckImage(const Image &image);
                    virtual  void       Release();

                    unsigned int            _texture;
                    Vector2ui               _size;
                    int                     _type;
                    static unsigned int     _currentBind;
                    static int              _currentType;
                    static int              _maxSize;

                    std::string             _name;
            };
        }
    }
}

#endif
