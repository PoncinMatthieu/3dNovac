
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

#ifndef NC_GRAPHICS_GL_FRAMEBUFFER_H_
#define NC_GRAPHICS_GL_FRAMEBUFFER_H_

#include "RenderBuffer.h"
#include "Texture.h"

namespace Nc
{
    namespace Graphic
    {
        namespace GL
        {
            /// To Manipulate a Frame Buffer Object (fbo).
            /**
                A framebuffer must be attached to render buffers or textures before to be used.
                After creating and attaching render buffers to the fbo, you should use the method 'CheckStatus' to check if you built the fbo correctly.

                \warning Be carefull while manipulating a framebuffer, a framebuffer must be enabled/disable each time you want to use it. The previous framebuffer will be set after disabling the current one.
            */
            class LGRAPHICS FrameBuffer : public Object
            {
                public:
                    typedef std::map<Enum::FrameBuffer::AttachementPoint, Object*>      MapAttachedObject;

                public:
                    FrameBuffer();
                    FrameBuffer(const FrameBuffer &fbo);
                    FrameBuffer &operator = (const FrameBuffer &fbo);
                    ~FrameBuffer();

                    /** Create a new copy of the object by without duplicate the ogl ressources. */
                    virtual Object          *   Clone() const      {return new FrameBuffer(*this);}

                    /** Create the frame buffer object (it would be automatically deleted). */
                    void                        Create(Enum::FrameBuffer::Target target = Enum::FrameBuffer::FrameBuffer);

                    /** \return the index of the Frame Buffer Object. */
                    virtual unsigned int        GetIndex() const    {return _index;}
                    /** Enable the fbo. */
                    virtual void                Enable();
                    /** Disable the fbo. */
                    virtual void                Disable();

                    /** Enable the fbo with the given target. */
                    virtual void                Enable(Enum::FrameBuffer::Target target);
                    /** Disable the fbo with the given target. */
                    virtual void                Disable(Enum::FrameBuffer::Target target);

                    /**
                        Attach the given \p renderBuffer to the frame buffer.
                        The current frame buffer need to be enable before attaching any render buffer.
                        \warning pay attention to attached the fbo in the drawing context (thread). Otherwise the fbo will have no effect.
                    */
                    void                        Attach(Enum::FrameBuffer::AttachementPoint attachPoint, const RenderBuffer &renderBuffer);
                    /**
                        Attach the given \p texture to the frame buffer.
                        \p level specifie the mipmap level of texture to attach.
                        The current frame buffer need to be enable before attaching any render buffer.
                        \warning pay attention to attached the fbo in the drawing context (thread). Otherwise the fbo will have no effect.
                    */
                    void                        Attach(Enum::FrameBuffer::AttachementPoint attachPoint, const Texture &texture, unsigned int level);

                    /**
                        \return the status of the frame buffer, affter attaching all your buffers, the status should be `FrameBufferComplete`.
                        The current frame buffer need to be enable before checking the status.
                    */
                    Enum::FrameBuffer::State    CheckStatus();

                    /** \return true if there are at least one buffer attached to the fbo. */
                    inline bool                 IsAttached()            {return (!_attachedBuffers.empty());}

                    /** Copy a block of pixels from the read framebuffer to the draw framebuffer. */
                    static void                 Blit(int srcX0, int srcY0, int srcX1, int srcY1, int dstX0, int dstY0, int dstX1, int dstY1, const Utils::Mask<Enum::BufferBitType> &mask, Enum::Blit::Filter filter);


                private:
                    /** Destroy the fbo. */
                    void                        Release();

                private:
                    Enum::FrameBuffer::Target   _target;            ///< the target used to bind the fbo.
                    unsigned int                _index;             ///< The id of the fbo.
                    MapAttachedObject           _attachedBuffers;   ///< The map of attached buffers.

                    unsigned int                _previouslyBound;   ///< framebuffer previously bound (saved in Enable and restored in Disable methods).
            };
        }
    }
}

#endif
