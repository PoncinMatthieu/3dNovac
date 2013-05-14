
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

#ifndef NC_GRAPHICS_DATA_IMAGE_H_
#define NC_GRAPHICS_DATA_IMAGE_H_

#include <Nc/Core/Utils/FileName.h>
#include <Nc/Core/Math/Math.h>
#include "../Define.h"

namespace Nc
{
    namespace Graphic
    {
        /// To Load and manage image.
        /**
            The images is stored in RGBA mode with 32 bit per pixel.

            Images are loaded by the `ImageLoader` class.

            \sa
                - ImageLoader
                - ImageLoaderPng
        */
        class LIB_NC_GRAPHICS  Image
        {
            public:
                Image();
                ~Image();

                /** Init the size of the image, /!\ the data is lost. */
                inline void     InitSize(const Vector2ui &size)                                 {_size = size; _pixels.InitSize(size.data[0] * size.data[1]);}
                /** Resize the image. */
                inline void     Resize(const Vector2ui &size)                                   {_size = size; _pixels.Resize(size.data[0] * size.data[1]);}

                // Loading
                /** Load an image from a file. */
                void            LoadFromFile(const Utils::FileName &file);

                // transform
                /** Reverse the image, usefull to create an OpenGL Texture. */
                void            Reverse();
                /** Swap two componant of the image, usefull to create a BGRA format or other. */
                void            SwapComponants(unsigned char c1, unsigned char c2);

                // accessors
                /** Set the pixel at the given position. */
                inline void                 SetPixel(const Vector2ui &pos, const Pixel<> &p)            {SetPixel(pos.data[0], pos.data[1], p);}
                /** Set the pixel at the given position. */
                inline void                 SetPixel(unsigned int x, unsigned int y, const Pixel<> &p)  {_pixels[(y * _size.data[0]) + x] = p;}
                /** \return the pixel from the given position. */
                inline const Pixel<>        &GetPixel(const Vector2ui &pos) const               {return GetPixel(pos.data[0], pos.data[1]);}
                /** \return the pixel from the given position. */
                inline const Pixel<>        &GetPixel(unsigned int x, unsigned int y) const     {return _pixels[(y * _size.data[0]) + x];}
                /** \return the pixels in unsigned char*. */
                inline const unsigned char  *GetPixels() const                                  {return reinterpret_cast<const unsigned char*>(_pixels.data);}
                /** \return the array of pixels. */
                inline Pixel<>              *GetPixelsArray()                                   {return _pixels.data;}
                /** \return the size of the image. */
                inline const Vector2ui      &Size() const                                       {return _size;}

            private:
                Vector2ui           _size;          ///< the size of the image.
                Array<Pixel<> >     _pixels;        ///< the array of pixels.

                friend class ImageLoaderPng;
        };
    }
}

#endif
