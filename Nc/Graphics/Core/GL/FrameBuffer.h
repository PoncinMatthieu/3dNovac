
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

#ifndef NC_GRAPHICS_CORE_GL_FRAMEBUFFER_H_
#define NC_GRAPHICS_CORE_GL_FRAMEBUFFER_H_

#include "RenderBuffer.h"
#include "Texture.h"

namespace Nc
{
    namespace Graphic
    {
        namespace GL
        {
            /// To Manipulate a Frame Buffer Object (fbo)
            class FrameBuffer : public Object
            {
                public:
                    typedef std::map<Enum::FrameBufferAttachementPoint, Object*>        MapAttachedObject;

                public:
                    FrameBuffer();
                    FrameBuffer(const FrameBuffer &fbo);
                    FrameBuffer &operator = (const FrameBuffer &fbo);
                    ~FrameBuffer();

                    /** Create a new copy of the object by without duplicate the ogl ressources */
                    virtual Object          *Clone() const      {return new FrameBuffer(*this);}

                    /** Create the frame buffer object (it would be automatically deleted) */
                    void                    Create(Enum::FrameBufferTarget target = Enum::FrameBuffer);

                    /** \return the index of the Frame Buffer Object */
                    virtual unsigned int    GetIndex() const    {return _index;}
                    /** Enable the fbo */
                    virtual void            Enable() const;
                    /** Disable the fbo */
                    virtual void            Disable() const;

                    /**
                        Attach the given \p renderBuffer to the frame buffer.
                        The current frame buffer need to be enable before attaching any render buffer.
                        \warning pay attention to attached the fbo in the drawing context (thread). Otherwise the fbo will have no effect.
                    */
                    void                    Attach(Enum::FrameBufferAttachementPoint attachPoint, const RenderBuffer &renderBuffer);
                    /**
                        Attach the given \p texture to the frame buffer.
                        \p level specifie the mipmap level of texture to attach.
                        The current frame buffer need to be enable before attaching any render buffer.
                        \warning pay attention to attached the fbo in the drawing context (thread). Otherwise the fbo will have no effect.
                    */
                    void                    Attach(Enum::FrameBufferAttachementPoint attachPoint, const Texture &texture, unsigned int level);

                    /**
                        \return the status of the frame buffer, affter attaching all your buffers, the status should be `FrameBufferComplete`.
                        The current frame buffer need to be enable before checking the status.
                    */
                    Enum::FrameBufferStatus CheckStatus();

                private:
                    /** Destroy the fbo */
                    void                    Release();

                private:
                    Enum::FrameBufferTarget _target;            ///< the target used to bind the fbo
                    unsigned int            _index;             ///< The id of the fbo
                    MapAttachedObject       _attachedBuffers;   ///< The map of attached buffers
            };
        }
    }
}

#endif
