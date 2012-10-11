
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

    File Created At:        16/07/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_GL_RENDERBUFFER_H_
#define NC_GRAPHICS_GL_RENDERBUFFER_H_

#include "GLObject.h"

namespace Nc
{
    namespace Graphic
    {
        namespace GL
        {
            /// To manipulate a Render Buffer Object (rbo).
            /**
                \sa
                    - FrameBuffer
            */
            class LGRAPHICS RenderBuffer : public Object
            {
                public:
                    enum BufferType
                    {
                        ColorBuffer,
                        DepthBuffer,
                        StencilBuffer,
                        DepthStencilBuffer
                    };

                public:
                    RenderBuffer();
                    ~RenderBuffer();

                    /** Create a new copy of the object by without duplicate the ogl resources. */
                    virtual Object          *Clone() const                  {return new RenderBuffer(*this);}

                    /** Create the rbo. */
                    void                    Create();
                    /**
                        Initialize and etablish the data storage, format, dimensions, and sample count of the render object image.
                        The render buffer need to be enable before any initialization.
                    */
                    void                    Init(Enum::RenderBuffer::ColorFormat internalFormat, const Vector2ui &size, unsigned int samples = 0);
                    /**
                        Initialize and etablish the data storage, format, dimensions, and sample count of the render object image.
                        The render buffer need to be enable before any initialization.
                    */
                    void                    Init(Enum::RenderBuffer::DepthFormat internalFormat, const Vector2ui &size, unsigned int samples = 0);
                    /**
                        Initialize and etablish the data storage, format, dimensions, and sample count of the render object image.
                        The render buffer need to be enable before any initialization.
                    */
                    void                    Init(Enum::RenderBuffer::StencilFormat internalFormat, const Vector2ui &size, unsigned int samples = 0);
                    /**
                        Initialize and etablish the data storage, format, dimensions, and sample count of the render object image.
                        The render buffer need to be enable before any initialization.
                    */
                    void                    Init(Enum::RenderBuffer::DepthStencilFormat internalFormat, const Vector2ui &size, unsigned int samples = 0);

                    /** \return the type of the render buffer. */
                    inline BufferType       Type() const                    {return _type;}

                    /** \return the index of the Render Buffer Object. */
                    virtual unsigned int    GetIndex() const                {return _index;}
                    /** Enable the rbo. */
                    virtual void            Enable() const;
                    /** Disable the rbo. */
                    virtual void            Disable() const;

                private:
                    /** Destroy the rbo. */
                    virtual void            Release();

                private:
                    BufferType              _type;
                    unsigned int            _index;         ///< The id of the rbo.
            };
        }
    }
}

#endif
