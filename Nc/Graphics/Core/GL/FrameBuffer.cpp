
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

FrameBuffer::FrameBuffer()
    : _index(0)
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
    for (; !_attachedBuffers.empty(); _attachedBuffers.erase(_attachedBuffers.begin()))
        delete _attachedBuffers.begin()->second;
    ReleaseRef();
}

void    FrameBuffer::Create(Enum::FrameBufferTarget target)
{
    NewRef();
    glGenFramebuffers(1, &_index);
    _target = target;
    LOG_DEBUG << "FrameBuffer " << _index << " CREATED" << std::endl;
}

void    FrameBuffer::Release()
{
    glDeleteFramebuffers(1, &_index);
    LOG_DEBUG << "FrameBuffer " << _index << " DELETED" << std::endl;
    _index = 0;
}

void    FrameBuffer::Enable() const
{
    if (State::IsSet())
        State::Current().Bind(_target, _index);
    else
        glBindFramebuffer(_target, _index);
}

void    FrameBuffer::Disable() const
{
    if (State::IsSet())
        State::Current().Unbind(_target);
    else
        glBindFramebuffer(_target, 0);
}

void    FrameBuffer::Attach(Enum::FrameBufferAttachementPoint attachPoint, const RenderBuffer &renderBuffer)
{
    if (!renderBuffer.IsValid())
        throw Utils::Exception("FrameBuffer::Attach", "The given RenderBuffer is not valid");
    if (!IsValid())
        throw Utils::Exception("FrameBuffer::Attach", "The FrameBuffer is not valid");
    if (State::IsSet() && State::Current().CurrentBound(_target) != _index)
        throw Utils::Exception("FrameBuffer::Attach", "Can't attach the render buffer with a framebuffer witch is not enabled.");

    glFramebufferRenderbuffer(_target, attachPoint, Enum::RenderBuffer, renderBuffer.GetIndex());
    _attachedBuffers[attachPoint] = renderBuffer.Clone();
}

void    FrameBuffer::Attach(Enum::FrameBufferAttachementPoint attachPoint, const Texture &texture, unsigned int level)
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

Enum::FrameBufferStatus FrameBuffer::CheckStatus()
{
    if (State::IsSet() && State::Current().CurrentBound(_target) != _index)
        throw Utils::Exception("FrameBuffer::CheckStatus", "Can't check the status of the framebuffer witch is not enabled.");
    return static_cast<Enum::FrameBufferStatus>(glCheckFramebufferStatus(_target));
}
