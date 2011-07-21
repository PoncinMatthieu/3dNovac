
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
    NewRef();
    glGenRenderbuffers(1, &_index);
    LOG_DEBUG << "RenderBuffer " << _index << " CREATED" << std::endl;
}

void    RenderBuffer::Release()
{
    glDeleteRenderbuffers(1, &_index);
    LOG_DEBUG << "RenderBuffer " << _index << " DELETED" << std::endl;
    _index = 0;
}

void    RenderBuffer::Enable() const
{
    if (State::IsSet())
        State::Current().Bind(Enum::RenderBuffer::RenderBuffer, _index);
    else
        glBindRenderbuffer(Enum::RenderBuffer::RenderBuffer, _index);
}

void    RenderBuffer::Disable() const
{
    if (State::IsSet())
        State::Current().Unbind(Enum::RenderBuffer::RenderBuffer);
    else
        glBindRenderbuffer(Enum::RenderBuffer::RenderBuffer, 0);
}

void    RenderBuffer::Init(Enum::RenderBuffer::ColorFormat internalFormat, const Vector2ui &size, unsigned int samples)
{
    if (State::IsSet() && State::Current().CurrentBound(Enum::RenderBuffer::RenderBuffer) != _index)
        throw Utils::Exception("RenderBuffer::Init", "Can't init the render buffer witch is not enabled.");

    if (samples == 0)
    {
        glRenderbufferStorage(Enum::RenderBuffer::RenderBuffer, internalFormat, size.Data[0], size.Data[1]);
    }
    else
    {
        glRenderbufferStorageMultisample(Enum::RenderBuffer::RenderBuffer, samples, internalFormat, size.Data[0], size.Data[1]);
    }
    _type = ColorBuffer;
}

void    RenderBuffer::Init(Enum::RenderBuffer::DepthFormat internalFormat, const Vector2ui &size, unsigned int samples)
{
    if (State::IsSet() && State::Current().CurrentBound(Enum::RenderBuffer::RenderBuffer) != _index)
        throw Utils::Exception("RenderBuffer::Init", "Can't init the render buffer witch is not enabled.");

    if (samples == 0)
    {
        glRenderbufferStorage(Enum::RenderBuffer::RenderBuffer, internalFormat, size.Data[0], size.Data[1]);
    }
    else
    {
        glRenderbufferStorageMultisample(Enum::RenderBuffer::RenderBuffer, samples, internalFormat, size.Data[0], size.Data[1]);
    }
    _type = DepthBuffer;
}

void    RenderBuffer::Init(Enum::RenderBuffer::StencilFormat internalFormat, const Vector2ui &size, unsigned int samples)
{
    if (State::IsSet() && State::Current().CurrentBound(Enum::RenderBuffer::RenderBuffer) != _index)
        throw Utils::Exception("RenderBuffer::Init", "Can't init the render buffer witch is not enabled.");

    if (samples == 0)
    {
        glRenderbufferStorage(Enum::RenderBuffer::RenderBuffer, internalFormat, size.Data[0], size.Data[1]);
    }
    else
    {
        glRenderbufferStorageMultisample(Enum::RenderBuffer::RenderBuffer, samples, internalFormat, size.Data[0], size.Data[1]);
    }
    _type = StencilBuffer;
}

void    RenderBuffer::Init(Enum::RenderBuffer::DepthStencilFormat internalFormat, const Vector2ui &size, unsigned int samples)
{
    if (State::IsSet() && State::Current().CurrentBound(Enum::RenderBuffer::RenderBuffer) != _index)
        throw Utils::Exception("RenderBuffer::Init", "Can't init the render buffer witch is not enabled.");

    if (samples == 0)
    {
        glRenderbufferStorage(Enum::RenderBuffer::RenderBuffer, internalFormat, size.Data[0], size.Data[1]);
    }
    else
    {
        glRenderbufferStorageMultisample(Enum::RenderBuffer::RenderBuffer, samples, internalFormat, size.Data[0], size.Data[1]);
    }
    _type = DepthStencilBuffer;
}
