
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

    File Created At:        31/01/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------

    Interface to load an image into a System::Image stocked in 32 bit

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_IMAGE_LOADER_PNG_H_
#define NC_GRAPHICS_IMAGE_LOADER_PNG_H_

#include <png.h>
#include "ImageLoader.h"

namespace Nc
{
    namespace Graphic
    {
        namespace Core
        {
            /// Load a png file using the libpng
            /**
                \todo Save method not implemented.
            */
            class LGRAPHICS  ImageLoaderPng : public ImageLoader
            {
                public:
                    ImageLoaderPng();
                    virtual ~ImageLoaderPng();

                    /** Load the png file */
                    virtual void    Load(const Utils::FileName &file, Image &image);
                    /** Save the png file */
                    virtual void    Save(const Utils::FileName &file, Image &image);

                private:
                    /** Initialize the libpng */
                    void            InitPng(const Utils::FileName &file, FILE *fp, unsigned int &width, unsigned int &height, int &bit_depth, int &color_type);

                    png_structp     _pngPtr;
                    png_infop       _infoPtr;
            };
        }
    }
}

#endif

