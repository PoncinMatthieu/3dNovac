
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
/*-----------------------------------------------------------------------------

    Provide a GLObject interface, its a reference counter of gl ressources (index)
    like texture and shader
    release the opengl ressources when the no more reference exists

    NB:
        use ReleaseRef() in the destructor of the chid
        use AddRef() when you are loading new ressource in the child
        the shared ressources of a child like GL index needs to be a pointer and malloc at the loading
        redefine the function Release() to destroy the shared ressources

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_CORE_GL_OBJECT_H_
#define NC_GRAPHICS_CORE_GL_OBJECT_H_

#include <iostream>
#include "../../Define.h"
#include "Extension.h"

namespace Nc
{
    namespace Graphic
    {
        namespace GL
        {
            class LCORE Object
            {
                public:
                    Object(const Object &sp);
                    Object &operator = (const Object &sp);
                    virtual ~Object();

                    inline bool             Unique() const      {return (_nbRef != NULL && *_nbRef == 1);}
                    inline unsigned int     NbRef() const       {return (_nbRef != NULL) ? *_nbRef : 0;}
                    inline bool             IsValid() const     {return (_nbRef != NULL && GetIndex() != 0);}
                    inline void             Destroy()           {ReleaseRef();}

                    // redefine these functions
                    virtual unsigned int    GetIndex() const    {return 0;}
                    virtual void            Enable() const      {}
                    virtual void            Disable() const     {}

                protected:
                    Object();
                    void            NewRef();
                    void            ReleaseRef();

                private:
                    void            AddRef(unsigned int *nbRef);
                    virtual void    Release() = 0;

                protected:
                    unsigned int    *_nbRef;
            };
        }
    }
}

#endif
