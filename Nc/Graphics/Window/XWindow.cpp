
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

    File Created At:        25/01/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "XWindow.h"
#include <X11/extensions/Xrandr.h>
#include "../Context/GLXContext.h"
#include "../Data/Image.h"

using namespace Nc;
using namespace Nc::Math;
using namespace Nc::Graphic;

// struct used in the function `SetWindowStyle` to set the window properties
struct WMHints
{
    unsigned long Flags;
    unsigned long Functions;
    unsigned long Decorations;
    long          InputMode;
    unsigned long State;
};

Graphic::Window::Window(SceneGraphManager *sceneGraphManager)
    : IWindow(sceneGraphManager)
{
    _display = NULL;
    _vInfo = NULL;
    _fbConfig = NULL;
    _xwin = 0;
    _input = new XWindowInput(this);
}

Graphic::Window::Window(const std::string &title, const Math::Vector2ui &size, const WindowStyle &style, const Utils::FileName &icon, unsigned int antialiasingLevel, SceneGraphManager *sceneGraphManager)
    : IWindow(title, size, style, icon, antialiasingLevel, sceneGraphManager)
{
    _display = NULL;
    _vInfo = NULL;
    _fbConfig = NULL;
    _xwin = 0;
    _input = new XWindowInput(this);
}

Graphic::Window::~Window()
{
    if (_isCreate)
        Close();
	delete _input;
}

static Bool WaitForNotify(Display *dpy, XEvent *event, XPointer arg)
{
    return (event->type == MapNotify) && (event->xmap.window == (::Window)arg);
}

void Graphic::Window::Create(const std::string &title, const Vector2ui &size, const WindowStyle &style, const Utils::FileName &icon, unsigned int antialiasingLevel)
{
    // init XLib support for concurrent thread support
    if (XInitThreads() == 0)
        throw Utils::Exception("XWindow", "Can't create a window. Your system does not support threads.");

    // Open a connection to the X server
    _display = XOpenDisplay(0);
    if (_display == NULL)
        throw Utils::Exception("XWindow", "Could not open X11 display !");
    _screen = DefaultScreen(_display);

    // Compute position and size
    _width = size.data[0];
    _height = size.data[1];
    if (_width <= 0)
        _width = 1;
    if (_height <= 0)
        _height = 1;
    bool    fullscreen = style.Enabled(Fullscreen);
    int     left = (fullscreen) ? 0 : (DisplayWidth(_display, _screen) - _width)  / 2;
    int     top = (fullscreen) ? 0 : (DisplayHeight(_display, _screen) - _height) / 2;

    // Switch to fullscreen if necessary
    if (fullscreen)
        SwitchToFullscreen(size);

    _antialiasingLevel = antialiasingLevel;
    ChooseBestVisualInfo();

    XSetWindowAttributes attributes;
    attributes.event_mask        = FocusChangeMask | ButtonPressMask | ButtonReleaseMask | ButtonMotionMask | PointerMotionMask |
                                   KeyPressMask | KeyReleaseMask | StructureNotifyMask | EnterWindowMask | LeaveWindowMask;
                                   /*ExposureMask | PropertyChangeMask | KeymapStateMask;*/
    attributes.colormap          = XCreateColormap(_display, RootWindow(_display, _vInfo->screen), _vInfo->visual, AllocNone);
    attributes.override_redirect = fullscreen; // disp the window border if false

    _xwin = XCreateWindow(_display, RootWindow(_display, _vInfo->screen), left, top, _width, _height,
                          0, _vInfo->depth, InputOutput, _vInfo->visual,
                          CWEventMask | CWColormap | CWOverrideRedirect , &attributes);
    if (_xwin == 0)
        throw Utils::Exception("XWindow", "Failed to create the window");

    XStoreName(_display, _xwin, title.c_str()); // set the window title

    SetWindowStyle(style); // set the window style

    // initialize the inputs of the window
    _input->Create();

    if (!_style.Enabled(Hidden)) // if the window is not hidden.
    {
        // Map the window to the screen, and wait for it to appear
        XMapWindow(_display, _xwin);
        XEvent event;
        XIfEvent(_display, &event, WaitForNotify, (XPointer)_xwin);
    }
    XFlush(_display);

    if (fullscreen)
    {
        XGrabPointer(_display, _xwin, true, 0, GrabModeAsync, GrabModeAsync, _xwin, None, CurrentTime);
        XGrabKeyboard(_display, _xwin, true, GrabModeAsync, GrabModeAsync, CurrentTime);
    }

    SetIcon(icon);

    // creation du curseur par defaut
    _defaultCursor = new Cursor(this);
    _defaultCursor->Enable();

    _isCreate = true;
    _own = true;
}

void Graphic::Window::ChooseBestVisualInfo()
{
    // Get all the visuals configs allowed
    int             nbConfigs;
    GLXFBConfig     *fbConfigs = glXGetFBConfigs(_display, _screen, &nbConfigs);

    // Find the best visual
    int             bestScore  = 0xFFFF;
    GLXFBConfig     *bestConfig = NULL;
    while (!bestConfig)
    {
        for (int i = 0; i < nbConfigs; ++i)
        {
            // Get the current visual attributes
            int doubleBuffer, red, green, blue, alpha, depth, stencil, multiSampling, samples;
            int renderable, drawableType, renderType, visualType;

            glXGetFBConfigAttrib(_display, fbConfigs[i], GLX_X_RENDERABLE,      &renderable);
            glXGetFBConfigAttrib(_display, fbConfigs[i], GLX_DRAWABLE_TYPE,     &drawableType);
            glXGetFBConfigAttrib(_display, fbConfigs[i], GLX_RENDER_TYPE,       &renderType);
            glXGetFBConfigAttrib(_display, fbConfigs[i], GLX_X_VISUAL_TYPE,     &visualType);
            glXGetFBConfigAttrib(_display, fbConfigs[i], GLX_DOUBLEBUFFER,      &doubleBuffer);
            glXGetFBConfigAttrib(_display, fbConfigs[i], GLX_RED_SIZE,          &red);
            glXGetFBConfigAttrib(_display, fbConfigs[i], GLX_GREEN_SIZE,        &green);
            glXGetFBConfigAttrib(_display, fbConfigs[i], GLX_BLUE_SIZE,         &blue);
            glXGetFBConfigAttrib(_display, fbConfigs[i], GLX_ALPHA_SIZE,        &alpha);
            glXGetFBConfigAttrib(_display, fbConfigs[i], GLX_DEPTH_SIZE,        &depth);
            glXGetFBConfigAttrib(_display, fbConfigs[i], GLX_STENCIL_SIZE,      &stencil);
            glXGetFBConfigAttrib(_display, fbConfigs[i], GLX_SAMPLE_BUFFERS_ARB,&multiSampling);
            glXGetFBConfigAttrib(_display, fbConfigs[i], GLX_SAMPLES_ARB,       &samples);

            // check that we have the doublebuffer and rbga mode, it's the subsistence level
            //LOG << renderable << " " << drawableType << " " << renderType << " " << visualType << std::endl;
            if (doubleBuffer != 0 && renderable && renderType == GLX_RGBA_BIT && visualType == GLX_TRUE_COLOR)
            {
                // Evaluate the current configuration
                int score = Math::Abs(static_cast<int>(32 - (red + green + blue + alpha))) +
                            Math::Abs(static_cast<int>(24 - depth))   +
                            Math::Abs(static_cast<int>(8  - stencil)) +
                            Math::Abs(static_cast<int>(_antialiasingLevel - ((multiSampling) ? samples : 0)));

                // Keep it if it's better than the current best
                if (score < bestScore)
                {
                    bestScore  = score;
                    bestConfig = &fbConfigs[i];
                }
            }
        }

        // If no visual has been found, try a lower level of antialiasing
        if (!bestConfig)
        {
            if (_antialiasingLevel > 2)
            {
                LOG_ERROR << "Failed to find a pixel format supporting " << _antialiasingLevel << " antialiasing levels. Trying with 2 levels" << std::endl;
                _antialiasingLevel = 2;
            }
            else if (_antialiasingLevel > 0)
            {
                LOG_ERROR << "Failed to find a pixel format supporting antialiasing. Antialiasing will be disabled" << std::endl;
                _antialiasingLevel = 0;
            }
            else
                throw Utils::Exception("XWindow", "Failed to find a suitable pixel format for the window");
        }
    }

    // Assign the chosen visual, and free the temporary visuals array
    _fbConfig = bestConfig;

    // Create an X colormap and window with a visual matching
    _vInfo = glXGetVisualFromFBConfig(_display, *_fbConfig);
    if (_vInfo == NULL)
        throw Utils::Exception("XWindow", "Failed to get the Visual Info structure from the fb config");
}

bool Graphic::Window::SetIcon(const Utils::FileName &f)
{
    if (f.empty() || !f.IsReadable()) // si l'icone n'existe pas, on retourne false sans afficher de message d'erreur
        return false;
    Image image;
    image.LoadFromFile(f);

    // X11 wants BGRA pixels : swap red and blue channels
    // Note : this memory will never be freed, but it seems to cause a bug on exit if I do so
    const unsigned char *pix = image.GetPixels();
    unsigned char *iconPixels = (unsigned char*)malloc(sizeof(unsigned char) * (image.Size().data[0] * image.Size().data[1] * 4));
    for (std::size_t i = 0; i < image.Size().data[1] * image.Size().data[1]; ++i)
    {
        iconPixels[i * 4 + 0] = pix[i * 4 + 2];
        iconPixels[i * 4 + 1] = pix[i * 4 + 1];
        iconPixels[i * 4 + 2] = pix[i * 4 + 0];
        iconPixels[i * 4 + 3] = pix[i * 4 + 3];
    }

    // Create the icon pixmap
    Visual          *defVisual = DefaultVisual(_display, _screen);
    unsigned int    defDepth  = DefaultDepth(_display, _screen);
    XImage          *iconImage = XCreateImage(_display, defVisual, defDepth, ZPixmap, 0, (char*)iconPixels, image.Size().data[0], image.Size().data[1], 32, 0);
    if (!iconImage)
        throw Utils::Exception("XWindow", "Failed to set the window's icon");
    Pixmap iconPixmap = XCreatePixmap(_display, RootWindow(_display, _screen), image.Size().data[0], image.Size().data[1], defDepth);
    XGCValues values;
    GC iconGC = XCreateGC(_display, iconPixmap, 0, &values);
    XPutImage(_display, iconPixmap, iconGC, iconImage, 0, 0, 0, 0, image.Size().data[0], image.Size().data[1]);
    XFreeGC(_display, iconGC);
    XDestroyImage(iconImage);

    // Create the mask pixmap (must have 1 bit depth)
    std::size_t pitch = (image.Size().data[0] + 7) / 8;
    static std::vector<unsigned char> maskPixels(pitch * image.Size().data[1], 0);
    for (std::size_t j = 0; j < image.Size().data[1]; ++j)
        for (std::size_t i = 0; i < pitch; ++i)
            for (std::size_t k = 0; k < 8; ++k)
                if (i * 8 + k < image.Size().data[0])
                {
                    unsigned char opacity = (pix[(i * 8 + k + j * image.Size().data[0]) * 4 + 3] > 0) ? 1 : 0;
                    maskPixels[i + j * pitch] |= (opacity << k);
                }

    Pixmap maskPixmap = XCreatePixmapFromBitmapData(_display, _xwin, (char*)&maskPixels[0], image.Size().data[0], image.Size().data[1], 1, 0, 1);

    // Send our new icon to the window through the WMHints
    XWMHints* hints = XAllocWMHints();
    hints->flags       = IconPixmapHint | IconMaskHint;
    hints->icon_pixmap = iconPixmap;
    hints->icon_mask   = maskPixmap;
    XSetWMHints(_display, _xwin, hints);
    XFree(hints);

    XFlush(_display);
    return true;
}

void Graphic::Window::SetWindowStyle(const WindowStyle &style)
{
    if (style.Disabled(Fullscreen))
    {
        Atom WMHintsAtom = XInternAtom(_display, "_MOTIF_WM_HINTS", true);
        if (WMHintsAtom != None)
        {
            const unsigned long MWM_HINTS_FUNCTIONS   = 1 << 0;
            const unsigned long MWM_HINTS_DECORATIONS = 1 << 1;

            //static const unsigned long MWM_DECOR_ALL         = 1 << 0;
            const unsigned long MWM_DECOR_BORDER      = 1 << 1;
            const unsigned long MWM_DECOR_RESIZEH     = 1 << 2;
            const unsigned long MWM_DECOR_TITLE       = 1 << 3;
            const unsigned long MWM_DECOR_MENU        = 1 << 4;
            const unsigned long MWM_DECOR_MINIMIZE    = 1 << 5;
            const unsigned long MWM_DECOR_MAXIMIZE    = 1 << 6;

            //static const unsigned long MWM_FUNC_ALL          = 1 << 0;
            const unsigned long MWM_FUNC_RESIZE       = 1 << 1;
            const unsigned long MWM_FUNC_MOVE         = 1 << 2;
            const unsigned long MWM_FUNC_MINIMIZE     = 1 << 3;
            const unsigned long MWM_FUNC_MAXIMIZE     = 1 << 4;
            const unsigned long MWM_FUNC_CLOSE        = 1 << 5;

            WMHints hints;
            hints.Flags       = MWM_HINTS_FUNCTIONS | MWM_HINTS_DECORATIONS;
            hints.Decorations = 0;
            hints.Functions   = 0;

            if (style.Enabled(Titlebar))
            {
                hints.Decorations |= MWM_DECOR_BORDER | MWM_DECOR_TITLE | MWM_DECOR_MINIMIZE | MWM_DECOR_MENU;
                hints.Functions   |= MWM_FUNC_MOVE | MWM_FUNC_MINIMIZE;
            }
            if (style.Enabled(Resizeable))
            {
                hints.Decorations |= MWM_DECOR_MAXIMIZE | MWM_DECOR_RESIZEH;
                hints.Functions   |= MWM_FUNC_MAXIMIZE | MWM_FUNC_RESIZE;
            }
            if (style.Enabled(Closeable))
            {
                hints.Decorations |= 0;
                hints.Functions   |= MWM_FUNC_CLOSE;
            }

            XChangeProperty(_display, _xwin, WMHintsAtom, WMHintsAtom, 32, PropModeReplace,
                            reinterpret_cast<const unsigned char*>(&hints), 5);
        }

        // This is a hack to force some windows managers to disable resizing
        if (style.Disabled(Resizeable))
        {
            XSizeHints XSizeHints;
            XSizeHints.flags      = PMinSize | PMaxSize;
            XSizeHints.min_width  = XSizeHints.max_width  = _width;
            XSizeHints.min_height = XSizeHints.max_height = _height;
            XSetWMNormalHints(_display, _xwin, &XSizeHints);
        }
    }
}

void Graphic::Window::SwitchToFullscreen(const Vector2ui &size)
{
    // Check if the XRandR extension is present
    int version;
    if (!XQueryExtension(_display, "RANDR", &version, &version, &version)) // XRandr extension is not supported : we cannot use fullscreen mode
        LOG_ERROR << "Fullscreen is not supported, switching to window mode" << std::endl;
    else
    {
        // Get the current configuration
        XRRScreenConfiguration* config = XRRGetScreenInfo(_display, RootWindow(_display, _screen));
        if (!config) // Failed to get the screen configuration
            LOG_ERROR << "Failed to get the current screen configuration for fullscreen mode, switching to window mode" << std::endl;
        else
        {
            // Get the current rotation
            Rotation currentRotation;
            XRRConfigCurrentConfiguration(config, &currentRotation);

            // Get the available screen sizes
            bool match = false;
            int nbSizes;
            XRRScreenSize* sizesAvailable = XRRConfigSizes(config, &nbSizes);
            for (int i = 0; i < nbSizes && sizesAvailable && !match; ++i)  // Search a compatible resolution
                if ((unsigned int)sizesAvailable[i].width == size.data[0] &&
                    (unsigned int)sizesAvailable[i].height == size.data[1]) // the size match, Switch to fullscreen mode
                {
                    XRRSetScreenConfig(_display, config, RootWindow(_display, _screen), i, currentRotation, CurrentTime);
                    match = true;
                }
            if (!match)
            {
                LOG_ERROR << "Can't switch to fullscreen mode, the resolution size (" << size.data[0] << "/" << size.data[1] << ") don't match." << std::endl;
                LOG_ERROR << "Here the list of compatibles size:" << std::endl;
                for (int i = 0; i < nbSizes && sizesAvailable && !match; ++i)  // print the compatibles resolutions
                    LOG_ERROR << "\t" << (unsigned int)sizesAvailable[i].width << "/" << sizesAvailable[i].height << std::endl;
            }
            XRRFreeScreenConfigInfo(config);   // Free the configuration instance
        }
    }
}

void Graphic::Window::UseExistingWindow(void *disp, int winId, const Vector2ui &size, unsigned int antialiasingLevel)
{
    _width = size.data[0];
    _height = size.data[1];
    _antialiasingLevel = antialiasingLevel;
    int     ret;
    int     doubleBufferAttributes[] =  {   GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
                                            GLX_RENDER_TYPE,   GLX_RGBA_BIT,
                                            GLX_DOUBLEBUFFER,  True,  // Request a double-buffered color buffer with
                                            GLX_RED_SIZE,      1,     // the maximum number of bits per component
                                            GLX_GREEN_SIZE,    1,
                                            GLX_BLUE_SIZE,     1,
                                            None };

    _display = reinterpret_cast<Display*>(disp);
    if (_display == NULL)
        throw Utils::Exception("XWindow", "The disp parameter is not a Display* from X11");
    _screen = DefaultScreen(_display);
    _xwin = winId;

    // get the visual info
    //ChooseBestVisualInfo();
    _fbConfig = glXChooseFBConfig(_display, _screen, doubleBufferAttributes, &ret);
    if (_fbConfig == NULL)
        throw Utils::Exception("XWindow", "Can't choose a double buffered config for the window");

    _vInfo = glXGetVisualFromFBConfig(_display, *_fbConfig);
    if (_vInfo == NULL)
        throw Utils::Exception("XWindow", "Failed to get the Visual Info structure from the fb config");

    // initialize the inputs of the window
    _input = new XWindowInput(this);

    // creation du curseur par defaut
    _defaultCursor = new Cursor(this);
    _defaultCursor->Enable();

    _isCreate = true;
    _own = false;
}

GLContext *Graphic::Window::CreateGLContext()
{
    _context = new GLXContext(this);
    _context->Create();
    return _context;
}

void Graphic::Window::Close()
{
    // Destroy the renderer attach to the window
    if (_context)
        delete _context;
	_context = NULL;

    // Destroy the input context
	_input->Destroy();

    if (_vInfo != NULL)
        XFree(_vInfo);

    // Destroy the window
    if (_own)
    {
        if (_xwin)
        {
            XDestroyWindow(_display, _xwin);
            XFlush(_display);
        }

        // Close the connection to the display
        XCloseDisplay(_display);
    }
    _isCreate = false;
}

void Graphic::Window::Resize(unsigned int width, unsigned int height)
{
    _width = width;
    _height = height;
    XResizeWindow(_display, _xwin, _width, _height);
    XFlush(_display);
    Resized();
}

ICursor *Graphic::Window::NewCursor()
{
    return new Cursor(this);
}

void    Graphic::Window::Hide()
{
    XUnmapWindow(_display, _xwin);
    XFlush(_display);
    _style.Enable(Hidden);
}

void    Graphic::Window::Show()
{
    XMapWindow(_display, _xwin);
    XFlush(_display);
    _style.Disable(Hidden);
}

