
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

#include "Image.h"
#include "ImageLoaderPng.h"

using namespace Nc;
using namespace Nc::Graphic;

Image::Image()
{
}

Image::~Image()
{
}

void    Image::Reverse()
{
    unsigned int pitch = _size.Data[0] * 4;
    Array<Pixel<> > oldPixels(_pixels);    // recopy of the old pixels

    // reverse the old pixels to the actual pixels
    for (unsigned int i = 0; i < _size.Data[1]; ++i)
        memcpy((unsigned char*)(_pixels.Data) + (i * pitch), (unsigned char*)(oldPixels.Data) + ((_size.Data[1] - 1 - i) * pitch), pitch);
}

void    Image::SwapComponants(unsigned char c1, unsigned char c2)
{
    for (unsigned int i = 0; i < _size.Data[1] * _size.Data[1]; ++i)
        Math::Swap(_pixels.Data[i][c1], _pixels.Data[i][c2]);
}

void Image::LoadFromFile(const Utils::FileName &file)
{
    std::string ext = file.Extension();
    Utils::Convert::ToLower(ext);

    if (ext == "png")
    {
        ImageLoaderPng png;
        png.Load(file, *this);
    }
    else
        throw Utils::Exception("Image", file.Fullname() + ": This file format is not supported");
}
