
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

#include "FrameBuffer.h"

using namespace Nc::Graphic::GL;
using namespace Nc::Utils;

FrameBuffer::FrameBuffer()
    : _index(0), _previouslyBound(0)
{
}

FrameBuffer::FrameBuffer(const FrameBuffer &fbo)
    : Object(fbo)
{
    for (MapAttachedObject::const_iterator it = fbo._attachedBuffers.begin(); it != fbo._attachedBuffers.end(); ++it)
        _attachedBuffers[it->first] = it->second->Clone();
    _target = fbo._target;
    _index = fbo._index;
}

FrameBuffer &FrameBuffer::operator = (const FrameBuffer &fbo)
{
    Object::operator = (fbo);
    for (; !_attachedBuffers.empty(); _attachedBuffers.erase(_attachedBuffers.begin()))
        delete _attachedBuffers.begin()->second;
    for (MapAttachedObject::const_iterator it = fbo._attachedBuffers.begin(); it != fbo._attachedBuffers.end(); ++it)
        _attachedBuffers[it->first] = it->second->Clone();
    _target = fbo._target;
    _index = fbo._index;
    return *this;
}

FrameBuffer::~FrameBuffer()
{
    ReleaseRef();
}

void    FrameBuffer::Create(Enum::FrameBuffer::Target target)
{
    NewRef();
    glGenFramebuffers(1, &_index);
    _target = target;
    LOG_DEBUG << "FrameBuffer " << _index << " CREATED" << std::endl;
}

void    FrameBuffer::Release()
{
    glDeleteFramebuffers(1, &_index);
    for (; !_attachedBuffers.empty(); _attachedBuffers.erase(_attachedBuffers.begin()))
        delete _attachedBuffers.begin()->second;
    LOG_DEBUG << "FrameBuffer " << _index << " DELETED" << std::endl;
    _index = 0;
}

void    FrameBuffer::Enable()
{
    Enable(_target);
}

void    FrameBuffer::Disable()
{
    Disable(_target);
}

void    FrameBuffer::Enable(Enum::FrameBuffer::Target target)
{
    if (State::IsSet())
    {
        State &st = State::Current();

        // save the previous framebuffer
        _previouslyBound = st.CurrentBound(target);

        // set the framebuffer
        st.Bind(target, _index);
    }
    else
        throw Utils::Exception("FrameBuffer::Enable", "GL::State is not set.");
}

void    FrameBuffer::Disable(Enum::FrameBuffer::Target target)
{
    if (State::IsSet())
    {
        // unset the framebuffer
        if (_previouslyBound == 0)
            State::Current().Unbind(target);
        else
            State::Current().Bind(target, _previouslyBound);
        _previouslyBound = 0;
    }
    else
        throw Utils::Exception("FrameBuffer::Disable", "GL::State is not set.");
}

void    FrameBuffer::Attach(Enum::FrameBuffer::AttachementPoint attachPoint, const RenderBuffer &renderBuffer)
{
    if (!renderBuffer.IsValid())
        throw Utils::Exception("FrameBuffer::Attach", "The given RenderBuffer is not valid");
    if (!IsValid())
        throw Utils::Exception("FrameBuffer::Attach", "The FrameBuffer is not valid");
    if (State::IsSet() && State::Current().CurrentBound(_target) != _index)
        throw Utils::Exception("FrameBuffer::Attach", "Can't attach the render buffer with a framebuffer witch is not enabled.");

    glFramebufferRenderbuffer(_target, attachPoint, Enum::RenderBuffer::RenderBuffer, renderBuffer.GetIndex());
    _attachedBuffers[attachPoint] = renderBuffer.Clone();
}

void    FrameBuffer::Attach(Enum::FrameBuffer::AttachementPoint attachPoint, const Texture &texture, unsigned int level)
{
    if (!texture.IsValid())
        throw Utils::Exception("FrameBuffer::Attach", "The given Texture is not valid");
    if (!IsValid())
        throw Utils::Exception("FrameBuffer::Attach", "The FrameBuffer is not valid");
    if (State::IsSet() && State::Current().CurrentBound(_target) != _index)
        throw Utils::Exception("FrameBuffer::Attach", "Can't attach the render buffer with a framebuffer witch is not enabled.");

    glFramebufferTexture(_target, attachPoint, texture.GetIndex(), level);
    _attachedBuffers[attachPoint] = texture.Clone();
}

Enum::FrameBuffer::State FrameBuffer::CheckStatus()
{
    if (State::IsSet() && State::Current().CurrentBound(_target) != _index)
        throw Utils::Exception("FrameBuffer::CheckStatus", "Can't check the status of the framebuffer witch is not enabled.");
    return static_cast<Enum::FrameBuffer::State>(glCheckFramebufferStatus(_target));
}

void    FrameBuffer::Blit(int srcX0, int srcY0, int srcX1, int srcY1, int dstX0, int dstY0, int dstX1, int dstY1, const Mask<Enum::BufferBitType> &mask, Enum::Blit::Filter filter)
{
    glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1,
                      dstX0, dstY0, dstX1, dstY1,
                      mask.GetMask(), filter);
}

