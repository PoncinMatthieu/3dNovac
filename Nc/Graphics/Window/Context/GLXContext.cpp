
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
//    _pbuffer = 0;
}

Graphic::GLXContext::~GLXContext()
{
    if (_isCreate)
    {
        //if (_pbuffer != 0)
        //    glXDestroyPbuffer(_display, _pbuffer);
        glXDestroyContext(_display, _context);

        // Close the connection to the display if it's a shared context
        if (_isShared)
            XCloseDisplay(_display);
    }
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

            LOG <<  "Failed to create GL 3.0 context... using old-style GLX context" << std::endl;;
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

    // Verifying that context is a direct context
    if (!glXIsDirect(_display, _context))
        LOG_DEBUG << "Indirect GLX rendering context obtained" << std::endl;
    else
        LOG_DEBUG << "Direct GLX rendering context obtained." << std::endl;
    _isCreate = true;
}

Graphic::GLContext *Graphic::GLXContext::CreateNewSharedContext()
{
LOG << "create shared context" << std::endl;
    if (!_isCreate)
        throw Utils::Exception("GLXContext", "Can't create a shared GLXContext if the first is not yet create");

LOG << "activate context" << std::endl;
    // creer un nouveau renderer et recopie chaque membre (un renderer n'est pas copyable) :
    Active();

    XWindow *window = static_cast<XWindow*>(_win);

LOG << "Create new context instance" << std::endl;
    //GLXContext   *newSharedRenderer = new GLXContext(window);
    GLXContext   *newSharedRenderer = new GLXContext(window);

    // create an other display connection to the Xserver otherwise the glx will crash on Qt with nvidia drivers
    newSharedRenderer->_display = XOpenDisplay(0);

    // recreer un context opengl pour le thread, et un pbuffer pour rendu sur off-screen
LOG << "Create new context" << std::endl;
    newSharedRenderer->Create(this);
LOG << "Create new pbuffer" << std::endl;

    //newSharedRenderer->_pbuffer = glXCreatePbuffer(_display, *window->_fbConfig, NULL);
    newSharedRenderer->_isShared = true;

LOG << "Disable" << std::endl;
    Disable();

LOG << "create shared context DONE" << std::endl;
    return newSharedRenderer;
}
