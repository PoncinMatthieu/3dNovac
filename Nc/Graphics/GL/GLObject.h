
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

    File Created At:        15/11/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_GL_OBJECT_H_
#define NC_GRAPHICS_GL_OBJECT_H_

#include <iostream>
#include "../Define.h"
#include "GL.h"

namespace Nc
{
    namespace Graphic
    {
        namespace GL
        {
            /// Interface to manage an OpenGL Object, manage the opengl resource like a smart pointer.
            /**
                The resource is shared between two object when you use the copy constructor or the copy operator.
                So GL::Object, is a reference counter of gl resources (index) like texture and shader.
                It release the opengl resources when no more reference exists.

                <div class="title"> To create new GL::Object follow these instructions : </div>
                    - Use ReleaseRef() in the destructor of the child.
                    - Use NewRef() when you are loading new resource in the child.
                    - The shared resources of a child like GL index needs to be a pointer and malloc at the loading.
                    - Redefine the function Release() to destroy the shared resources.
            */
            class LGRAPHICS Object
            {
                public:
                    Object(const Object &sp);
                    Object &operator = (const Object &sp);
                    virtual ~Object();

                    /** Create a new copy of the object by without duplicate the ogl resources. */
                    virtual Object          *Clone() const = 0;

                    /** \return true if the resource is unique. */
                    inline bool             Unique() const      {return (_nbRef != NULL && *_nbRef == 1);}
                    /** \return the number of reference on the resource. */
                    inline unsigned int     NbRef() const       {return (_nbRef != NULL) ? *_nbRef : 0;}
                    /** \return true if the resource is valid (loaded). */
                    inline bool             IsValid() const     {return (_nbRef != NULL && GetIndex() != 0);}
                    /** Destroy the reference of the resource. */
                    inline void             Destroy()           {ReleaseRef();}

                    // redefine these functions
                    /** \return the index of the GL object (like texture object). */
                    virtual unsigned int    GetIndex() const    {return 0;}
                    /** Enable the resource. */
                    virtual void            Enable() const      {}
                    /** Disable the resource. */
                    virtual void            Disable() const     {}

                protected:
                    Object();

                    /** Create a new reference for a new resource. */
                    void            NewRef();
                    /** Release the reference of the resource. */
                    void            ReleaseRef();

                private:
                    /** Add the reference on the shared counter. */
                    void            AddRef(unsigned int *nbRef);
                    /** To redefine, called when the shared resource is released. */
                    virtual void    Release() = 0;

                protected:
                    unsigned int    *_nbRef;        ///< shared counter to get the number of reference of a shared resource.
            };
        }
    }
}

#endif
