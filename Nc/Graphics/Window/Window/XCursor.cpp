
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

    File Created At:        02/11/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include <X11/xpm.h>
#include "XCursor.h"
#include "XWindow.h"

using namespace Nc;
using namespace Nc::Math;

::Cursor        *Graphic::Cursor::_hiddenCursor = NULL;
unsigned int    Graphic::Cursor::_nbCursor = 0;

Graphic::Cursor::Cursor(XWindow *w)
    : ICursor(w), _win(w), _cursor(NULL)
{
    CreateHiddenCursor();
}

Graphic::Cursor::Cursor(const Graphic::Cursor &c)
    : ICursor(c), _win(c._win), _cursor(c._cursor)
{
    CreateHiddenCursor();
}


Graphic::Cursor::~Cursor()
{
    if (_attachedWindow->CurrentCursor() == this)
        SetCurrentCursor(NULL);
    if (_win->_currentCursor == this)
        _win->_currentCursor = NULL;
    _nbCursor--;
    if (_nbCursor == 0 && _hiddenCursor)
    {
        //XFreeCursor(static_cast<XWindow*>(_win)->_display, *_hiddenCursor);  // generate multiple invalid read of size 4 under valgrind
        delete _hiddenCursor;
        _hiddenCursor = NULL;
    }
    if (_cursor.Unique() && _cursor != NULL)
    {
        //XLockDisplay(static_cast<XWindow*>(_win)->_display);
        XFreeCursor(_win->_display, *_cursor);
        //XUnlockDisplay(static_cast<XWindow*>(_win)->_display);
    }
}

void Graphic::Cursor::Enable()
{
    if (_win->IsOwn())
    {
        //XLockDisplay(static_cast<XWindow*>(_win)->_display);
        XDefineCursor(_win->_display, _win->_xwin, (_cursor != NULL) ? *_cursor : None);
        //XUnlockDisplay(static_cast<XWindow*>(_win)->_display);
        //XFlush(static_cast<XWindow*>(_win)->_display); // genere un XIO error avec Qt
    }
    SetCurrentCursor(this);
}

void Graphic::Cursor::Disable()
{
    if (_win->IsOwn())
    {
        //XLockDisplay(static_cast<XWindow*>(_win)->_display);
        XDefineCursor(_win->_display, _win->_xwin, *_hiddenCursor);
        //XUnlockDisplay(static_cast<XWindow*>(_win)->_display);
        //XFlush(static_cast<XWindow*>(_win)->_display); // genere un XIO error avec Qt
    }
    SetCurrentCursor(this);
}

void Graphic::Cursor::CreateHiddenCursor()
{
    if (_nbCursor == 0)
    {
        //XLockDisplay(static_cast<XWindow*>(_win)->_display);
        // Create the cursor's pixmap (1x1 pixels)
        Pixmap cursorPixmap = XCreatePixmap(_win->_display, _win->_xwin, 1, 1, 1);
        GC graphicsContext = XCreateGC(_win->_display, cursorPixmap, 0, NULL);
        XDrawPoint(_win->_display, cursorPixmap, graphicsContext, 0, 0);
        XFreeGC(_win->_display, graphicsContext);

        // Create the cursor, using the pixmap as both the shape and the mask of the cursor
        XColor color;
        color.flags = DoRed | DoGreen | DoBlue;
        color.red = color.blue = color.green = 0;
        _hiddenCursor = new ::Cursor(XCreatePixmapCursor(_win->_display, cursorPixmap, cursorPixmap, &color, &color, 0, 0));

        // We don't need the pixmap any longer, free it
        XFreePixmap(_win->_display, cursorPixmap);
        //XUnlockDisplay(static_cast<XWindow*>(_win)->_display);
    }
    _nbCursor++;
}

void Graphic::Cursor::LoadFromData(const unsigned char *data, const unsigned char *mask, const Vector2ui &size, const Vector2ui &posCenter)
{
    if (!_win->IsOwn())
        return;

    //XLockDisplay(static_cast<XWindow*>(_win)->_display);
    try
    {
        if (_cursor != NULL) // delete the last cursor
            XFreeCursor(_win->_display, *_cursor);

        XGCValues   GCvalues;
        GC          GCcursor;
        XImage      *data_image, *mask_image;
        Pixmap      data_pixmap, mask_pixmap;
        int         clen, i;
        char        *x_data, *x_mask;

        // Mix the mask and the data
        clen = (size.Data[0]/8)*size.Data[1];
        x_data = (char *)malloc(clen);
        x_mask = (char *)malloc(clen);
        for ( i=0; i<clen; ++i )
        {
            x_mask[i] = data[i] | mask[i];
            x_data[i] = data[i];
        }

        // Create the data image
        data_image = XCreateImage(_win->_display, DefaultVisual(_win->_display, _win->_screen), 1, XYBitmap, 0, x_data, size.Data[0], size.Data[1], 8, size.Data[0]/8);
        data_image->byte_order = MSBFirst;
        data_image->bitmap_bit_order = MSBFirst;
        data_pixmap = XCreatePixmap(_win->_display, _win->_xwin, size.Data[0], size.Data[1], 1);

        // Create the data mask
        mask_image = XCreateImage(_win->_display, DefaultVisual(_win->_display, _win->_screen), 1, XYBitmap, 0, x_mask, size.Data[0], size.Data[1], 8, size.Data[0]/8);
        mask_image->byte_order = MSBFirst;
        mask_image->bitmap_bit_order = MSBFirst;
        mask_pixmap = XCreatePixmap(_win->_display, _win->_xwin, size.Data[0], size.Data[1], 1);

        // Create the graphics context
        GCvalues.function = GXcopy;
        GCvalues.foreground = ~0;
        GCvalues.background =  0;
        GCvalues.plane_mask = AllPlanes;
        GCcursor = XCreateGC(_win->_display, data_pixmap, (GCFunction|GCForeground|GCBackground|GCPlaneMask), &GCvalues);

        // Blit the images to the pixmaps
        XPutImage(_win->_display, data_pixmap, GCcursor, data_image, 0, 0, 0, 0, size.Data[0], size.Data[1]);
        XPutImage(_win->_display, mask_pixmap, GCcursor, mask_image, 0, 0, 0, 0, size.Data[0], size.Data[1]);
        XFreeGC(_win->_display, GCcursor);
        // These free the x_data and x_mask memory pointers
        XDestroyImage(data_image);
        XDestroyImage(mask_image);

        // Create the cursor
        XColor black = {0, 0, 0, 0};
        XColor white = {0xffff, 0xffff, 0xffff, 0xffff};
        if (_cursor == NULL)
            _cursor = new ::Cursor;
        *_cursor = XCreatePixmapCursor(_win->_display, data_pixmap, mask_pixmap,
                                       &black, &white, posCenter.Data[0], posCenter.Data[1]);
        XFreePixmap(_win->_display, data_pixmap);
        XFreePixmap(_win->_display, mask_pixmap);
    }
    catch (...)
    {
        //XUnlockDisplay(static_cast<XWindow*>(_win)->_display);
        throw;
    }
    //XUnlockDisplay(static_cast<XWindow*>(_win)->_display);


/*
    char *x_data = (char*)malloc(sizeof(char) * size.Data[0] * size.Data[1] / 8);
    char *x_mask = (char*)malloc(sizeof(char) * size.Data[0] * size.Data[1] / 8);

    memcpy(x_data, data, size.Data[0] * size.Data[1] / 8);
    memset(x_data, 0xFF, size.Data[0] * size.Data[1] / 8);

	// Create the data image
    LOG << "create image" << std::endl;
//	XImage *data_image = XCreateImage(_win->_display, DefaultVisual(_win->_display, _win->_screen), 1, XYBitmap, 0, const_cast<char*>(data), size.Data[0], size.Data[1], 32, size.Data[0] * 4);
	XImage *data_image = XCreateImage(_win->_display, DefaultVisual(_win->_display, _win->_screen), 1, XYBitmap, 0, x_data, size.Data[0], size.Data[1], 8, size.Data[0] / 8);
    if (data_image == NULL)
        throw Utils::Exception("XCursor", "Failed to create the data XImage.");
    LOG << "create image end" << std::endl;
	data_image->byte_order = MSBFirst;
	data_image->bitmap_bit_order = MSBFirst;
    LOG << "create pixmap" << std::endl;
	Pixmap dataPixmap = XCreatePixmap(_win->_display, _win->_xwin, size.Data[0], size.Data[1], 1);


	XImage *data_mask = XCreateImage(_win->_display, DefaultVisual(_win->_display, _win->_screen), 1, XYBitmap, 0, x_mask, size.Data[0], size.Data[1], 8, size.Data[0] / 8);
//	XImage *data_mask = XCreateImage(_win->_display, v, depth, XYBitmap, 0, x_mask, size.Data[0], size.Data[1], 8, (size.Data[0] + 7) / 8);
    if (data_mask == NULL)
        throw Utils::Exception("XCursor", "Failed to create the mask XImage.");
    LOG << "create image end" << std::endl;
	data_mask->byte_order = MSBFirst;
	data_mask->bitmap_bit_order = MSBFirst;
    LOG << "create pixmap" << std::endl;
	Pixmap maskPixmap = XCreatePixmap(_win->_display, _win->_xwin, size.Data[0], size.Data[1], 1);


	// Create the graphics context
    LOG << "create graphic context" << std::endl;
    //GC graphicsContext = XCreateGC(_win->_display, cursorPixmap, 0, NULL);
	XGCValues GCvalues;
	GCvalues.function = GXcopy;
	GCvalues.foreground = ~0;
	GCvalues.background =  0;
	GCvalues.plane_mask = AllPlanes;
	GC graphicsContext = XCreateGC(_win->_display, dataPixmap, GCFunction | GCForeground | GCBackground | GCPlaneMask, &GCvalues);

	// Blit the images to the pixmaps and free the image and graphic context
    LOG << "put image" << std::endl;
	XPutImage(_win->_display, dataPixmap, graphicsContext, data_image, 0, 0, 0, 0, size.Data[0], size.Data[1]);
	XPutImage(_win->_display, maskPixmap, graphicsContext, data_mask, 0, 0, 0, 0, size.Data[0], size.Data[1]);
    XFreeGC(_win->_display, graphicsContext);
    LOG << "destroy image" << std::endl;
	XDestroyImage(data_image);

    // Create the cursor, using the pixmap as both the shape and the mask of the cursor
	XColor black = { 0, 0, 0, 0 };
	XColor white = { 0xffff, 0xffff, 0xffff, 0xffff };
    LOG << "create pixmap cursor" << std::endl;
    _cursor = new ::Cursor(XCreatePixmapCursor(_win->_display, dataPixmap, maskPixmap, &black, &white, posCenter.Data[0], posCenter.Data[1]));
    LOG << "create pixmap cursor end" << std::endl;

    // We don't need the pixmap any longer, free it
    XFreePixmap(_win->_display, dataPixmap);
    XFreePixmap(_win->_display, maskPixmap);
*/
}

/*
void Graphic::Cursor::LoadFromImage(const Utils::FileName &f)
{

}
*/

void Graphic::Cursor::LoadFromXpm(const char *xpm[])
{
    int i, row, col;
    int width, height;
    unsigned char *data;
    unsigned char *mask;
    int hot_x, hot_y;

    sscanf(xpm[0], "%d %d", &width, &height);
    data = new unsigned char[width/8*height];
    mask = new unsigned char[width/8*height];

    i = -1;
    for ( row=0; row<height; ++row )
    {
        for ( col=0; col<width; ++col )
        {
            if ( col % 8 )
            {
                data[i] <<= 1;
                mask[i] <<= 1;
            }
            else
            {
                ++i;
                data[i] = mask[i] = 0;
            }
            switch (xpm[4+row][col])
            {
                case 'X':
                data[i] |= 0x01;
                mask[i] |= 0x01;
                break;
                case '.':
                mask[i] |= 0x01;
                break;
                case ' ':
                break;
            }
        }
    }
    sscanf(xpm[4+row], "%d,%d", &hot_x, &hot_y);
    LoadFromData(data, mask, Vector2i(width, height), Vector2i(hot_x, hot_y));
    delete[] data;
    delete[] mask;


/*
    Vector2i                    size, posCenter;
    unsigned int                nbColors;
    Array<unsigned char[4]>     colors;
    Array<char>                 colorsChar;

    // read size, and malloc the data tab
    sscanf(xpm[0], "%d %d %d", &size.Data[0], &size.Data[1], &nbColors);
    unsigned char *data = new unsigned char[4 * size.Data[0] * size.Data[1]];

LOG << "read colors" << std::endl;
    // read colors
    colors.InitSize(nbColors);
    colorsChar.InitSize(nbColors);
    for (unsigned int i = 0; i < nbColors; ++i)
    {
        std::string     s(xpm[i+1]);
        unsigned char   c;
        size_t          pos = s.find_first_of("#") + 1;

        LOG << "s = " << s << std::endl;
        LOG << "pos = " << pos << std::endl;
        colors[i][0] = 255;
        colors[i][1] = 255;
        colors[i][2] = 255;
        colors[i][3] = 0;
        colorsChar[i] = s[0];
        for (unsigned int j = 0; j < 4 && pos != std::string::npos && pos < s.size() && pos > 0; ++j, pos += 2)
        {
            std::istringstream iss(s.substr(pos, 2));
            unsigned int c;
            iss >> std::hex >> c;
            colors[i][j] = c;
            LOG << "color value: " << (unsigned int)colors[i][j] << std::endl;
        }

    }
LOG << "read data" << std::endl;
    // read data
    for (unsigned int i = 0; i < size.Data[1]; ++i)
    {
        for (unsigned int j = 0; j < size.Data[0]; ++j)
        {
            for (unsigned int k = 0; k < nbColors; ++k)
                if (xpm[i + nbColors + 1][j] == colorsChar[k])
                {
                    for (unsigned int l = 0; l < 4; ++l)
                        data[(i*size.Data[0]*4) + (j*4) + l] = colors[k][l];
LOG << k;
                }
        }
LOG << std::endl;
    }

    for (unsigned int i = 0; i < size.Data[1]; ++i)
    {
        for (unsigned int j = 0; j < size.Data[0]; ++j)
        {
            LOG << "[";
            for (unsigned int l = 0; l < 4; ++l)
                LOG << (int)data[(i*size.Data[0]*4) + (j*4) + l];
            LOG << "] ";
        }
        LOG << std::endl;
    }


    sscanf(xpm[size.Data[1] + nbColors + 1], "%d,%d", &posCenter.Data[0], &posCenter.Data[1]);
LOG << "load from dada" << std::endl;
    LoadFromData(data, size, posCenter, 1);
    delete[] data;
*/
}
