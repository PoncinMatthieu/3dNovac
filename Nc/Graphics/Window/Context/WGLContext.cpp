
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

    File Created At:        07/11/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "../../Core/GL.h"
#include "WGLContext.h"
#include <strsafe.h>

using namespace std;
using namespace Nc;
using namespace Nc::System;
using namespace Nc::Graphic;

WGLContext::WGLContext(WWindow *win, HDC drawable) : GLContext(win), _drawable(drawable)
{
    if (_win == NULL)
        throw Utils::Exception("WGLContext", "Can't create any Renderer if the window is null");
    _context = 0;
}

WGLContext::~WGLContext()
{
    if (_isCreate)
    {
        ReleaseDC(static_cast<WWindow*>(_win)->_handle, _drawable);
        wglDeleteContext(_context);
    }
}

HGLRC WGLContext::CreateDummyContext()
{
    // Setup a pixel format descriptor from the rendering settings
    PIXELFORMATDESCRIPTOR PixelDescriptor;
    ZeroMemory(&PixelDescriptor, sizeof(PIXELFORMATDESCRIPTOR));
    PixelDescriptor.nSize        = sizeof(PIXELFORMATDESCRIPTOR);
    PixelDescriptor.nVersion     = 1;
    PixelDescriptor.iLayerType   = PFD_MAIN_PLANE;
    PixelDescriptor.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    PixelDescriptor.iPixelType   = PFD_TYPE_RGBA;
    PixelDescriptor.cColorBits   = 32;
    PixelDescriptor.cDepthBits   = 24;
    PixelDescriptor.cStencilBits = 8;
    PixelDescriptor.cAlphaBits   = 0;

    // Get the pixel format that best matches our requirements
    int format = ChoosePixelFormat(_drawable, &PixelDescriptor);
    if (format == 0)
	{
		ShowError("WGLContext::ChoosePixelFormat");
        throw Utils::Exception("WGLContext", "Can't find a suitable pixel format for the device context");
	}

	// Extract the depth and stencil bits from the chosen format
    PIXELFORMATDESCRIPTOR attrib;
    attrib.nSize    = sizeof(PIXELFORMATDESCRIPTOR);
    attrib.nVersion = 1;
    DescribePixelFormat(_drawable, format, sizeof(PIXELFORMATDESCRIPTOR), &attrib);

    // Set the chosen pixel format
    if (!SetPixelFormat(_drawable, format, &attrib))
	{
		ShowError("WGLContext::SetPixelFormat");
        throw Utils::Exception("WGLContext", "Can't set pixel format for the device context");
	}

	HGLRC context = wglCreateContext(_drawable);
    if (context == NULL)
	{
		ShowError("WGLContext::wglCreateContext");
        throw Utils::Exception("WGLContext", "Can't create the gl context for the window");
	}
	return context;
}

void WGLContext::Create(GLContext *sharedContext)
{
    if (!_win->IsCreate())
        throw Utils::Exception("WGLContext", "Can't create any renderer if the Window isn't create");
    if (_isCreate)
        throw Utils::Exception("WGLContext", "The Renderer is already create");

    // Get the device context attached to the window
    _drawable = GetDC(static_cast<WWindow*>(_win)->_handle);
    if (_drawable == NULL)
        throw Utils::Exception("WGLContext", "Can't get device context of window");

	// \todo windows do not allow to set the pixel format more than once... therefore to use the wgl extention we need to create also a dummy windows
	// we need some wgl extention to load a proper opengl context...
	// but before loading a wgl extention we need a valid opengl context... :/
	// therefore, we create a dummy opengl context to load and use those extention before creating the real opengl context which will be used by the renderer.
	//HGLRC dummyContext = CreateDummyContext();
	//if (wglMakeCurrent(_drawable, dummyContext) == false)
	//	throw Utils::Exception("WGLContext", "Active dummy context: Make current failed");

    // Let's find a suitable pixel format -- first try with antialiasing
    _format = 0;
    if (_win->AntialiasingLevel() > 0)
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
                WGL_SAMPLE_BUFFERS_ARB, (_win->AntialiasingLevel() ? GL_TRUE : GL_FALSE),
		        WGL_SAMPLES_ARB,        _win->AntialiasingLevel(),
		        0,                      0
            };

            // Let's check how many formats are supporting our requirements
            int   Formats[128];
	        UINT  NbFormats;
	        float FloatAttributes[] = {0, 0};
	        bool  IsValid = wglChoosePixelFormatARB(_drawable, IntAttributes, FloatAttributes, sizeof(Formats) / sizeof(*Formats), Formats, &NbFormats) != 0;
            if (!IsValid || (NbFormats == 0))
            {
                if (_win->AntialiasingLevel() > 2)
                {
                    // No format matching our needs : reduce the multisampling level
                    LOG_ERROR << "Failed to find a pixel format supporting "
                              << _win->AntialiasingLevel() << " antialiasing levels ; trying with 2 levels" << std::endl;

                    static_cast<WWindow*>(_win)->_antialiasingLevel = IntAttributes[11] = 2;
	                IsValid = wglChoosePixelFormatARB(_drawable, IntAttributes, FloatAttributes, sizeof(Formats) / sizeof(*Formats), Formats, &NbFormats) != 0;
                }

                if (!IsValid || (NbFormats == 0))
                {
                    // Cannot find any pixel format supporting multisampling ; disabling antialiasing
                    LOG_ERROR << "Failed to find a pixel format supporting antialiasing ; antialiasing will be disabled" << std::endl;
                    static_cast<WWindow*>(_win)->_antialiasingLevel = 0;
                }
            }

            // Get the best format among the returned ones
            if (IsValid && (NbFormats > 0))
            {
                int BestScore = 0xFFFF;
                for (UINT i = 0; i < NbFormats; ++i)
                {
                    // Get the current format's attributes
                    PIXELFORMATDESCRIPTOR attribs;
                    attribs.nSize    = sizeof(PIXELFORMATDESCRIPTOR);
                    attribs.nVersion = 1;
                    DescribePixelFormat(_drawable, Formats[i], sizeof(PIXELFORMATDESCRIPTOR), &attribs);

                    // Evaluate the current configuration
                    int Color = attribs.cRedBits + attribs.cGreenBits + attribs.cBlueBits + attribs.cAlphaBits;
                    int Score = abs(static_cast<int>(_win->BitsPerPixel()) - Color) + abs(static_cast<int>(_win->Depth()) - attribs.cDepthBits) + abs(static_cast<int>(_win->Stencil()) - attribs.cStencilBits);
						//EvaluateConfig(Mode, Params, Color, Attribs.cDepthBits, Attribs.cStencilBits, _antialiasingLevel);

                    // Keep it if it's better than the current best
                    if (Score < BestScore)
                    {
                        BestScore  = Score;
                        _format = Formats[i];
                    }
                }
            }
        }
        else
        {
            // wglChoosePixelFormatARB not supported ; disabling antialiasing
            LOG_ERROR << "Antialiasing is not supported ; it will be disabled" << std::endl;
            static_cast<WWindow*>(_win)->_antialiasingLevel = 0;
        }
    }

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
        PixelDescriptor.cColorBits   = static_cast<BYTE>(_win->BitsPerPixel());
        PixelDescriptor.cDepthBits   = static_cast<BYTE>(_win->Depth());
        PixelDescriptor.cStencilBits = static_cast<BYTE>(_win->Stencil());
        PixelDescriptor.cAlphaBits   = (_win->BitsPerPixel() == 32) ? 8 : 0;

        // Get the pixel format that best matches our requirements
        _format = ChoosePixelFormat(_drawable, &PixelDescriptor);
        if (_format == 0)
		{
			ShowError("WGLContext::ChoosePixelFormat");
            throw Utils::Exception("WGLContext", "Can't find a suitable pixel format for the device context");
		}
    }

/*	if (wglMakeCurrent(0, 0) == false)
		throw Utils::Exception("WGLContext", "Disable dummy context: Make current failed");
*/

    // Extract the depth and stencil bits from the chosen format
    PIXELFORMATDESCRIPTOR attribs;
    attribs.nSize    = sizeof(PIXELFORMATDESCRIPTOR);
    attribs.nVersion = 1;
    DescribePixelFormat(_drawable, _format, sizeof(PIXELFORMATDESCRIPTOR), &attribs);
    static_cast<WWindow*>(_win)->_depth   = attribs.cDepthBits;
    static_cast<WWindow*>(_win)->_stencil = attribs.cStencilBits;

    // Set the chosen pixel format
    if (!SetPixelFormat(_drawable, _format, &attribs))
	{
		ShowError("WGLContext::SetPixelFormat");
        throw Utils::Exception("WGLContext", "Failed to set pixel format for the device");
	}

    // Create the OpenGL context from the device context
    _context = wglCreateContext(_drawable);
    if (_context == NULL)
	{
		ShowError("WGLContext::SetPixelFormat");
        throw Utils::Exception("WGLContext", "Failed to create the gl context for the device");
	}

	if (sharedContext != NULL)
	{
		if (!wglShareLists(static_cast<WGLContext*>(sharedContext)->_context, _context))
		{
			ShowError("WGLContext::wglShareLists");
			throw Utils::Exception("WGLContext", "Can't share lists with the new context");
		}
	}

	_isCreate = true;
}

GLContext *WGLContext::CreateNewSharedContext()
{
    if (!_isCreate)
        throw Utils::Exception("WGLContext", "Can't create a shared WGLContext if the first is not yet create");

	// create a new opengl context for the current thread
	Active();
	WGLContext *newSharedContext = new WGLContext(static_cast<WWindow*>(_win));
	newSharedContext->Create(this);
	newSharedContext->_isShared = true;
	Disable();
	return newSharedContext;
}

void WGLContext::ShowError(char *title)
{
	// Get and display the error message
	// Ugly windows code...
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, NULL);

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, (lstrlen((LPCTSTR)lpMsgBuf)+lstrlen((LPCTSTR)TEXT(""))+40)*sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf, LocalSize(lpDisplayBuf) / sizeof(TCHAR), TEXT("%s failed with error %d: %s"), TEXT(""), dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, (LPCTSTR)TEXT(title), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}