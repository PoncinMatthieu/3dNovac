
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

    File Created At:        03/07/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "Program.h"

using namespace Nc;
using namespace Nc::Graphic::GL;

Program::Program()
    : _program(0), _linked(false)
{
}

Program::~Program()
{
    ReleaseRef();
}

void    Program::Release()
{
    CALLSTACK_INFO_ARG("program: " + Utils::Convert::ToString(_program));
    if (_program != 0)
    {
        State::Instance().UnbindProgram();
        glDeleteProgram(_program);
    }
    NC_GRAPHIC_GL_CHECK_ERROR();
    _linked = false;
    _program = 0;
}

void     Program::Enable() const
{
	if (_program == 0 || !_linked)
        throw Utils::Exception("GL::Program::Enable", "The program hasn't been linked.");
    State::Instance().UseProgram(_program);
}

void     Program::Disable() const
{
/*
    State::Instance().UnbindProgram();
*/
}

void    Program::Attach(const Shader &shader)
{
    CALLSTACK_INFO();
    if (_program == 0)
    {
        NewRef();
        if ((_program = glCreateProgram()) == 0)
            throw Utils::Exception("GL::Program::Attach", "Can't create the program shader");
        CALLSTACK_APPEND_INFO("New program created: " + Utils::Convert::ToString(_program));
        NC_GRAPHIC_GL_CHECK_ERROR();
    }
    if (shader.IsValid())
        glAttachShader(_program, shader.GetIndex());
    NC_GRAPHIC_GL_CHECK_ERROR();
}

void    Program::Attach(const Utils::FileName &shaderFile, Enum::ShaderType type)
{
    Attach(Shader(shaderFile, type));
}

void    Program::Attach(const char *shaderSource, Enum::ShaderType type, const Utils::FileName &shaderName)
{
    Attach(Shader(shaderSource, type, shaderName));
}

void    Program::Link()
{
    CALLSTACK_INFO();
    if (!IsValid())
        throw Utils::Exception("GL::Program::Link", "There is no attached shader");
    glLinkProgram(_program);

    // check link error
    int linkStatus = 0;
    glGetProgramiv(_program, GL_LINK_STATUS, &linkStatus);
    if (!linkStatus)
    {
        int     lenght;
        char    *error;
        glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &lenght);
        error = new char[lenght];
        glGetProgramInfoLog(_program, lenght, &lenght, error);
        std::string s(error);
        delete[] error;
        throw Utils::Exception("GL::Program::Link", s);
    }
    _linked = true;
    NC_GRAPHIC_GL_CHECK_ERROR();
}

unsigned int    Program::GetUniformLocation(const char *name)
{
    int location = glGetUniformLocation(_program, name);
    if (location == -1)
        LOG_ERROR << "GL::Program: Bad Uniform location `" << name << "`" << std::endl;
    return location;
}

unsigned int    Program::GetAttribLocation(const char *name)
{
    int location = glGetAttribLocation(_program, name);
    if (location == -1)
        LOG_ERROR << "GL::Program: Bad Attrib location `" << name << "`" << std::endl;
    return location;
}

void Program::BindAttrib(unsigned int attrib, const char *name)
{
    CALLSTACK_INFO();
	glBindAttribLocation(_program, attrib, name);
	NC_GRAPHIC_GL_CHECK_ERROR();
}

void    Program::SetUniform(unsigned int location, const TMatrix &m, bool transpose)
{
    CALLSTACK_INFO_ARG("location: " + Utils::Convert::ToString(location));
    glUniformMatrix4fv(location, 1, transpose, m.Elements());
    NC_GRAPHIC_GL_CHECK_ERROR();
}

void    Program::SetUniform(unsigned int location, int v)
{
    CALLSTACK_INFO_ARG("location: " + Utils::Convert::ToString(location));
    glUniform1i(location, v);
    NC_GRAPHIC_GL_CHECK_ERROR();
}

void    Program::SetUniform(unsigned int location, unsigned int v)
{
    CALLSTACK_INFO_ARG("location: " + Utils::Convert::ToString(location));
    glUniform1ui(location, v);
    NC_GRAPHIC_GL_CHECK_ERROR();
}

void    Program::SetUniform(unsigned int location, float v)
{
    CALLSTACK_INFO_ARG("location: " + Utils::Convert::ToString(location));
    glUniform1f(location, v);
    NC_GRAPHIC_GL_CHECK_ERROR();
}

void    Program::SetUniform(unsigned int location, float f1, float f2)
{
    CALLSTACK_INFO_ARG("location: " + Utils::Convert::ToString(location));
    glUniform2f(location, f1, f2);
    NC_GRAPHIC_GL_CHECK_ERROR();
}

void    Program::SetUniform(unsigned int location, const Vector2f &v)
{
    CALLSTACK_INFO_ARG("location: " + Utils::Convert::ToString(location));
    glUniform2f(location, v.data[0], v.data[1]);
    NC_GRAPHIC_GL_CHECK_ERROR();
}

void    Program::SetUniform(unsigned int location, float f1, float f2, float f3)
{
    CALLSTACK_INFO_ARG("location: " + Utils::Convert::ToString(location));
    glUniform3f(location, f1, f2, f3);
    NC_GRAPHIC_GL_CHECK_ERROR();
}

void    Program::SetUniform(unsigned int location, const Vector3f &v)
{
    CALLSTACK_INFO_ARG("location: " + Utils::Convert::ToString(location));
    glUniform3f(location, v.data[0], v.data[1], v.data[2]);
    NC_GRAPHIC_GL_CHECK_ERROR();
}

void    Program::SetUniform(unsigned int location, float f1, float f2, float f3, float f4)
{
    CALLSTACK_INFO_ARG("location: " + Utils::Convert::ToString(location));
    glUniform4f(location, f1, f2, f3, f4);
    NC_GRAPHIC_GL_CHECK_ERROR();
}

