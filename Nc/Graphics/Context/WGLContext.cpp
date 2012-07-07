
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

void 	WGLContext::Active()
{
	// get the current thread id
	unsigned int threadId = System::ThreadId();

	// check if the context is already active
	if (_currentThreadId != 0)
	{
		// check is the context is already active in that thread or into another thread
		if (_currentThreadId == threadId)
			System::Config::Warning("WGLContext::Active", "The context is already active.");
		else
			System::Config::Error("WGLContext::Active", "The context is active into another context.");
	}
	else
	{
		// active the context
		if (wglMakeCurrent(_drawable, _context) == false)
			System::Config::Error("WGLContext::Active", "Make current failed.");
		_currentThreadId = threadId;
	}
}

void	WGLContext::Disable()
{
	// get the current thread id
	unsigned int threadId = System::ThreadId();

	// check if the context is active into the current thread
	if (_currentThreadId != threadId)
	{
		// check if the context is already disable or active into another thread
		if (_currentThreadId == 0)
			System::Config::Warning("WGLContext::Disable", "The context is already disable.");
		else
			System::Config::Error("WGLContext::Disable", "The context is active into another context.");
	}
	else
	{
		// disable the context
		if (wglMakeCurrent(0, 0) == false)
			System::Config::Error("WGLContext::Disable", "Make current failed.");
		_currentThreadId = 0;
	}
}

void	WGLContext::SwapBuffers()
{
	// get the current thread id
	unsigned int threadId = System::ThreadId();

	// check if the context is active in the current thread
	if (_currentThreadId != threadId)
		System::Config::Error("WGLContext::SwapBuffers", "The context is not active in the current thread.");
	else
	{
		if (::SwapBuffers(_drawable) == false)
			System::Config::Error("WGLContext::SwapBuffers", "SwapBuffers failed");
	}
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
	{
		ShowError("WGLContext:Create:GetDC");
        throw Utils::Exception("WGLContext", "Can't get device context of window");
	}

	// if we are creating an independant context, we choose the pixel format
	if (sharedContext == NULL)
	{
		ChoosePixelFormat();
		SetPixelFormat();
	}

	// Create the OpenGL context from the device context
    _context = wglCreateContext(_drawable);
    if (_context == NULL)
	{
		ShowError("WGLContext:Create:SetPixelFormat");
        throw Utils::Exception("WGLContext", "Failed to create the gl context for the device");
	}

	// Set the shared context
	if (sharedContext != NULL)
	{
		if (!wglShareLists(static_cast<WGLContext*>(sharedContext)->_context, _context))
		{
			ShowError("WGLContext:Create:wglShareLists");
			throw Utils::Exception("WGLContext", "Can't share lists with the new context");
		}
		_isShared = true;
	}

	// the context is now created but not active
	_currentThreadId = 0;
	_isCreate = true;
}

void	WGLContext::ChoosePixelFormat()
{
	// Let's find a suitable pixel format -- first try with antialiasing
    _format = 0;
    if (_win->AntialiasingLevel() > 0)
    {
		// we need some wgl extention to load a proper opengl context...
		// but before loading a wgl extention we need a valid opengl context... :/
		// therefore, we create a dummy opengl context to load and use those extention before creating the real opengl context which will be used by the renderer.
		HWND dummyWindowHandle;
		HDC dummyDrawable;
		HGLRC dummyContext;
		CreateDummyContext(dummyWindowHandle, dummyDrawable, dummyContext);

		// active the context to be able to load the extension
		if (wglMakeCurrent(dummyDrawable, dummyContext) == false)
		{
			ShowError("WGLContext:ChoosePixelFormat:wglMakeCurrent");
			throw Utils::Exception("WGLContext", "Active dummy context: Make current failed");
		}

		// Load the extension wglChoosePixelFormatARB
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
                              << _win->AntialiasingLevel() << " antialiasing levels. trying with 2 levels" << std::endl;

                    static_cast<WWindow*>(_win)->_antialiasingLevel = IntAttributes[11] = 2;
	                IsValid = wglChoosePixelFormatARB(_drawable, IntAttributes, FloatAttributes, sizeof(Formats) / sizeof(*Formats), Formats, &NbFormats) != 0;
                }

                if (!IsValid || (NbFormats == 0))
                {
                    // Cannot find any pixel format supporting multisampling ; disabling antialiasing
                    LOG_ERROR << "Failed to find a pixel format supporting antialiasing. antialiasing will be disabled" << std::endl;
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
            // wglChoosePixelFormatARB not supported; disabling antialiasing
            LOG_ERROR << "Antialiasing is not supported. it will be disabled" << std::endl;
            static_cast<WWindow*>(_win)->_antialiasingLevel = 0;
        }

		// unset the dummy context
		if (wglMakeCurrent(0, 0) == false)
		{
			ShowError("WGLContext:ChoosePixelFormat:wglMakeCurrent");
			throw Utils::Exception("WGLContext:ChoosePixelFormat", "Make current failed");
		} 

		// delete dummy context
		DeleteDummyContext(dummyWindowHandle, dummyDrawable, dummyContext);
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
        _format = ::ChoosePixelFormat(_drawable, &PixelDescriptor);
        if (_format == 0)
		{
			ShowError("WGLContext::ChoosePixelFormat");
            throw Utils::Exception("WGLContext:ChoosePixelFormat", "Can't find a suitable pixel format for the device context");
		}
	}
}

void	WGLContext::SetPixelFormat()
{
    // Extract the depth and stencil bits from the chosen format
	PIXELFORMATDESCRIPTOR	attribs;
    attribs.nSize    = sizeof(PIXELFORMATDESCRIPTOR);
    attribs.nVersion = 1;
    DescribePixelFormat(_drawable, _format, sizeof(PIXELFORMATDESCRIPTOR), &attribs);
    static_cast<WWindow*>(_win)->_depth   = attribs.cDepthBits;
    static_cast<WWindow*>(_win)->_stencil = attribs.cStencilBits;

    // Set the chosen pixel format
    if (!::SetPixelFormat(_drawable, _format, &attribs))
	{
		ShowError("WGLContext:SetPixelFormat:SetPixelFormat");
        throw Utils::Exception("WGLContext:SetPixelFormat", "Failed to set pixel format for the device");
	}
}

void	WGLContext::CreateDummyContext(HWND &dummyWindowHandle, HDC &dummyDrawable, HGLRC &dummyContext)
{
	dummyWindowHandle = CreateWindowA("STATIC", "", WS_POPUP | WS_DISABLED, 0, 0, 1, 1, NULL, NULL, GetModuleHandle(NULL), NULL);
	ShowWindow(dummyWindowHandle, SW_HIDE);

	// Get the device context attached to the dummy window
    dummyDrawable = GetDC(dummyWindowHandle);
    if (dummyDrawable == NULL)
	{
		ShowError("WGLContext:CreateDummyContext:GetDC");
        throw Utils::Exception("WGLContext:CreateDummyContext", "Can't get device context of the dummy window");
	}

    // Setup a pixel format descriptor from the rendering settings
    PIXELFORMATDESCRIPTOR pixelDescriptor;
    ZeroMemory(&pixelDescriptor, sizeof(PIXELFORMATDESCRIPTOR));
    pixelDescriptor.nSize        = sizeof(PIXELFORMATDESCRIPTOR);
    pixelDescriptor.nVersion     = 1;
    pixelDescriptor.iLayerType   = PFD_MAIN_PLANE;
    pixelDescriptor.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pixelDescriptor.iPixelType   = PFD_TYPE_RGBA;
    pixelDescriptor.cColorBits   = 32;
    pixelDescriptor.cDepthBits   = 24;
    pixelDescriptor.cStencilBits = 8;
    pixelDescriptor.cAlphaBits   = 0;

    // Get the pixel format that best matches our requirements
    int format = ::ChoosePixelFormat(dummyDrawable, &pixelDescriptor);
    if (format == 0)
	{
		ShowError("WGLContext:CreateDummyContext:ChoosePixelFormat");
        throw Utils::Exception("WGLContext", "Can't find a suitable pixel format for the device context");
	}

	// Extract the depth and stencil bits from the chosen format
    PIXELFORMATDESCRIPTOR attrib;
    attrib.nSize    = sizeof(PIXELFORMATDESCRIPTOR);
    attrib.nVersion = 1;
    DescribePixelFormat(dummyDrawable, format, sizeof(PIXELFORMATDESCRIPTOR), &attrib);

    // Set the chosen pixel format
    if (!::SetPixelFormat(dummyDrawable, format, &attrib))
	{
		ShowError("WGLContext:CreateDummyContext:SetPixelFormat");
        throw Utils::Exception("WGLContext", "Can't set pixel format for the device context");
	}

    if ((dummyContext = wglCreateContext(dummyDrawable)) == NULL)
	{
		ShowError("WGLContext:CreateDummyContext:wglCreateContext");
        throw Utils::Exception("WGLContext", "Can't create the gl context for the window");
	}
}

void WGLContext::DeleteDummyContext(HWND dummyWindowHandle, HDC dummyDrawable, HGLRC dummyContext)
{
	if (ReleaseDC(dummyWindowHandle, dummyDrawable) == false)
	{
		ShowError("WGLContext:DeleteDummyContext:ReleaseDC");
		throw Utils::Exception("WGLContext:DeleteDummyContext", "ReleaseDC failed");
	}
	if (wglDeleteContext(dummyContext) == false)
	{
		ShowError("WGLContext:DeleteDummyContext:wglDeleteContext");
		throw Utils::Exception("WGLContext:DeleteDummyContext", "wglDeleteContext failed");
	}
	if (DestroyWindow(dummyWindowHandle) == false)
	{
		ShowError("WGLContext:DeleteDummyContext:DestroyWindow");
		throw Utils::Exception("WGLContext:DeleteDummyContext", "DestroyWindow failed");
	}
}

GLContext *WGLContext::CreateNewSharedContext()
{
    if (!_isCreate)
        throw Utils::Exception("WGLContext", "Can't create a shared WGLContext if the first is not yet create");

	// create a new opengl context for the current thread
	Active();
	WGLContext *newSharedContext = new WGLContext(static_cast<WWindow*>(_win));
	newSharedContext->Create(this);
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