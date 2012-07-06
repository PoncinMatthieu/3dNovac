
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

#include "ImageLoaderPng.h"

using namespace Nc;
using namespace Nc::Graphic::Core;

ImageLoaderPng::ImageLoaderPng()
{
    // create the png structures
    _pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, png_voidp_NULL, png_error_ptr_NULL, png_error_ptr_NULL);
    if (!_pngPtr)
        throw Utils::Exception("Image", "pnglib: Can't create the read struct");
    _infoPtr = png_create_info_struct(_pngPtr);
    if (!_infoPtr)
        throw Utils::Exception("Image", "pnglib: Can't create the info struct");

    if (setjmp(png_jmpbuf(_pngPtr)))
        throw Utils::Exception("Image", "pnglib: setJump failed");
}

ImageLoaderPng::~ImageLoaderPng()
{
    png_destroy_read_struct(&_pngPtr, &_infoPtr, (png_infopp)NULL);
}

void ImageLoaderPng::Load(const Utils::FileName &file, Image &image)
{
	// open the file
	std::FILE *fp = fopen(file.c_str(), "rb");
    if (!fp)
        throw Utils::Exception("Image", "Can't open the file `" + file + "`");

    // init png lib
    int bit_depth, color_type;
    InitPng(file, fp, image._size.data[0], image._size.data[1], bit_depth, color_type);

    // expand palette images to RGB, low-bit-depth grayscale images to 8 bits,
    // transparency chunks to full alpha channel; strip 16-bit-per-sample
    // images to 8 bits per sample; and convert grayscale to RGB[A]
    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_expand(_pngPtr);
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand(_pngPtr);
    if (png_get_valid(_pngPtr, _infoPtr, PNG_INFO_tRNS))
        png_set_expand(_pngPtr);
    if (bit_depth == 16)
        png_set_strip_16(_pngPtr);
    if (color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(_pngPtr);

    // all transformations have been registered; now update _infoPtr data, get rowbytes and channels
    png_read_update_info(_pngPtr, _infoPtr);
    png_uint_32     rowbytes = png_get_rowbytes(_pngPtr, _infoPtr);
    int             nbChannels = (int)png_get_channels(_pngPtr, _infoPtr);

    // check channels
    // the check should not fail, because we have already expand the grayscale image to 24-bit RGB
    if (nbChannels < 3) // we needs 3 or 4 channels
    {
        fclose(fp);
        throw Utils::Exception("Image:", "Can't load the png file. nbChannels=" + Utils::Convert::ToString(nbChannels) + ". We support only 3 or 4 channels");
    }

    // malloc the data pointers
    image._pixels.InitSize(image._size.data[0] * image._size.data[1]);
    unsigned char   *image_data;
    png_bytepp      row_pointers = new png_bytep[image._size.data[1]];

    // if the nbChannel is 3, we need to shift the data, so we save the datas in a tmp array, or nbChannel is 4 and we save directly the data in the _pixels.data
    image_data = (nbChannels == 3) ? new unsigned char[rowbytes * image._size.data[1]] : (unsigned char*)image._pixels.data;

    // set the individual row_pointers to point at the correct offsets
    for (unsigned int i = 0;  i < image._size.data[1];  ++i)
        row_pointers[i] = image_data + (i * rowbytes);

    // read the image
    png_read_image(_pngPtr, row_pointers);

    // release
    png_read_end(_pngPtr, NULL);
    fclose(fp);
    delete[] row_pointers;

    // if nChannels was 3, we recopy the data in the pixels data with an apha value to 255
    if (nbChannels == 3)
    {
        unsigned int r = 0;
        for (unsigned int i = 0; i < image._size.data[1]; ++i)
        {
            for (unsigned int j = 0; j < image._size.data[0]; ++j)
            {
                image._pixels.data[(i * image._size.data[0]) + j].r = image_data[r++];
                image._pixels.data[(i * image._size.data[0]) + j].g = image_data[r++];
                image._pixels.data[(i * image._size.data[0]) + j].b = image_data[r++];
                image._pixels.data[(i * image._size.data[0]) + j].a = 255;
            }
        }
        delete[] image_data;
    }
}

void ImageLoaderPng::InitPng(const Utils::FileName &file, std::FILE *fp, unsigned int &width, unsigned int &height, int &bit_depth, int &color_type)
{
	// fisrt check the header
    png_byte header[8];
	std::size_t size = std::fread(header, 1, 8, fp);

	if (size != 8)
	{
		if(feof(fp) != 0)
			throw Utils::Exception("Image", "End of file reached. " + Utils::Convert::ToString(size) + " Blocks read. The file `" + file + "` is not a png file.");
		throw Utils::Exception("Image", "Error in disk read : Buffer write failed. " + Utils::Convert::ToString(size));
	}

	if (png_sig_cmp(header, 0, 8))
    {
        fclose(fp);
		throw Utils::Exception("Image", "The file `" + file + "` is not a png file.");
    }

	// set the file stream in the png lib
	png_init_io(_pngPtr, fp);
    png_set_sig_bytes(_pngPtr, 8);		// we have already read 8 bytes in the file
    png_read_info(_pngPtr, _infoPtr);	// get the pngInfo structs and next the size/bitDepth/color info
    png_get_IHDR(_pngPtr, _infoPtr, (png_uint_32*)&width, (png_uint_32*)&height, &bit_depth, &color_type, NULL, NULL, NULL);

    // setjmp() must be called in every function that calls a PNG-reading libpng function
    if (setjmp(png_jmpbuf(_pngPtr)))
    {
        fclose(fp);
        throw Utils::Exception("Image", "pnglib: setJump failed");
    }
}

void    ImageLoaderPng::Save(const Utils::FileName &file, Image &image)
{
    LOG << "ImageLoaderPng::Save() not implemented" << std::endl;
}
