
/*--------------------------------------------------------------------------------

	3dNovac Engine
	Copyright (C) 2010, Poncin Matthieu
	All rights reserved.

	This work is licensed under the Creative Commons
	Attribution-Share Alike 2.0 France License.
	To view a copy of this license, visit :
	http://creativecommons.org/licenses/by-sa/2.0/fr/

    cree le :        07/11/2010

--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------

    Provide an implementation of GLContext for a server X

--------------------------------------------------------------------------------*/

#ifndef NC_SYSTEM_GL_WIN32_RENDERER_H_
#define NC_SYSTEM_GL_WIN32_RENDERER_H_

#include <iostream>
#include "GLContext.h"
#include "../Window/WWindow.h"

namespace Nc
{
    namespace System
    {
        class LSYSTEM WGLContext : public GLContext
        {
            public:
                WGLContext(WWindow *win, HDC drawable = 0);
                virtual ~WGLContext();

                void            Create();
                GLContext      *CreateNewSharedRenderer();

                virtual inline void            Active()        {wglMakeCurrent(_drawable, _context);}
                virtual inline void            Disable()       {wglMakeCurrent(0, 0);}

				virtual inline void            SwapBuffers()   {::SwapBuffers(_drawable);}//(_pbuffer == 0) ? _drawable : _pbuffer);}

            private:
                HGLRC           _context;
                HDC             _drawable;
//                HDC             _pbuffer;  // for any shared context, we draw in a off screen renderer

                int             _antialiasingLevel;
                int             _depth;
                int             _stencil;
                int             _bitsPerPixel;
                int             _format;
        };
    }
}

#endif
