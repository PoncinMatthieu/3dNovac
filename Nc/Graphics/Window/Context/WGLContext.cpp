/*
	3dNovac Engine
	Copyright (C) 2010, Poncin Matthieu
	All rights reserved.

	This work is licensed under the Creative Commons
	Attribution-Share Alike 2.0 France License.
	To view a copy of this license, visit :
	http://creativecommons.org/licenses/by-sa/2.0/fr/
*/

//#include <GL/wglext.h>
#include "WGLContext.h"

using namespace std;
using namespace Nc;
using namespace Nc::System;

WGLContext::WGLContext(WWindow *win, HDC drawable) : GLContext(win), _drawable(drawable)
{
    if (_win == NULL)
        throw Utils::Exception("WGLContext", "Can't create any Renderer if the window is null");
    _context = 0;
//    _pbuffer = 0;
    _antialiasingLevel = 0;
    _depth = 24;
    _stencil = 8;
    _bitsPerPixel = 32;
}

WGLContext::~WGLContext()
{
    if (_isCreate)
    {
//        if (_pbuffer == 0)
            ReleaseDC(static_cast<WWindow*>(_win)->_handle, _drawable);
//        else
//            ReleaseDC(_win->_handle, _pbuffer);
        wglDeleteContext(_context);
    }
}

void WGLContext::Create()
{
    if (!_win->IsCreate())
        throw Utils::Exception("WGLContext", "Can't create any renderer if the Window isn't create");
    if (_isCreate)
        throw Utils::Exception("WGLContext", "The Renderer is already create");

    // Get the device context attached to the window
    _drawable = GetDC(static_cast<WWindow*>(_win)->_handle);
    if (_drawable == NULL)
        throw Utils::Exception("WGLContext", "Can't get device context of window");

    // Let's find a suitable pixel format -- first try with antialiasing
    _format = 0;
/*    if (_antialiasingLevel > 0)
    {
        // Get the wglChoosePixelFormatARB function (it is an extension)
        PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = reinterpret_cast<PFNWGLCHOOSEPIXELFORMATARBPROC>(wglGetProcAddress("wglChoosePixelFormatARB"));
        if (wglChoosePixelFormatARB)
        {
            // Define the basic attributes we want for our window
            int IntAttributes[] =
            {
                WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		        WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
		        WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
                WGL_SAMPLE_BUFFERS_ARB, (_antialiasingLevel ? GL_TRUE : GL_FALSE),
		        WGL_SAMPLES_ARB,        _antialiasingLevel,
		        0,                      0
            };

            // Let's check how many formats are supporting our requirements
            int   Formats[128];
	        UINT  NbFormats;
	        float FloatAttributes[] = {0, 0};
	        bool  IsValid = wglChoosePixelFormatARB(_drawable, IntAttributes, FloatAttributes, sizeof(Formats) / sizeof(*Formats), Formats, &NbFormats) != 0;
            if (!IsValid || (NbFormats == 0))
            {
                if (_antialiasingLevel > 2)
                {
                    // No format matching our needs : reduce the multisampling level
                    LOG_ERROR << "Failed to find a pixel format supporting "
                              << _antialiasingLevel << " antialiasing levels ; trying with 2 levels" << std::endl;

                    _antialiasingLevel = IntAttributes[11] = 2;
	                IsValid = wglChoosePixelFormatARB(_drawable, IntAttributes, FloatAttributes, sizeof(Formats) / sizeof(*Formats), Formats, &NbFormats) != 0;
                }

                if (!IsValid || (NbFormats == 0))
                {
                    // Cannot find any pixel format supporting multisampling ; disabling antialiasing
                    LOG_ERROR << "Failed to find a pixel format supporting antialiasing ; antialiasing will be disabled" << std::endl;
                    _antialiasingLevel = 0;
                }
            }

            // Get the best format among the returned ones
            if (IsValid && (NbFormats > 0))
            {
                int BestScore = 0xFFFF;
/*                for (UINT i = 0; i < NbFormats; ++i)
                {
                    // Get the current format's attributes
                    PIXELFORMATDESCRIPTOR Attribs;
                    Attribs.nSize    = sizeof(PIXELFORMATDESCRIPTOR);
                    Attribs.nVersion = 1;
                    DescribePixelFormat(_drawable, Formats[i], sizeof(PIXELFORMATDESCRIPTOR), &Attribs);

                    // Evaluate the current configuration
                    int Color = Attribs.cRedBits + Attribs.cGreenBits + Attribs.cBlueBits + Attribs.cAlphaBits;
                    int Score = EvaluateConfig(Mode, Params, Color, Attribs.cDepthBits, Attribs.cStencilBits, _antialiasingLevel);

                    // Keep it if it's better than the current best
                    if (Score < BestScore)
                    {
                        BestScore  = Score;
                        _format = Formats[i];
                    }
                }
*//*            }
        }
        else
        {
            // wglChoosePixelFormatARB not supported ; disabling antialiasing
            LOG_ERROR << "Antialiasing is not supported ; it will be disabled" << std::endl;
            _antialiasingLevel = 0;
        }
    }
*/
    // Find a pixel format with no antialiasing, if not needed or not supported
    if (_format == 0)
    {
        // Setup a pixel format descriptor from the rendering settings
        PIXELFORMATDESCRIPTOR PixelDescriptor;
        ZeroMemory(&PixelDescriptor, sizeof(PIXELFORMATDESCRIPTOR));
        PixelDescriptor.nSize        = sizeof(PIXELFORMATDESCRIPTOR);
        PixelDescriptor.nVersion     = 1;
        PixelDescriptor.iLayerType   = PFD_MAIN_PLANE;
        PixelDescriptor.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        PixelDescriptor.iPixelType   = PFD_TYPE_RGBA;
        PixelDescriptor.cColorBits   = static_cast<BYTE>(_bitsPerPixel);
        PixelDescriptor.cDepthBits   = static_cast<BYTE>(_depth);
        PixelDescriptor.cStencilBits = static_cast<BYTE>(_stencil);
        PixelDescriptor.cAlphaBits   = _bitsPerPixel == 32 ? 8 : 0;

        // Get the pixel format that best matches our requirements
        _format = ChoosePixelFormat(_drawable, &PixelDescriptor);
        if (_format == 0)
            throw Utils::Exception("WGLContext", "Can't find a suitable pixel format for the device context");
    }

    // Extract the depth and stencil bits from the chosen format
    PIXELFORMATDESCRIPTOR ActualFormat;
    ActualFormat.nSize    = sizeof(PIXELFORMATDESCRIPTOR);
    ActualFormat.nVersion = 1;
    DescribePixelFormat(_drawable, _format, sizeof(PIXELFORMATDESCRIPTOR), &ActualFormat);
    _depth   = ActualFormat.cDepthBits;
    _stencil = ActualFormat.cStencilBits;

    // Set the chosen pixel format
    if (!SetPixelFormat(_drawable, _format, &ActualFormat))
        throw Utils::Exception("WGLContext", "Can't set pixel format for the device context");

    // Create the OpenGL context from the device context
    _context = wglCreateContext(_drawable);
    if (_context == NULL)
        throw Utils::Exception("WGLContext", "Can't create the gl context for the window");

    // Activate the context
//    SetActive(true);

    // Enable multisampling
//    if (Params.AntialiasingLevel > 0)
//        glEnable(GL_MULTISAMPLE_ARB);

    // ajout du renderer dans la map d'association avec le PID du thread courant
    unsigned int pid = ThreadId();
    _mapPIDRendererMutex.Lock();
    MapPIDRenderer::iterator it = _mapPIDRenderer.find(pid);
    if (it != _mapPIDRenderer.end())
        throw Utils::Exception("WGLContext", "The Current Thread as already a GLContext !");
    _mapPIDRenderer[pid] = this;
    _mapPIDRendererMutex.Unlock();
    _isCreate = true;
}

#include <strsafe.h>

GLContext *WGLContext::CreateNewSharedRenderer()
{

    LOG << "Create New Shared Renderer" << std::endl;
    if (!_isCreate)
        throw Utils::Exception("WGLContext", "Can't create a shared WGLContext if the first is not yet create");

    // creer un nouveau renderer et recopie chaque membre (un renderer n'est pas copyable) :
//    Disable();
	Active();
    WGLContext   *newSharedRenderer = new WGLContext(static_cast<WWindow*>(_win), _drawable);
    newSharedRenderer->_isCreate = true;
	// recreer un context opengl pour le thread, et un pbuffer pour rendu sur off-screen

	newSharedRenderer->_context = wglCreateContext(_drawable);
	if (newSharedRenderer->_context == NULL)
		throw Utils::Exception("WGLContext", "Can't create a new context");
    newSharedRenderer->_drawable = _drawable;
	newSharedRenderer->_format = _format;
/*
	// Extract the depth and stencil bits from the chosen format
    PIXELFORMATDESCRIPTOR ActualFormat;
    ActualFormat.nSize    = sizeof(PIXELFORMATDESCRIPTOR);
    ActualFormat.nVersion = 1;
    DescribePixelFormat(newSharedRenderer->_drawable, _format, sizeof(PIXELFORMATDESCRIPTOR), &ActualFormat);
    newSharedRenderer->_depth   = ActualFormat.cDepthBits;
    newSharedRenderer->_stencil = ActualFormat.cStencilBits;

	// Set the chosen pixel format
    if (!SetPixelFormat(_drawable, _format, &ActualFormat))
        throw Utils::Exception("WGLContext", "Can't set pixel format for the shared context");
*/
	if (!wglShareLists(_context, newSharedRenderer->_context))
	{
		LPVOID lpMsgBuf;
		LPVOID lpDisplayBuf;
		DWORD dw = GetLastError();

		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dw,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &lpMsgBuf,
			0, NULL );

		// Display the error message and exit the process

		lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
			(lstrlen((LPCTSTR)lpMsgBuf)+lstrlen((LPCTSTR)TEXT("plopiplop"))+40)*sizeof(TCHAR));
		StringCchPrintf((LPTSTR)lpDisplayBuf,
			LocalSize(lpDisplayBuf) / sizeof(TCHAR),
			TEXT("%s failed with error %d: %s"),
			TEXT("plopiplop"), dw, lpMsgBuf);
		MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

		LocalFree(lpMsgBuf);
		LocalFree(lpDisplayBuf);
		throw Utils::Exception("WGLContext", "Can't share list with the new context");
	}

/*
    HDC     hdc = wglGetCurrentDC();
    int     attrib[] = {WGL_DRAW_TO_PBUFFER_ARB, 1, PFD_SUPPORT_OPENGL, 1, WGL_BIND_TO_TEXTURE_RGBA_ARB, 1, PFD_TYPE_RGBA, 1, 0};
    int     formats[2];
    UINT    numforms;

    if(!EXT.wglChoosePixelFormat(hdc, attrib, NULL, 2, formats, &numforms))
        throw Utils::Exception("WGLContext", "Failed to choose extended pixel format.");
    int attrib2[] = {WGL_TEXTURE_FORMAT_ARB, WGL_TEXTURE_RGBA_ARB, WGL_TEXTURE_TARGET_ARB, WGL_TEXTURE_2D_ARB, WGL_MIPMAP_TEXTURE_ARB, 1, WGL_PBUFFER_LARGEST_ARB, 1, 0};
    newSharedRenderer->_pbuffer = EXT.wglGetPbufferDC(EXT.wglCreatePbuffer(hdc, _format, _win->_width, _win->_height, attrib2));

    if (newSharedRenderer->_pbuffer == NULL)
        throw Utils::Exception("WGLContext", "Failed to get PBuffer for shared context");
    newSharedRenderer->_isShared = true;
*/
// ajout du renderer dans la map d'association avec le PID du thread courant
    unsigned int pid = ThreadId();
    _mapPIDRendererMutex.Lock();
    MapPIDRenderer::iterator it = _mapPIDRenderer.find(pid);
    if (it != _mapPIDRenderer.end())
        throw Utils::Exception("WGLContext", "The Current Thread as already a GLContext !");
    _mapPIDRenderer[pid] = this;
    _mapPIDRendererMutex.Unlock();
	return newSharedRenderer;
}
