
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

    File Created At:        29/01/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------

    Class image to manage a data image structure
    images are stocked in 32 bit

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_IMAGE_H_
#define NC_GRAPHICS_IMAGE_H_

#include <Nc/Core/Utils/FileName.h>
#include <Nc/Core/Math/Math.h>
#include "../Define.h"

namespace Nc
{
    namespace Graphic
    {
        class LGRAPHICS  Image
        {
            public:
                Image();
                ~Image();

                inline void     InitSize(const Vector2ui &size)                                 {_size = size; _pixels.InitSize(size.Data[0] * size.Data[1]);}
                inline void     Resize(const Vector2ui &size)                                   {_size = size; _pixels.Resize(size.Data[0] * size.Data[1]);}

                /// Loading
                void            LoadFromFile(const Utils::FileName &file);

                /// transform
                void            Reverse();
                void            SwapComponants(unsigned char c1, unsigned char c2);

                /// accessor
                inline void                 SetPixel(const Vector2ui &pos, const Pixel<> &p)      {_pixels[(pos.Data[1] * _size.Data[0]) + pos.Data[0]] = p;}
                inline const Pixel<>        &GetPixel(const Vector2ui &pos) const               {return _pixels[(pos.Data[1] * _size.Data[0]) + pos.Data[0]];}
                inline const unsigned char  *GetPixels() const                                  {return reinterpret_cast<const unsigned char*>(_pixels.Data);}
                inline Pixel<>              *GetPixelsArray()                                   {return _pixels.Data;}
                inline const Vector2ui      &Size() const                                       {return _size;}

            private:
                Vector2ui           _size;
                Array<Pixel<> >     _pixels;

                friend class ImageLoaderPng;
        };
    }
}

#endif
