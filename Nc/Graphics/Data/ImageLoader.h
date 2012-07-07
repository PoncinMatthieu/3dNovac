
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

    File Created At:        31/01/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------

    Interface to load an image into a System::Image stocked in 32 bit

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_DATA_IMAGELOADER_H_
#define NC_GRAPHICS_DATA_IMAGELOADER_H_

#include "../Define.h"
#include "Image.h"

namespace Nc
{
    namespace Graphic
    {
        /// Interface to load an image
        /**
            Actually only the png file format is supported.

            \sa
                - ImageLoaderPng
        */
        class LGRAPHICS  ImageLoader
        {
            public:
                ImageLoader()           {}
                virtual ~ImageLoader()  {}

                /** Load the given image */
                virtual void        Load(const Utils::FileName &file, Image &image) = 0;
                /** Save the given image */
                virtual void        Save(const Utils::FileName &file, Image &image) = 0;
        };
    }
}

#endif

