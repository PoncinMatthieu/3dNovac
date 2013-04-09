
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

    File Created At:        22/06/2012
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "ICursor.h"
#include "../Input/SubWindowInput.h"
#include "SubWindow.h"

using namespace Nc;
using namespace Nc::Graphic;


SubWindow::SubWindow(IWindow *parent, SceneGraphManager *sceneGraphManager)
    : IWindow(sceneGraphManager), _parent(parent)
{
}

SubWindow::~SubWindow()
{
    if (_isCreate)
		Close();
}

void    SubWindow::Close()
{
    if (_isCreate)
    {
        _parent->RemoveSubWindow(this);
		_isCreate = false;
    }
}

void    SubWindow::Create(const Math::Vector2ui &size)
{
	if (_isCreate)
		System::Config::Error("SubWindow", "The window has already been created.");

	_width = size.data[0];
    _height = size.data[1];
    _context = _parent->Context();
    _own = _parent->IsOwn();
    _antialiasingLevel = _parent->AntialiasingLevel();
    _depth = _parent->Depth();
    _stencil = _parent->Stencil();
    _bitsPerPixel = _parent->BitsPerPixel();
    _defaultCursor = _parent->DefaultCursor()->Clone();
    _currentCursor = _defaultCursor;

    _input = new SubWindowInput(this, _parent->Input());
    _input->Create();

    _parent->AddSubWindow(this);

	_needInitFbo = true;
    _isCreate = true;
}

void    SubWindow::Create(const std::string &, const Math::Vector2ui &size, const Utils::Mask<Style> &, const Utils::FileName &, unsigned int)
{
    Create(size);
}

void    SubWindow::UseExistingWindow(void *, int, const Math::Vector2ui &size, unsigned int)
{
    _width = size.data[0];
    _height = size.data[1];
}

void    SubWindow::Resize(unsigned int width, unsigned int height)
{
    _width = width;
    _height = height;
    Resized();
}

GLContext   *SubWindow::CreateGLContext()
{
    _context = _parent->CreateGLContext();
    return _context;
}

ICursor *SubWindow::NewCursor()
{
    ICursor *c = _parent->NewCursor();
    c->AttachWindow(this);
    return c;
}

void    SubWindow::Resized()
{
    _needInitFbo = true;
}

void    SubWindow::InitFbo()
{
	Vector2ui size(Width(), Height());

    _colorTexture.Create(GL::Enum::Texture::Texture2d);
	_colorTexture.Enable();
	_colorTexture.Init2d(0, GL::Enum::Texture::RGBA8, size, GL::Enum::PixelFormat::RGBA, GL::Enum::PixelDataType::UnsignedByte, NULL);
	_colorTexture.GenerateMipmaps();
	_colorTexture.Disable();

	GL::RenderBuffer depthRenderBuffer;
	depthRenderBuffer.Create();
	depthRenderBuffer.Enable();
	depthRenderBuffer.Init(GL::Enum::RenderBuffer::DepthComponent, size);
	depthRenderBuffer.Disable();

    _fbo.Create();
	_fbo.Enable();
	_fbo.Attach(GL::Enum::FrameBuffer::ColorAttachment0, _colorTexture, 0);
	_fbo.Attach(GL::Enum::FrameBuffer::DepthAttachement, depthRenderBuffer);
	if(_fbo.CheckStatus() != GL::Enum::FrameBuffer::Complete)
		throw Utils::Exception("Can't initialize the fbo");
	_fbo.Disable();
	_needInitFbo = false;
}

void    SubWindow::Render(GLContext *context)
{
    CALLSTACK_INFO();

    // create the fbo at the first pass
	if (_needInitFbo)
		InitFbo();

    _fbo.Enable();
    IWindow::Render(context);
    _fbo.Disable();
}


