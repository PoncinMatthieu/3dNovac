
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
    if (_program != 0)
        glDeleteProgram(_program);
    LOG_DEBUG << "Program " << _program << " DELETED" << std::endl;
    _linked = false;
    _program = 0;
}

void     Program::Enable() const
{
    if (_program == 0 || !_linked)
        throw Utils::Exception("GL::Program", "The program hasn't been linked.");
    if (State::IsSet())
        State::Current().BindProgram(_program);
    else
        glUseProgram(_program);
}

void     Program::Disable() const
{
/*
    if (State::IsSet())
        State::Current().UnbindProgram();
    else
        glUseProgram(0);
*/
}

void    Program::Attach(const Shader &shader)
{
    if (_program == 0)
    {
        NewRef();
        if ((_program = glCreateProgram()) == 0)
            throw Utils::Exception("Shader", "Can't create the program shader");
        LOG_DEBUG << "Program " << _program << " CREATED" << std::endl;
    }
    if (shader.IsValid())
        glAttachShader(_program, shader.GetIndex());
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
    if (!IsValid())
        throw Utils::Exception("GL::Program", "There is no attached shader");
    LOG_DEBUG << "Link program shader " << _program << "\t\t\t\t\t";
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
        throw Utils::Exception("Shader", s);
    }
    _linked = true;
    LOG_DEBUG << "DONE" << std::endl;
}

unsigned int    Program::GetUniformLocation(const char *name)
{
    int location = glGetUniformLocation(_program, name);
    if (location == -1)
        throw Utils::Exception("Shader", "Bad Uniform location `" + std::string(name) + "`");
    return location;
}

unsigned int    Program::GetAttribLocation(const char *name)
{
    int location = glGetAttribLocation(_program, name);
    if (location == -1)
        throw Utils::Exception("Shader", "Bad Attrib location `" + std::string(name) + "`");
    return location;
}

void Program::BindAttrib(unsigned int attrib, const char *name)
{
	glBindAttribLocation(_program, attrib, name);
}
