
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

    File Created At:        04/07/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "State.h"
#include "../Context/GLContext.h"

using namespace Nc;
using namespace Nc::Graphic::GL;

State           *State::_current = NULL;
unsigned int    State::_instanceCounter = 0;

State::State()
{
    _instanceCounter++;
}

State::~State()
{
    _instanceCounter--;
}

void    State::Enable()
{
    _current = this;
}

void    State::Disable()
{
    _current = NULL;
}

void    State::InitContext(GLContext *context)
{
    _context = context;

    // check ogl context version
    CheckGLVersion();

    // init states
    IWindow *win = _context->AttachedWindow();
    _currentViewportX = 0;
    _currentViewportY = 0;
    _currentViewportWidth = win->Width();
    _currentViewportHeight = win->Height();
    _currentScissorX = 0;
    _currentScissorY = 0;
    _currentScissorWidth = win->Width();
    _currentScissorHeight = win->Height();
    _currentClearColor.Init(0,0,0,0);
    _currentDepthMask = true;
    _currentDepthFunc = Enum::Less;
    _currentBlendSFactor = Enum::One;
    _currentBlendDFactor = Enum::Zero;
    _currentPolygonFace = Enum::FrontAndBack;
    _currentPolygonMode = Enum::Fill;
    _currentPointSize = 1.f;
    _currentLineWidth = 1.f;
    _currentPolygonOffsetFactor = 0.f;
    _currentPolygonOffsetUnits = 0.f;

    _mapCurrentCapabilityStatement[Enum::Blend] = false;
    _mapCurrentCapabilityStatement[Enum::ColorLogicOp] = false;
    _mapCurrentCapabilityStatement[Enum::CullFace] = false;
    _mapCurrentCapabilityStatement[Enum::DepthClamp] = false;
    _mapCurrentCapabilityStatement[Enum::DepthTest] = false;
    _mapCurrentCapabilityStatement[Enum::Dither] = false;
    _mapCurrentCapabilityStatement[Enum::LineSmooth] = false;
    _mapCurrentCapabilityStatement[Enum::Multisample] = false;
    _mapCurrentCapabilityStatement[Enum::PolygonOffsetFill] = false;
    _mapCurrentCapabilityStatement[Enum::PolygonOffsetLine] = false;
    _mapCurrentCapabilityStatement[Enum::PolygonOffsetPoint] = false;
    _mapCurrentCapabilityStatement[Enum::PolygonSmooth] = false;
    _mapCurrentCapabilityStatement[Enum::PrimitiveRestart] = false;
    _mapCurrentCapabilityStatement[Enum::SampleAlphaCoverage] = false;
    _mapCurrentCapabilityStatement[Enum::SampleAlphaToOne] = false;
    _mapCurrentCapabilityStatement[Enum::SampleCoverage] = false;
    _mapCurrentCapabilityStatement[Enum::ScissorTest] = false;
    _mapCurrentCapabilityStatement[Enum::StencilTest] = false;
    _mapCurrentCapabilityStatement[Enum::TextureCubeMapSeamLess] = false;
    _mapCurrentCapabilityStatement[Enum::ProgramPointSize] = false;

    _mapCurrentBufferBound[Enum::DataBuffer::ArrayBuffer] = 0;
    _mapCurrentBufferBound[Enum::DataBuffer::CopyReadBuffer] = 0;
    _mapCurrentBufferBound[Enum::DataBuffer::CopyWriteBuffer] = 0;
    _mapCurrentBufferBound[Enum::DataBuffer::ElementArrayBuffer] = 0;
    _mapCurrentBufferBound[Enum::DataBuffer::PixelPackBuffer] = 0;
    _mapCurrentBufferBound[Enum::DataBuffer::PixelUnpackBuffer] = 0;
    _mapCurrentBufferBound[Enum::DataBuffer::TextureBuffer] = 0;
    _mapCurrentBufferBound[Enum::DataBuffer::TransformFeedbackBuffer] = 0;
    _mapCurrentBufferBound[Enum::DataBuffer::UniformBuffer] = 0;
    _mapCurrentTextureBound[Enum::Texture::Texture1d] = 0;
    _mapCurrentTextureBound[Enum::Texture::Texture2d] = 0;
    _mapCurrentTextureBound[Enum::Texture::Texture3d] = 0;
    _mapCurrentTextureBound[Enum::Texture::Texture1dArray] = 0;
    _mapCurrentTextureBound[Enum::Texture::Texture2dArray] = 0;
    _mapCurrentTextureBound[Enum::Texture::TextureRectangle] = 0;
    _mapCurrentTextureBound[Enum::Texture::TextureCubeMap] = 0;
    _mapCurrentTextureBound[Enum::Texture::Texture2dMultiSample] = 0;
    _mapCurrentTextureBound[Enum::Texture::Texture2dMultisampleArray] = 0;

    _currentDrawFrameBufferBound = 0;
    _currentReadFrameBufferBound = 0;
    _currentRenderBufferBound = 0;
    _currentProgramBound  = 0;
    _currentActiveTextureUnit = 0;

    // Init extention
    if(!EXT.Init())
        throw Utils::Exception("SceneGraphManager", "Can't initialize gl extensions");
}

const GLubyte *State::GetString(Enum::ImplementationDescription type)
{
    const GLubyte   *s = glGetString(type);

    if (s == NULL)
        throw Utils::Exception("GL::State", "glGetString return NULL");
    return s;
}

std::string State::GetError()
{
    GLenum e = glGetError();
    switch (e)
    {
        case GL_NO_ERROR:                       return "[GL_NO_ERROR] No error has been recorded.";
        case GL_INVALID_ENUM:                   return "[GL_INVALID_ENUM] An unacceptable value is specified for an enumerated argument.";
        case GL_INVALID_VALUE:                  return "[GL_INVALID_VALUE] A numeric argument is out of range.";
        case GL_INVALID_OPERATION:              return "[GL_INVALID_OPERATION] The specified operation is not allowed in the current state.";
        case GL_INVALID_FRAMEBUFFER_OPERATION:  return "[GL_INVALID_FRAMEBUFFER_OPERATION] The framebuffer object is not complete.";
        case GL_OUT_OF_MEMORY:                  return "[GL_OUT_OF_MEMORY] There is not enough memory left to execute the command.";
        default:                                return "[" + Utils::Convert::ToString(e) + "] Unknown error.";
    }
}


namespace Nc {
    namespace Graphic {
        namespace GL {
            std::ostream &operator << (std::ostream &oss, State &s)
            {
                oss << "GL::State: using context                " << s._context << std::endl;
                oss << "Viewport:                               " << s._currentViewportX << "\t" << s._currentViewportY << "\t" << s._currentViewportWidth << "\t" << s._currentViewportHeight << std::endl;
                oss << "Scissor:                                " << s._currentScissorX << "\t" << s._currentScissorY << "\t" << s._currentScissorWidth << "\t" << s._currentScissorHeight << std::endl;
                oss << "ClearColor:                             " << s._currentClearColor << std::endl;
                oss << "DepthMask:                              " << ((s._currentDepthMask) ? "True" : "False") << std::endl;
                oss << "DepthFunc:                              " << s._currentDepthFunc << std::endl;
                oss << "BlendFactor:                            " << s._currentBlendSFactor << "\tD: " << s._currentBlendDFactor << std::endl;
                oss << "PolygonFace:                            " << s._currentPolygonFace << std::endl;
                oss << "PolygonMode:                            " << s._currentPolygonMode << std::endl;
                oss << "PointSize:                              " << s._currentPointSize << std::endl;
                oss << "LineWidth:                              " << s._currentLineWidth << std::endl;
                oss << "PolygonOffsetFactor:                    " << s._currentPolygonOffsetFactor << std::endl;
                oss << "PolygonOffsetUnits:                     " << s._currentPolygonOffsetUnits << std::endl;
                oss << "_currentActiveTextureUnit:              " << s._currentActiveTextureUnit << std::endl;

                oss << "Capability[Blend]:                      " << ((s._mapCurrentCapabilityStatement[Enum::Blend]) ? "True" : "False") << std::endl;
                oss << "Capability[ColorLogicOp]:               " << ((s._mapCurrentCapabilityStatement[Enum::ColorLogicOp]) ? "True" : "False") << std::endl;
                oss << "Capability[CullFace]:                   " << ((s._mapCurrentCapabilityStatement[Enum::CullFace]) ? "True" : "False") << std::endl;
                oss << "Capability[DepthClamp]:                 " << ((s._mapCurrentCapabilityStatement[Enum::DepthClamp]) ? "True" : "False") << std::endl;
                oss << "Capability[DepthTest]:                  " << ((s._mapCurrentCapabilityStatement[Enum::DepthTest]) ? "True" : "False") << std::endl;
                oss << "Capability[Dither]:                     " << ((s._mapCurrentCapabilityStatement[Enum::Dither]) ? "True" : "False") << std::endl;
                oss << "Capability[LineSmooth]:                 " << ((s._mapCurrentCapabilityStatement[Enum::LineSmooth]) ? "True" : "False") << std::endl;
                oss << "Capability[Multisample]:                " << ((s._mapCurrentCapabilityStatement[Enum::Multisample]) ? "True" : "False") << std::endl;
                oss << "Capability[PolygonOffsetFill]:          " << ((s._mapCurrentCapabilityStatement[Enum::PolygonOffsetFill]) ? "True" : "False") << std::endl;
                oss << "Capability[PolygonOffsetLine]:          " << ((s._mapCurrentCapabilityStatement[Enum::PolygonOffsetLine]) ? "True" : "False") << std::endl;
                oss << "Capability[PolygonOffsetPoint]:         " << ((s._mapCurrentCapabilityStatement[Enum::PolygonOffsetPoint]) ? "True" : "False") << std::endl;
                oss << "Capability[PolygonSmooth]:              " << ((s._mapCurrentCapabilityStatement[Enum::PolygonSmooth]) ? "True" : "False") << std::endl;
                oss << "Capability[PrimitiveRestart]:           " << ((s._mapCurrentCapabilityStatement[Enum::PrimitiveRestart]) ? "True" : "False") << std::endl;
                oss << "Capability[SampleAlphaCoverage]:        " << ((s._mapCurrentCapabilityStatement[Enum::SampleAlphaCoverage]) ? "True" : "False") << std::endl;
                oss << "Capability[SampleAlphaToOne]:           " << ((s._mapCurrentCapabilityStatement[Enum::SampleAlphaToOne]) ? "True" : "False") << std::endl;
                oss << "Capability[SampleCoverage]:             " << ((s._mapCurrentCapabilityStatement[Enum::SampleCoverage]) ? "True" : "False") << std::endl;
                oss << "Capability[ScissorTest]:                " << ((s._mapCurrentCapabilityStatement[Enum::ScissorTest]) ? "True" : "False") << std::endl;
                oss << "Capability[StencilTest]:                " << ((s._mapCurrentCapabilityStatement[Enum::StencilTest]) ? "True" : "False") << std::endl;
                oss << "Capability[TextureCubeMapSeamLess]:     " << ((s._mapCurrentCapabilityStatement[Enum::TextureCubeMapSeamLess]) ? "True" : "False") << std::endl;
                oss << "Capability[ProgramPointSize]:           " << ((s._mapCurrentCapabilityStatement[Enum::ProgramPointSize]) ? "True" : "False") << std::endl;

                oss << "Bound[ArrayBuffer]:                     " << s._mapCurrentBufferBound[Enum::DataBuffer::ArrayBuffer] << std::endl;
                oss << "Bound[CopyReadBuffer]:                  " << s._mapCurrentBufferBound[Enum::DataBuffer::CopyReadBuffer] << std::endl;
                oss << "Bound[CopyWriteBuffer]:                 " << s._mapCurrentBufferBound[Enum::DataBuffer::CopyWriteBuffer] << std::endl;
                oss << "Bound[ElementArrayBuffer]:              " << s._mapCurrentBufferBound[Enum::DataBuffer::ElementArrayBuffer] << std::endl;
                oss << "Bound[PixelPackBuffer]:                 " << s._mapCurrentBufferBound[Enum::DataBuffer::PixelPackBuffer] << std::endl;
                oss << "Bound[PixelUnpackBuffer]:               " << s._mapCurrentBufferBound[Enum::DataBuffer::PixelUnpackBuffer] << std::endl;
                oss << "Bound[TextureBuffer]:                   " << s._mapCurrentBufferBound[Enum::DataBuffer::TextureBuffer] << std::endl;
                oss << "Bound[TransformFeedbackBuffer]:         " << s._mapCurrentBufferBound[Enum::DataBuffer::TransformFeedbackBuffer] << std::endl;
                oss << "Bound[UniformBuffer]:                   " << s._mapCurrentBufferBound[Enum::DataBuffer::UniformBuffer] << std::endl;
                oss << "Bound[Texture1D]:                       " << s._mapCurrentTextureBound[Enum::Texture::Texture1d] << std::endl;
                oss << "Bound[Texture2D]:                       " << s._mapCurrentTextureBound[Enum::Texture::Texture2d] << std::endl;
                oss << "Bound[Texture3D]:                       " << s._mapCurrentTextureBound[Enum::Texture::Texture3d] << std::endl;
                oss << "Bound[Texture1DArray]:                  " << s._mapCurrentTextureBound[Enum::Texture::Texture1dArray] << std::endl;
                oss << "Bound[Texture2DArray]:                  " << s._mapCurrentTextureBound[Enum::Texture::Texture2dArray] << std::endl;
                oss << "Bound[TextureRectangle]:                " << s._mapCurrentTextureBound[Enum::Texture::TextureRectangle] << std::endl;
                oss << "Bound[TextureCubeMap]:                  " << s._mapCurrentTextureBound[Enum::Texture::TextureCubeMap] << std::endl;
                oss << "Bound[Texture2DMultiSample]:            " << s._mapCurrentTextureBound[Enum::Texture::Texture2dMultiSample] << std::endl;
                oss << "Bound[Texture2DMultisampleArray]:       " << s._mapCurrentTextureBound[Enum::Texture::Texture2dMultisampleArray] << std::endl;
                oss << "Bound[DrawFrameBuffer]:                 " << s._currentDrawFrameBufferBound << std::endl;
                oss << "Bound[ReadFrameBuffer]:                 " << s._currentReadFrameBufferBound << std::endl;
                oss << "Bound[RenderBuffer]:                    " << s._currentRenderBufferBound << std::endl;
                oss << "Bound[CurrentProgramBound]:             " << s._currentProgramBound << std::endl;
                return oss;
            }
        }
    }
}

void    State::CheckGLVersion()
{
    // on a linux system, we check if the graphic acceleration is support and enable
    #ifdef SYSTEM_LINUX
    LOG << "Direct Rendering : \t\t\t\t\t";
    char *alwaysIndirect = getenv("LIBGL_ALWAYS_INDIRECT");     // if the `LIBGL_ALWAYS_INDIRECT` is set, no graphic acceleration
    if (alwaysIndirect != NULL && std::string(alwaysIndirect) == "1")
    {
        LOG << "Bad" << std::endl;
        System::Config::Error("GraphicEngine", "Graphic acceleration disabled.\nPlease check that. On compiz, you could try to unset LIBGL_ALWAYS_INDIRECT");
    }
    else
        LOG << "Ok" << std::endl;
    #endif

    // check la version actuel d'opengl
    std::string version((const char*)(GetString(Enum::Version)));
    LOG << "GL_VERSION = `" << version << "`" << std::endl;
	LOG << "GL_VENDOR = `" << GetString(Enum::Vendor) << "`" << std::endl;
    LOG << "GL_RENDERER = `" << GetString(Enum::Renderer) << "`" << std::endl;
    //LOG << "GL_EXTENSIONS = `" << EXT.GetInfo(Enum::Extentions) << "`" << std::endl;
    try
        {LOG << "GL_SHADING_LANGUAGE_VERSION = `" << GetString(Enum::ShadingLanguageVersion) << "`" << std::endl;}
    catch (...)
        {System::Config::Error("GraphicEngine", "Failed to fetch GL_SHADING_LANGUAGE_VERSION, Shaders is probably not supported");}

    float   nbr = 0;
    Utils::Convert::StringTo(version, nbr);
    if (nbr < VERSION_MIN_OPENGL)
    {
        System::Config::Error("GraphicEngine", "Bad OpenGl version, minimum is '" + Utils::Convert::ToString(VERSION_MIN_OPENGL) + "'\nPlease upgrade your opengl driver");
    }
    else
        LOG << "OpenGL version OK" << std::endl;
}

void    State::Viewport(int viewportX, int viewportY, int viewportWidth, int viewportHeight)
{
    if (_currentViewportX != viewportX || _currentViewportY != viewportY ||
        _currentViewportWidth != viewportWidth || _currentViewportHeight != viewportHeight)
    {
        glViewport(viewportX, viewportY, viewportWidth, viewportHeight);
        _currentViewportX = viewportX;
        _currentViewportY = viewportY;
        _currentViewportWidth = viewportWidth;
        _currentViewportHeight = viewportHeight;
    }
}

void    State::ClearColor(Color c)
{
    if (_currentClearColor != c)
    {
        glClearColor(c.r, c.g, c.b, c.a);
        _currentClearColor = c;
    }
}

void    State::Enable(Enum::Capability cp)
{
    MapCapabilityStatement::iterator it = _mapCurrentCapabilityStatement.find(cp);
    if (it->second != true)
    {
        glEnable(cp);
        it->second = true;
    }
}

void    State::Disable(Enum::Capability cp)
{
    MapCapabilityStatement::iterator it = _mapCurrentCapabilityStatement.find(cp);
    if (it->second != false)
    {
        glDisable(cp);
        it->second = false;
    }
}

void    State::DepthFunc(Enum::DepthFunc f)
{
    if (_currentDepthFunc != f)
    {
        glDepthFunc(f);
        _currentDepthFunc = f;
    }
}

void    State::PolygonMode(Enum::PolygonFace f, Enum::PolygonMode m)
{
    if (f != _currentPolygonFace || m != _currentPolygonMode)
    {
        glPolygonMode(f, m);
        _currentPolygonFace = f;
        _currentPolygonMode = m;
    }
}

void    State::PolygonOffset(float offsetFactor, float offsetUnits)
{
    if (offsetFactor != _currentPolygonOffsetFactor || offsetUnits != _currentPolygonOffsetUnits)
    {
        glPolygonOffset(offsetFactor, offsetUnits);
        _currentPolygonOffsetFactor = offsetFactor;
        _currentPolygonOffsetUnits = offsetUnits;
    }
}

void    State::PointSize(float s)
{
    if (s != _currentPointSize)
    {
        glPointSize(s);
        _currentPointSize = s;
    }
}

void    State::LineWidth(float w)
{
    if (w != _currentLineWidth)
    {
        glLineWidth(w);
        _currentLineWidth = w;
    }
}

void    State::DepthMask(bool state)
{
    if (_currentDepthMask != state)
    {
        glDepthMask(state);
        _currentDepthMask = state;
    }
}

void    State::BlendFunc(Enum::BlendFactor s, Enum::BlendFactor d)
{
    if (_currentBlendSFactor != s || _currentBlendDFactor != d)
    {
        glBlendFunc(s, d);
        _currentBlendSFactor = s;
        _currentBlendDFactor = d;
    }
}

void    State::Bind(Enum::DataBuffer::Target target, unsigned int id)
{
    MapBufferBound::iterator it = _mapCurrentBufferBound.find(target);
    if (it->second != id)
    {
        glBindBuffer(target, id);
        it->second = id;
    }
}

void    State::Bind(Enum::Texture::Target target, unsigned int id)
{
    MapTextureBound::iterator it = _mapCurrentTextureBound.find(target);
    if (it->second != id)
    {
        glBindTexture(target, id);
        it->second = id;
    }
}

void    State::Bind(Enum::FrameBuffer::Target target, unsigned int id)
{
    if (target == Enum::FrameBuffer::FrameBuffer)
    {
        if (_currentDrawFrameBufferBound != id || _currentReadFrameBufferBound != id)
        {
            glBindFramebuffer(target, id);
            _currentDrawFrameBufferBound = id;
            _currentReadFrameBufferBound = id;
        }
    }
    else if (target == Enum::FrameBuffer::DrawFrameBuffer && _currentDrawFrameBufferBound != id)
    {
        glBindFramebuffer(target, id);
        _currentDrawFrameBufferBound = id;
    }
    else if (target == Enum::FrameBuffer::ReadFrameBuffer && _currentReadFrameBufferBound != id)
    {
        glBindFramebuffer(target, id);
        _currentReadFrameBufferBound = id;
    }
}

void    State::Bind(Enum::RenderBuffer::Target target, unsigned int id)
{
    if (_currentRenderBufferBound != id)
    {
        glBindRenderbuffer(target, id);
        _currentRenderBufferBound = id;
    }
}

void    State::BindProgram(unsigned int id)
{
    if (_currentProgramBound != id)
    {
        glUseProgram(id);
        _currentProgramBound = id;
    }
}

void    State::Unbind(Enum::DataBuffer::Target target)
{
    MapBufferBound::iterator it = _mapCurrentBufferBound.find(target);
    if (it->second != 0)
    {
        glBindBuffer(target, 0);
        it->second = 0;
    }
}

void    State::Unbind(Enum::Texture::Target target)
{
    MapTextureBound::iterator it = _mapCurrentTextureBound.find(target);
    if (it->second != 0)
    {
        glBindTexture(target, 0);
        it->second = 0;
    }
}

void    State::Unbind(Enum::FrameBuffer::Target target)
{
    if (target == Enum::FrameBuffer::FrameBuffer)
    {
        if (_currentDrawFrameBufferBound != 0 || _currentReadFrameBufferBound != 0)
        {
            glBindFramebuffer(target, 0);
            _currentDrawFrameBufferBound = 0;
            _currentReadFrameBufferBound = 0;
        }
    }
    else if (target == Enum::FrameBuffer::DrawFrameBuffer && _currentDrawFrameBufferBound != 0)
    {
        glBindFramebuffer(target, 0);
        _currentDrawFrameBufferBound = 0;
    }
    else if (target == Enum::FrameBuffer::ReadFrameBuffer && _currentReadFrameBufferBound != 0)
    {
        glBindFramebuffer(target, 0);
        _currentReadFrameBufferBound = 0;
    }
}

void    State::Unbind(Enum::RenderBuffer::Target target)
{
    if (_currentRenderBufferBound != 0)
    {
        glBindRenderbuffer(target, 0);
        _currentRenderBufferBound = 0;
    }
}

void    State::UnbindProgram()
{
    if (_currentProgramBound != 0)
    {
        glUseProgram(0);
        _currentProgramBound = 0;
    }
}

void    State::ActiveTexture(unsigned int no)
{
    if (_currentActiveTextureUnit != no)
    {
        glActiveTexture(GL_TEXTURE0 + no);
        _currentActiveTextureUnit = no;
    }
}

void    State::Scissor(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
    if (_currentScissorX != x || _currentScissorY != y ||
        _currentScissorWidth != width || _currentScissorHeight != height)
    {
        glScissor(x, y, width, height);
        _currentScissorX = x;
        _currentScissorY = y;
        _currentScissorWidth = width;
        _currentScissorHeight = height;
    }
}
