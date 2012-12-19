
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

#include "RenderBuffer.h"

using namespace Nc::Graphic::GL;

RenderBuffer::RenderBuffer()
    : _index(0), _type(RenderBuffer::ColorBuffer)
{
}

RenderBuffer::~RenderBuffer()
{
    ReleaseRef();
}

void    RenderBuffer::Create()
{
    CALLSTACK_INFO("RenderBuffer::Create() ");
    NewRef();
    glGenRenderbuffers(1, &_index);
    CALLSTACK_APPEND_INFO(Utils::Convert::ToString(_index));
    NC_GRAPHIC_GL_CHECK_ERROR();
}

void    RenderBuffer::Release()
{
    CALLSTACK_INFO("RenderBuffer::Release() " + Utils::Convert::ToString(_index));
    State::Instance().Unbind(Enum::RenderBuffer::RenderBuffer);
    glDeleteRenderbuffers(1, &_index);
    _index = 0;
    NC_GRAPHIC_GL_CHECK_ERROR();
}

void    RenderBuffer::Enable() const
{
    State::Instance().Bind(Enum::RenderBuffer::RenderBuffer, _index);
}

void    RenderBuffer::Disable() const
{
    State::Instance().Unbind(Enum::RenderBuffer::RenderBuffer);
}

void    RenderBuffer::Init(Enum::RenderBuffer::ColorFormat internalFormat, const Vector2ui &size, unsigned int samples)
{
    CALLSTACK_INFO("RenderBuffer::Init()");
    if (State::Instance().CurrentBound(Enum::RenderBuffer::RenderBuffer) != _index)
        throw Utils::Exception("RenderBuffer::Init", "Can't init the render buffer which is not enabled.");

    if (samples == 0)
    {
        glRenderbufferStorage(Enum::RenderBuffer::RenderBuffer, internalFormat, size.data[0], size.data[1]);
    }
    else
    {
        glRenderbufferStorageMultisample(Enum::RenderBuffer::RenderBuffer, samples, internalFormat, size.data[0], size.data[1]);
    }
    _type = ColorBuffer;
    NC_GRAPHIC_GL_CHECK_ERROR();
}

void    RenderBuffer::Init(Enum::RenderBuffer::DepthFormat internalFormat, const Vector2ui &size, unsigned int samples)
{
    CALLSTACK_INFO("RenderBuffer::Init()");
    if (State::Instance().CurrentBound(Enum::RenderBuffer::RenderBuffer) != _index)
        throw Utils::Exception("RenderBuffer::Init", "Can't init the render buffer which is not enabled.");

    if (samples == 0)
    {
        glRenderbufferStorage(Enum::RenderBuffer::RenderBuffer, internalFormat, size.data[0], size.data[1]);
    }
    else
    {
        glRenderbufferStorageMultisample(Enum::RenderBuffer::RenderBuffer, samples, internalFormat, size.data[0], size.data[1]);
    }
    _type = DepthBuffer;
    NC_GRAPHIC_GL_CHECK_ERROR();
}

void    RenderBuffer::Init(Enum::RenderBuffer::StencilFormat internalFormat, const Vector2ui &size, unsigned int samples)
{
    CALLSTACK_INFO("RenderBuffer::Init()");
    if (State::Instance().CurrentBound(Enum::RenderBuffer::RenderBuffer) != _index)
        throw Utils::Exception("RenderBuffer::Init", "Can't init the render buffer which is not enabled.");

    if (samples == 0)
    {
        glRenderbufferStorage(Enum::RenderBuffer::RenderBuffer, internalFormat, size.data[0], size.data[1]);
    }
    else
    {
        glRenderbufferStorageMultisample(Enum::RenderBuffer::RenderBuffer, samples, internalFormat, size.data[0], size.data[1]);
    }
    _type = StencilBuffer;
    NC_GRAPHIC_GL_CHECK_ERROR();
}

void    RenderBuffer::Init(Enum::RenderBuffer::DepthStencilFormat internalFormat, const Vector2ui &size, unsigned int samples)
{
    CALLSTACK_INFO("RenderBuffer::Init()");
    if (State::Instance().CurrentBound(Enum::RenderBuffer::RenderBuffer) != _index)
        throw Utils::Exception("RenderBuffer::Init", "Can't init the render buffer which is not enabled.");

    if (samples == 0)
    {
        glRenderbufferStorage(Enum::RenderBuffer::RenderBuffer, internalFormat, size.data[0], size.data[1]);
    }
    else
    {
        glRenderbufferStorageMultisample(Enum::RenderBuffer::RenderBuffer, samples, internalFormat, size.data[0], size.data[1]);
    }
    _type = DepthStencilBuffer;
    NC_GRAPHIC_GL_CHECK_ERROR();
}
