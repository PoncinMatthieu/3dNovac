
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

    File Created At:        06/09/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include <fstream>
#include "Shader.h"

using namespace std;
using namespace Nc;
using namespace Nc::Graphic::GL;

Shader::Shader()
{
    _vertexShader = 0;
    _fragmentShader = 0;
    _program = 0;
}

Shader::Shader(const std::string &vertexFile, const std::string &fragmentFile)
{
    LoadFromFiles(vertexFile, fragmentFile);
}

Shader::~Shader()
{
    ReleaseRef();
}

void Shader::LoadFromFiles(const std::string &vertexFile, const std::string &fragmentFile)
{
    NewRef();
    _vertexShader = (!vertexFile.empty()) ? NewShader(vertexFile, GL_VERTEX_SHADER) : 0;
    _fragmentShader = (!fragmentFile.empty()) ? NewShader(fragmentFile, GL_FRAGMENT_SHADER) : 0;
    _program = NewProgramShader();
    AttachAndLinkProgram(_program, _vertexShader, _fragmentShader);
}

void Shader::LoadFromMemory(const char *vertexSource, const char *fragmentSource, const std::string &name)
{
    NewRef();
    _vertexShader = (vertexSource != NULL) ? CompileShader(vertexSource, GL_VERTEX_SHADER, name) : 0;
    _fragmentShader = (fragmentSource != NULL) ? CompileShader(fragmentSource, GL_FRAGMENT_SHADER, name) : 0;
    _program = NewProgramShader();
    AttachAndLinkProgram(_program, _vertexShader, _fragmentShader);
}

void    Shader::Release()
{
    if (_vertexShader != 0)
        glDeleteShader(_vertexShader);
    if (_fragmentShader != 0)
        glDeleteShader(_fragmentShader);
    if (_program != 0)
        glDeleteProgram(_program);
}

unsigned int    Shader::NewShader(const Utils::FileName &filename, GLenum type)
{
    //envoie du code source au shader
    ifstream file(filename.c_str(), ios::in);
    if(!file)
        throw Utils::Exception("Shader", "Can't open the source file GLSL " + filename);
    string source;
    while (!file.eof())
        source.push_back(file.get());
    file.close();
    return CompileShader(source.c_str(), type, filename.Filename());
}

unsigned int    Shader::CompileShader(const char *source, GLenum type, const std::string &name)
{
    unsigned int shader;
    if ((shader = glCreateShader(type)) == 0)
        throw Utils::Exception("Shader", "Can't create the shader !");
    glShaderSource(shader, 1, &source, NULL);

    //compilation du shader
    LOG << "Compile `" << name << "`...\t\t\t";
    int compile_status = 1;
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
    if (!compile_status)
    {
        int error_size;
        char *error;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &error_size);
        error = new char[error_size];
        glGetShaderInfoLog(shader, error_size, &error_size, error);
        std::string se(error);
        delete[] error;
        throw Utils::Exception("Shader", se);
    }
    LOG << "DONE" << std::endl;
    return shader;
}

unsigned int    Shader::NewProgramShader()
{
    unsigned int program;

    if ((program = glCreateProgram()) == 0)
        throw Utils::Exception("Shader", "Can't create the program shader");
    return program;
}

void            Shader::AttachAndLinkProgram(unsigned int program, unsigned int vertexShader, unsigned int fragmentShader)
{
    if (vertexShader != 0)
        glAttachShader(program, vertexShader);
    if (fragmentShader != 0)
        glAttachShader(program, fragmentShader);
    LOG << "Link program shader\t\t\t\t\t";
    glLinkProgram(program);
    int linkStatus = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if (!linkStatus)
    {
        int lenght;
        char *error;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &lenght);
        error = new char[lenght];
        glGetProgramInfoLog(program, lenght, &lenght, error);
        std::string s(error);
        delete[] error;
        throw Utils::Exception("Shader", s);
    }
    LOG << "DONE" << std::endl;
}

unsigned int    Shader::UniformLocation(const char *name)
{
    int location = glGetUniformLocation(_program, name);
    if (location == -1)
        throw Utils::Exception("Shader", "Bad Uniform location `" + std::string(name) + "`");
    return location;
}

unsigned int    Shader::AttribLocation(const char *name)
{
    int location = glGetAttribLocation(_program, name);
    if (location == -1)
        throw Utils::Exception("Shader", "Bad Attrib location `" + std::string(name) + "`");
    return location;
}

void Shader::BindAttrib(unsigned int attrib, const char *name)
{
	glBindAttribLocation(_program, attrib, name);
}
