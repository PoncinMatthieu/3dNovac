
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

#include "GLXContext.h"

using namespace std;
using namespace Nc;

Graphic::GLXContext::GLXContext(XWindow *win) : GLContext(win)
{
    _context = 0;
    _display = win->_display;
}

Graphic::GLXContext::~GLXContext()
{
    if (_isCreate)
    {
        glXDestroyContext(_display, _context);

        // Close the connection to the display if it's a shared context
        if (_isShared)
            XCloseDisplay(_display);
    }
}

void    Graphic::GLXContext::Active()
{
    // get the current thread id
    unsigned int threadId = System::ThreadId();

    // check if the context is already active
    if (_currentThreadId != 0)
    {
        // check if the context is already active in that thread ot into another
        if (_currentThreadId == threadId)
            System::Config::Warning("GLXContext:Active", "The context is already active.");
        else
            System::Config::Error("GLXContext:Active", "The context is active into another context.");
    }
    else
    {
        // active the context
        if (glXMakeCurrent(_display, static_cast<XWindow*>(_win)->_xwin, _context) == 0)
            System::Config::Error("GLXContext:Active", "Make current failed.");
        _currentThreadId = threadId;
    }
}

void    Graphic::GLXContext::Disable()
{
    // get the current thread id
    unsigned int threadId = System::ThreadId();

    // check if the context is active into the current thread
    if (_currentThreadId != threadId)
    {
        // check if the context is already disable or active into another thread
        if (_currentThreadId == 0)
            System::Config::Warning("GLXContext:Disable", "The context is already disable.");
        else
            System::Config::Error("GLXContext:Disable", "The context is active into another context.");
    }
    else
    {
        // disable the context
        if (glXMakeCurrent(_display, 0, 0) == 0)
            System::Config::Error("GLXContext:Disable", "Make current failed.");
        _currentThreadId = 0;
    }
}

void    Graphic::GLXContext::SwapBuffers()
{
    glXSwapBuffers(_display, static_cast<XWindow*>(_win)->_xwin);
}


/*
// Helper to check for extension string presence.  Adapted from:
//   http://www.opengl.org/resources/features/OGLextensions/
static bool isExtensionSupported(const char *extList, const char *extension)

{
  const char *start;
  const char *where, *terminator;

  // Extension names should not have spaces.
  where = strchr(extension, ' ');
  if ( where || *extension == '\0' )
    return false;

  // It takes a bit of care to be fool-proof about parsing the
  // OpenGL extensions string. Don't be fooled by sub-strings,
  // etc.
  for ( start = extList; ; ) {
    where = strstr( start, extension );

    if ( !where )
      break;

    terminator = where + strlen( extension );

    if ( where == start || *(where - 1) == ' ' )
      if ( *terminator == ' ' || *terminator == '\0' )
        return true;

    start = terminator;
  }
  return false;
}
*/
static bool ctxErrorOccurred = false;
static int ctxErrorHandler(Display *dpy, XErrorEvent *ev)
{
    ctxErrorOccurred = true;
    return 0;
}


void Graphic::GLXContext::Create(GLContext *sharedContext)
{
    XWindow *window = static_cast<XWindow*>(_win);
    ::GLXContext sharedContextPtr = (sharedContext != NULL) ? static_cast<GLXContext*>(sharedContext)->_context : NULL;

    if (!window->IsCreate())
        throw Utils::Exception("GLXContext", "Can't create any renderer if the Window isn't create");
    if (_isCreate)
        throw Utils::Exception("GLXContext", "The Renderer is already create");


    // Install an X error handler so the application won't exit if GL 3.0
    // context allocation fails.
    //
    // Note this error handler is global.  All display connections in all threads
    // of a process use the same error handler, so be sure to guard against other
    // threads issuing X commands while this code is running.
    ctxErrorOccurred = false;
    int (*oldHandler)(Display*, XErrorEvent*) = XSetErrorHandler(&ctxErrorHandler);


/* *******************************************************************************************************************
// gl3.3 context creation
    // Get the default screen's GLX extension list
    const char *glxExts = glXQueryExtensionsString(_display, window->_screen);

    // NOTE: It is not necessary to create or make current to a context before
    // calling glXGetProcAddressARB
    typedef ::GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, ::GLXContext, Bool, const int*);
    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB((const GLubyte *)"glXCreateContextAttribsARB");

    // Check for the GLX_ARB_create_context extension string and the function.
    // If either is not present, use GLX 1.3 context creation method.
    if ( !isExtensionSupported(glxExts, "GLX_ARB_create_context") || !glXCreateContextAttribsARB)
    {
        LOG << "glXCreateContextAttribsARB() not found... using old-style GLX context" << std::endl;
        _context = glXCreateNewContext(_display, *window->_fbConfig, GLX_RGBA_TYPE, sharedContextPtr, True);
    }
    else // If it does, try to get a GL 3.3 context!
    {
        int context_attribs[] =
        {
            GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
            GLX_CONTEXT_MINOR_VERSION_ARB, 3,
            //GLX_CONTEXT_FLAGS_ARB        , GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
            None
        };

        LOG << "Creating context... ";
        _context = glXCreateContextAttribsARB(_display, *window->_fbConfig, sharedContextPtr, True, context_attribs);

        // Sync to ensure any errors generated are processed.
        XSync(_display, False);
        if (!ctxErrorOccurred && _context)
            LOG << "GL 3.3 context ";
        else
        {
            // Couldn't create GL 3.3 context.  Fall back to old-style 2.x context.
            // When a context version below 3.0 is requested, implementations will
            // return the newest context version compatible with OpenGL versions less
            // than version 3.0.
            // GLX_CONTEXT_MAJOR_VERSION_ARB = 1
            context_attribs[1] = 1;
            // GLX_CONTEXT_MINOR_VERSION_ARB = 0
            context_attribs[3] = 0;

            ctxErrorOccurred = false;

            LOG <<  "Failed to create GL 3.0 context... using old-style GLX context" << std::endl;
            _context = glXCreateContextAttribsARB(_display, *window->_fbConfig, sharedContextPtr, True, context_attribs);
        }
    }
*******************************************************************************************************************/
// default context creation
    // Create a GLX context for OpenGL rendering
    _context = glXCreateNewContext(_display, *window->_fbConfig, GLX_RGBA_TYPE, sharedContextPtr, True);
    if (_context == 0)
        throw Utils::Exception("GLXContext", "Can't create a GLXContext");
/* *******************************************************************************************************************/

    // Sync to ensure any errors generated are processed.
    XSync(_display, False);

    // Restore the original error handler
    XSetErrorHandler(oldHandler);

    if (ctxErrorOccurred || !_context)
        throw Utils::Exception("GLXContext", "Failed to create an OpenGL context");

    // Verifying that the context is a direct context
    LOG_DEBUG << this;
    if (!glXIsDirect(_display, _context))
        LOG_DEBUG << " Indirect GLX rendering context obtained.";
    else
        LOG_DEBUG << " Direct GLX rendering context obtained.";
    if (sharedContext != NULL)
    {
        LOG_DEBUG << " Shared with " << sharedContext;
        _isShared = true;
    }
    LOG_DEBUG << std::endl;

    // the context is now created but not active
    _currentThreadId = 0;
    _isCreate = true;
}

Graphic::GLContext *Graphic::GLXContext::CreateNewSharedContext()
{
    if (!_isCreate)
        throw Utils::Exception("GLXContext", "Can't create a shared GLXContext if the first is not even created");

    // Create a new opengl context for the current thread
    // Create an other display connection to the Xserver otherwise the glx will crash on Qt with nvidia drivers
    Active();
    GLXContext   *newSharedRenderer = new GLXContext(static_cast<XWindow*>(_win));
    newSharedRenderer->_display = XOpenDisplay(0);
    newSharedRenderer->Create(this);
    Disable();
    return newSharedRenderer;
}
