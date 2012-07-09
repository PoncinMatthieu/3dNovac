
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

#ifndef NC_GRAPHIC_GL_TEXTURE_H_
#define NC_GRAPHIC_GL_TEXTURE_H_

#include "GLObject.h"
#include "../Data/Image.h"

namespace Nc
{
    namespace Graphic
    {
        namespace GL
        {
            ///< provide an opengl texture object.
            class LGRAPHICS Texture : public Object
            {
                public:
                    Texture();
                    /**
                        - \param file specify the texture filename.
                        - \param magnifyingFilter must be GL_NEAREST or GL_LINEAR.
                        - \param mignifyingFilter can be GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR.
                        - \param generateMipmap, if true, generate the mipmaps of the texture.
                    */
                    Texture(const Utils::FileName &file, Enum::Texture::Filter magnifyingFilter = Enum::Texture::Linear, Enum::Texture::Filter mignifyingFilter = Enum::Texture::LinearMipmapLinear, bool generateMipmap = true);
                    virtual ~Texture();

                    /** Create a new copy of the object by without duplicate the ogl ressources. */
                    virtual Object          *Clone() const              {return new Texture(*this);}

                    // Texture Opengl Object manip
                    /** \return the name of the texture. */
                    std::string                     &Name()             {return _name;}
                    /** \return the index of the gl texture. */
                    virtual inline unsigned int		GetIndex() const    {return _texture;}
					/** \return the target of the gl texture. */
					Enum::Texture::Target			GetTarget() const	{return _target;}

                    /** Bind the texture. */
                    virtual void					Enable() const;
                    /** Unbind the texture. */
                    virtual void					Disable() const;

                    // Loading and auto-generation
                    /** Load the texture from a file. */
                    void            LoadFromFile(const Utils::FileName &file, Enum::Texture::Filter magnifyingFilter = Enum::Texture::Linear, Enum::Texture::Filter mignifyingFilter = Enum::Texture::LinearMipmapLinear, bool generateMipmap = true);
                    /** Load the texture from a given Image. */
                    void            LoadFromImage(const Image &image, Enum::Texture::Filter magnifyingFilter = Enum::Texture::Linear, Enum::Texture::Filter mignifyingFilter = Enum::Texture::LinearMipmapLinear, bool generateMipmap = true, const std::string &name = "");
                    /** Load a Cube map (to display a SkyBox). */
                    void            LoadCubeMap(const Utils::FileName Names[6]);
                    /** Generate a 3d sphere map (to create a light map in the DefaultLightingMaterial class). */
                    void            GenereSphereMap(unsigned int diametre);

                    /** Generate an empty ogl texture object. */
                    void            Create(Enum::Texture::Target target);
                    /** Initialize the texture 2d. */
                    void            Init2d(unsigned int mipMapLevel, Enum::Texture::Format internalFormat, const Vector2ui &size, Enum::PixelFormat::Type pixelFormat, Enum::PixelDataType::Type pixelType, const void *pixelData);
                    /** Automatically generate the mimaps of the texture. */
                    void            GenerateMipmaps();

                    // texture properties
                    /** Return the max size of an opengl texture. */
                    static int              MaxSize();
                    /** Return the size of the texture. */
                    inline const Vector2ui  &Size() const               {return _size;}
                    /** Set the no repeat mode to the texture. */
                    void                    AddNoRepeatMode();

                    // current binded textures
                    /** Return true if the texture is currently binded. */
                    inline bool         IsBound()                {return State::Current().CurrentBound(_target) == _texture;}
                    /** \return true if the texture is a texture 2d. */
                    inline bool         Is2d()                  {return (_target == Enum::Texture::Texture2d);}
                    /** \return true if the texture is a cube map texture (used to render skyboxs). */
                    inline bool         IsCubeMap()             {return (_target == Enum::Texture::TextureCubeMap);}
                    /** \return true if the texture is a texture 3d. */
                    inline bool         Is3d()                  {return (_target == Enum::Texture::Texture3d);}

                private:
                    /** Check if the image is correctly dimensioned. */
                    void                CheckSize(const Vector2ui &size);

                    /** Destroy the gl texture object. */
                    virtual void        Release();

                    unsigned int            _texture;                   ///< the gl texture object index.
                    Vector2ui               _size;                      ///< the size of the texture.
                    Enum::Texture::Target   _target;                    ///< the texture target to which the texture is bound.
                    std::string             _name;                      ///< the name of the texture.

                    static int              _maxSize;                   ///< the maximum size of a texture.
            };
        }
    }
}

#endif
