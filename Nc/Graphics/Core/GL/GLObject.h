
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

#ifndef NC_GRAPHICS_CORE_GL_OBJECT_H_
#define NC_GRAPHICS_CORE_GL_OBJECT_H_

#include <iostream>
#include "../../Define.h"
#include "../GL.h"

namespace Nc
{
    namespace Graphic
    {
        /// This namespace reference every classes that abstract the OpenGL librairy
        namespace GL
        {
            /// Interface to manage OpenGL Object, manage the opengl ressource like a smart pointer
            /**
                The ressource is shared between two ressources when you use the copy constructor or the copy operator. <br/>
                So GL::Object, is a reference counter of gl ressources (index) like texture and shader.
                Release the opengl ressources when no more reference exists <br/>
                <br/>
                To create new GL::Object follow these instructions : <br/>
                    * Use ReleaseRef() in the destructor of the child. <br/>
                    * Use NewRef() when you are loading new ressource in the child. <br/>
                    * The shared ressources of a child like GL index needs to be a pointer and malloc at the loading. <br/>
                    * Redefine the function Release() to destroy the shared ressources. <br/>
            */
            class LGRAPHICS Object
            {
                public:
                    Object(const Object &sp);
                    Object &operator = (const Object &sp);
                    virtual ~Object();

                    /** Return true if the ressource is unique */
                    inline bool             Unique() const      {return (_nbRef != NULL && *_nbRef == 1);}
                    /** Return the number of reference on the ressource */
                    inline unsigned int     NbRef() const       {return (_nbRef != NULL) ? *_nbRef : 0;}
                    /** Return true if the ressource is valid (loaded) */
                    inline bool             IsValid() const     {return (_nbRef != NULL && GetIndex() != 0);}
                    /** Destroy the reference of the ressource */
                    inline void             Destroy()           {ReleaseRef();}

                    // redefine these functions
                    /** Return the index of the GL object (like texture object) */
                    virtual unsigned int    GetIndex() const    {return 0;}
                    /** Enable the ressource */
                    virtual void            Enable() const      {}
                    /** Disable the ressource */
                    virtual void            Disable() const     {}

                protected:
                    Object();

                    /** Create a new reference for a new ressource */
                    void            NewRef();
                    /** Release the reference of the ressource */
                    void            ReleaseRef();

                private:
                    /** Add the reference on the shared counter */
                    void            AddRef(unsigned int *nbRef);
                    /** To redefine, called when the shared ressource is released */
                    virtual void    Release() = 0;

                protected:
                    unsigned int    *_nbRef;        ///< shared counter to get the number of reference of a shared ressource
            };
        }
    }
}

#endif
