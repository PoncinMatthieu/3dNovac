
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

    File Created At:        06/09/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "Texture.h"

using namespace std;
using namespace Nc;
using namespace Nc::Graphic;
using namespace Nc::Graphic::GL;

int             Texture::_maxSize = -1;

Texture::Texture()
    : Object(), _texture(0)
{
}

Texture::Texture(const Utils::FileName &file, Enum::Texture::Filter magnifyingFilter, Enum::Texture::Filter mignifyingFilter, bool generateMipmap)
    : Object(), _texture(0)
{
    LoadFromFile(file, magnifyingFilter, mignifyingFilter, generateMipmap);
}

Texture::~Texture()
{
    ReleaseRef();
}

void Texture::Release()
{
    CALLSTACK_INFO_ARG("texture: " + Utils::Convert::ToString(_texture));
    State::Instance().Unbind(_target);
    glDeleteTextures(1, &_texture);
    _texture = 0;
    NC_GRAPHIC_GL_CHECK_ERROR();
}

void  Texture::Enable() const
{
    if (_target >= 0)
    {
        State::Instance().Bind(_target, _texture);
    }
}

void  Texture::Disable() const
{
    // since OpenGL version 3.3, unbind a texture is most of the time unnecessary because we render the texture using shaders.
    // However, when rendering with multitexture, we bind texture to different texture units.
    // --> The textures must be disabled in texture unit it is used before being able to use it in another texture unit.

    // to improve performance, we must be able to remove the call to the "Disable" method in many places.
    State::Instance().Unbind(_target);
}

int Texture::MaxSize()
{
    if (_maxSize == -1)
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &_maxSize);
    return _maxSize;
}

void Texture::CheckSize(const Vector2ui &size)
{
    int maxSize = MaxSize();
    if ((size.data[0] > static_cast<unsigned int>(maxSize)) || (size.data[1] > static_cast<unsigned int>(maxSize)))
        throw Utils::Exception("Texture", "Failed to create texture, internal size too high (" +
                                Utils::Convert::ToString(size.data[0]) + "x" + Utils::Convert::ToString(size.data[1]) +
                                "), maxLargerSize:" + Utils::Convert::ToString(maxSize));

    if (!Math::IsAPowerOf2(size.data[0]) || !Math::IsAPowerOf2(size.data[1]))
    {
        if (EXT.NonPowerOf2Supported())
            LOG_DEBUG << "Warning: GL::Texture: To increse performance, please use a texture size of power of 2. TextureId:" << _texture << " Size:" << size.data[0] << "/" << size.data[1] << std::endl;
        else
            throw Utils::Exception("Texture", "Can't create the texture: Texturing with a non power of two size is not supported on this plateform.");
    }
}

void    Texture::Create(Enum::Texture::Target target)
{
    CALLSTACK_INFO();
    NewRef();
    _target = target;
    glGenTextures(1, &_texture);
    CALLSTACK_APPEND_INFO(Utils::Convert::ToString(_texture));
    NC_GRAPHIC_GL_CHECK_ERROR();
    if (_texture == 0)
        throw Utils::Exception("Texture", "Failed to generate the texture unit");
}

void    Texture::Init2d(unsigned int level, Enum::Texture::Format internalFormat, const Vector2ui &size, Enum::PixelFormat::Type pixelFormat, Enum::PixelDataType::Type pixelType, const void *pixelData)
{
    CALLSTACK_INFO();
    if (State::Instance().CurrentBound(_target) != _texture)
        throw Utils::Exception("Texture::InitTexture2d", "Can't init the texture which is not enabled.");
    CheckSize(size);
    glTexImage2D(_target, level, internalFormat, size.data[0], size.data[1], 0, pixelFormat, pixelType, pixelData);
    _size = size;
    NC_GRAPHIC_GL_CHECK_ERROR();
}

void    Texture::GenerateMipmaps()
{
    CALLSTACK_INFO();
    if (State::Instance().CurrentBound(_target) != _texture)
        throw Utils::Exception("Texture::GenerateMipmap", "Can't generate the texture mipmaps if the texture is not enabled.");
    glGenerateMipmap(_target);
    NC_GRAPHIC_GL_CHECK_ERROR();
}

void Texture::LoadFromFile(const Utils::FileName &file, Enum::Texture::Filter magnifyingFilter, Enum::Texture::Filter mignifyingFilter, bool generateMipmap)
{
    CALLSTACK_INFO_ARG("filename: " + file);

// Load the image and reverse it
    Image image;
    image.LoadFromFile(file);
    image.Reverse();

    LoadFromImage(image, magnifyingFilter, mignifyingFilter, generateMipmap);
}

void Texture::LoadFromImage(const Image &image, Enum::Texture::Filter magnifyingFilter, Enum::Texture::Filter mignifyingFilter, bool generateMipmap)
{
    CALLSTACK_INFO();
    NewRef();

// Load the image and reverse it
    CheckSize(image.Size());  // check the image
    _size = image.Size();
    const unsigned char *pixels = image.GetPixels();

// Create the OpenGL texture
    _target = Enum::Texture::Texture2d;
    glGenTextures(1, &_texture);
    CALLSTACK_APPEND_INFO(Utils::Convert::ToString(_texture));
    if (_texture == 0)
        throw Utils::Exception("Texture", "Failed to generate the texture unit");
    Enable();

    glTexParameteri(_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(_target, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, magnifyingFilter);
    glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, mignifyingFilter);
    //glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, (generateMipmap) ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);

    glTexImage2D(_target, 0, GL_RGBA8, image.Size().data[0], image.Size().data[1], 0, GL_RGBA, Enum::UnsignedByte, pixels);
    NC_GRAPHIC_GL_CHECK_ERROR();

    if(generateMipmap)
        glGenerateMipmap(_target);

    Disable();
}

void Texture::SaveToImage(Image &image)
{
    CALLSTACK_INFO();

    if (_texture == 0)
        throw Utils::Exception("Texture", "The texture isn't created yet.");

    image.InitSize(_size);

    Enable();
    glGetTexImage(_target, 0, GL_RGBA, Enum::UnsignedByte, image.GetPixels());
    Disable();
}

void Texture::SaveToFile(const Utils::FileName &file)
{
    CALLSTACK_INFO();

    // retreive the data
    Image img;
    SaveToImage(img);

    // reverse the datas
    img.Reverse();

    // save the image
    img.SaveToFile(file);
}

void Texture::LoadCubeMap(const Utils::FileName names[6])
{
    CALLSTACK_INFO();
    NewRef();

    Image image;
    GLenum target[6] =
    {
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
        GL_TEXTURE_CUBE_MAP_POSITIVE_X,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z
    };

    // Génération d'une texture CubeMap
    _target = Enum::Texture::TextureCubeMap;
    glGenTextures(1, &_texture);
    if (_texture == 0)
        throw Utils::Exception("Texture", "Failed to generate the texture unit");

    CALLSTACK_APPEND_INFO(Utils::Convert::ToString(_texture));

    // Configuration de la texture
    Enable();
    glTexParameteri(_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(_target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    for (int i = 0; i < 6; i++)
    {
        image.LoadFromFile(names[i]);
        CheckSize(image.Size());  // check the image
        image.Reverse(); // and reverse it
        const unsigned char *pixels = image.GetPixels();

        glTexImage2D(target[i], 0, GL_RGBA8, image.Size().data[0], image.Size().data[1], 0, GL_RGBA, Enum::UnsignedByte, pixels);
    }
    NC_GRAPHIC_GL_CHECK_ERROR();
    Disable();
}

void Texture::AddNoRepeatMode()
{
    CALLSTACK_INFO();
    Enable();
    glTexParameteri(_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    NC_GRAPHIC_GL_CHECK_ERROR();
}

void Texture::GenereSphereMap(const unsigned int d)
{
    CALLSTACK_INFO_ARG("d: " + Utils::Convert::ToString(d));
    NewRef();

    float           *data;
    unsigned int    sizeTotal = d * d * d * 3;
    float           length;
    float           radius = ((float)d)/2.0f;

    if (d < 1)
        throw Utils::Exception("Texture", "Can't gernerate a SphereMap with a raduis lower than 1");

    // creation des donnees de la texture
    data = new float[sizeTotal];
    memset(data, 0, sizeTotal * sizeof(float)); // reset la map de noir
    for (float i = 0; i < d; i++) // remplit la sphere
    {
        for (float j = 0; j < d; j++)
        {
            for (float k = 0; k < d; k++)
            {
                length = ((radius-1) - sqrt(pow(i-radius, 2) + pow(j-radius, 2) + pow(k-radius, 2)));
                length /= radius;

                int offset = (i*d*d*3) + (j*d*3) + (k*3);
                data[offset] = length;
                data[offset + 1] = length;
                data[offset + 2] = length;
            }
        }
    }
    // creation de la texture
    _target = Enum::Texture::Texture3d;
    glGenTextures(1, &_texture);
    if (_texture == 0)
        throw Utils::Exception("Texture", "Failed to generate the texture unit");

    CALLSTACK_APPEND_INFO(Utils::Convert::ToString(_texture));

    Enable();
	glTexParameteri(_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(_target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // deprecated
	glTexImage3D(_target, 0, GL_RGB8, d, d, d, 0, GL_RGB, Enum::Float, data);
    NC_GRAPHIC_GL_CHECK_ERROR();
    Disable();
    delete[] data;
}

/*
unsigned int    GLContext::GenereCubeMapNormalize(const unsigned int size)
{
    unsigned int    textureFinal;
	GLubyte         *pixels = new GLubyte[size*size*3];
	float           vector[3];

    // activation et creation de la cube map
    glEnable(Enum::TextureCubeMap);
	glGenTextures(1, &textureFinal);
	State::Instance().Bind(Enum::TextureCubeMap, textureFinal);

	// on configure la texture et utilise un filtrage lineaire pour ne pas avoir a generer les mipmaps.
	glTexParameteri(Enum::TextureCubeMap, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(Enum::TextureCubeMap, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(Enum::TextureCubeMap, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(Enum::TextureCubeMap, GL_TEXTURE_MIN_FILTER,GL_LINEAR);

	for (int i = 0; i < 6; i++)	    // Pour chaque face
	{
		for (int y = 0; y < size; y++)
			for (int x = 0; x < size; x++)
			{
				getCubeVector(i, size, x, y, vector);
				pixels[3*(y*size+x) + 0] = 128 - 127*vector[0];
				pixels[3*(y*size+x) + 1] = 128 - 127*vector[1];
				pixels[3*(y*size+x) + 2] = 128 - 127*vector[2];
			}
		// on envoi le buffer de texture à OpenGL
		gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,GL_RGB8,size,size,GL_RGB,Enum::UnsignedByte,pixels);
	}

	delete [] pixels;
	glDisable(Enum::TextureCubeMap);
}*/
