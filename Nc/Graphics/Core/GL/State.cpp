
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
#include "../../Window/Context/GLContext.h"

using namespace Nc;
using namespace Nc::Graphic::GL;

State           *State::_current = NULL;
System::Mutex   State::_mutex;
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
    if (_instanceCounter != 1)
        _mutex.Lock();
    _current = this;
}

void    State::Disable()
{
    _current = NULL;
    if (_instanceCounter != 1)
        _mutex.Unlock();
}

void    State::InitContext(GLContext *context)
{
    _context = context;

    // check ogl context version
    CheckGLVersion();

    // init states
    Window *win = _context->AttachedWindow();
    _currentViewportX = 0;
    _currentViewportY = 0;
    _currentViewportWidth = win->Width();
    _currentViewportHeight = win->Height();
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

    // Init extention
    if(!EXT.Init())
        throw Utils::Exception("SceneGraphManager", "Can't initialize gl extensions");
}

namespace Nc {
    namespace Graphic {
        namespace GL {
            std::ostream &operator << (std::ostream &oss, State &s)
            {
                oss << "GL::State: using context                        " << s._context << std::endl;
                oss << "ViewportX:                                      " << s._currentViewportX << std::endl;
                oss << "ViewportY:                                      " << s._currentViewportY << std::endl;
                oss << "ViewportWidth:                                  " << s._currentViewportWidth << std::endl;
                oss << "ViewportHeight:                                 " << s._currentViewportHeight << std::endl;
                oss << "CapabilityStatement[Blend]:                     " << s._mapCurrentCapabilityStatement[Enum::Blend] << std::endl;
                oss << "CapabilityStatement[ColorLogicOp]:              " << s._mapCurrentCapabilityStatement[Enum::ColorLogicOp] << std::endl;
                oss << "CapabilityStatement[CullFace]:                  " << s._mapCurrentCapabilityStatement[Enum::CullFace] << std::endl;
                oss << "CapabilityStatement[DepthClamp]:                " << s._mapCurrentCapabilityStatement[Enum::DepthClamp] << std::endl;
                oss << "CapabilityStatement[DepthTest]:                 " << s._mapCurrentCapabilityStatement[Enum::DepthTest] << std::endl;
                oss << "CapabilityStatement[Dither]:                    " << s._mapCurrentCapabilityStatement[Enum::Dither] << std::endl;
                oss << "CapabilityStatement[LineSmooth]:                " << s._mapCurrentCapabilityStatement[Enum::LineSmooth] << std::endl;
                oss << "CapabilityStatement[Multisample]:               " << s._mapCurrentCapabilityStatement[Enum::Multisample] << std::endl;
                oss << "CapabilityStatement[PolygonOffsetFill]:         " << s._mapCurrentCapabilityStatement[Enum::PolygonOffsetFill] << std::endl;
                oss << "CapabilityStatement[PolygonOffsetLine]:         " << s._mapCurrentCapabilityStatement[Enum::PolygonOffsetLine] << std::endl;
                oss << "CapabilityStatement[PolygonOffsetPoint]:        " << s._mapCurrentCapabilityStatement[Enum::PolygonOffsetPoint] << std::endl;
                oss << "CapabilityStatement[PolygonSmooth]:             " << s._mapCurrentCapabilityStatement[Enum::PolygonSmooth] << std::endl;
                oss << "CapabilityStatement[PrimitiveRestart]:          " << s._mapCurrentCapabilityStatement[Enum::PrimitiveRestart] << std::endl;
                oss << "CapabilityStatement[SampleAlphaCoverage]:       " << s._mapCurrentCapabilityStatement[Enum::SampleAlphaCoverage] << std::endl;
                oss << "CapabilityStatement[SampleAlphaToOne]:          " << s._mapCurrentCapabilityStatement[Enum::SampleAlphaToOne] << std::endl;
                oss << "CapabilityStatement[SampleCoverage]:            " << s._mapCurrentCapabilityStatement[Enum::SampleCoverage] << std::endl;
                oss << "CapabilityStatement[ScissorTest]:               " << s._mapCurrentCapabilityStatement[Enum::ScissorTest] << std::endl;
                oss << "CapabilityStatement[StencilTest]:               " << s._mapCurrentCapabilityStatement[Enum::StencilTest] << std::endl;
                oss << "CapabilityStatement[TextureCubeMapSeamLess]:    " << s._mapCurrentCapabilityStatement[Enum::TextureCubeMapSeamLess] << std::endl;
                oss << "CapabilityStatement[ProgramPointSize]:          " << s._mapCurrentCapabilityStatement[Enum::ProgramPointSize] << std::endl;
                oss << "ClearColor:                                     " << s._currentClearColor << std::endl;
                oss << "DepthMask:                                      " << s._currentDepthMask << std::endl;
                oss << "DepthFunc:                                      " << s._currentDepthFunc << std::endl;
                oss << "BlendSFactor:                                   " << s._currentBlendSFactor << std::endl;
                oss << "BlendDFactor:                                   " << s._currentBlendDFactor << std::endl;
                oss << "PolygonFace:                                    " << s._currentPolygonFace << std::endl;
                oss << "PolygonMode:                                    " << s._currentPolygonMode << std::endl;
                oss << "PointSize:                                      " << s._currentPointSize << std::endl;
                oss << "LineWidth:                                      " << s._currentLineWidth << std::endl;
                oss << "PolygonOffsetFactor:                            " << s._currentPolygonOffsetFactor << std::endl;
                oss << "PolygonOffsetUnits:                             " << s._currentPolygonOffsetUnits << std::endl;
                return oss;
            }
        }
    }
}

void    State::CheckGLVersion()
{
    float   nbr;

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
    std::string version((const char*)(EXT.GetInfo(GL_VERSION)));
    LOG << "GL_VERSION = `" << version << "`\t";
    Utils::Convert::StringTo(version, nbr);
    if (nbr < VERSION_MIN_OPENGL)
    {
        LOG << "Bad" << std::endl;
        System::Config::Error("GraphicEngine", "Bad OpenGl version, minimum is '" + Utils::Convert::ToString(VERSION_MIN_OPENGL) + "'\nPlease upgrade your opengl driver");
    }
    else
        LOG << "OK" << std::endl;
    LOG << "GL_VENDOR = `" << EXT.GetInfo(GL_VENDOR) << "`" << std::endl;
    LOG << "GL_RENDERER = `" << EXT.GetInfo(GL_RENDERER) << "`" << std::endl;
    //LOG << "GL_EXTENSIONS = `" << EXT.GetInfo(GL_EXTENSIONS) << "`" << std::endl;
    try
        {LOG << "GL_SHADING_LANGUAGE_VERSION = `" << EXT.GetInfo(GL_SHADING_LANGUAGE_VERSION) << "`" << std::endl;}
    catch (...)
        {System::Config::Error("GraphicEngine", "Failed to fetch GL_SHADING_LANGUAGE_VERSION, Shaders is probably not supported");}
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
        glClearColor(c.R, c.G, c.B, c.A);
        _currentClearColor = c;
    }
}

void    State::Enable(Enum::Capability cp)
{
    if (_mapCurrentCapabilityStatement[cp] != true)
    {
        glEnable(cp);
        _mapCurrentCapabilityStatement[cp] = true;
    }
}

void    State::Disable(Enum::Capability cp)
{
    if (_mapCurrentCapabilityStatement[cp] != false)
    {
        glDisable(cp);
        _mapCurrentCapabilityStatement[cp] = false;
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
