
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
    : _shader(0)
{
}

Shader::Shader(const Utils::FileName &file, Enum::ShaderType type)
    : _shader(0)
{
    LoadFromFile(file, type);
}

Shader::Shader(const char *source, Enum::ShaderType type, const Utils::FileName &name)
    : _shader(0)
{
    LoadFromMemory(source, type, name);
}

Shader::~Shader()
{
    ReleaseRef();
}

void    Shader::Release()
{
    if (_shader != 0)
        glDeleteShader(_shader);
    LOG_DEBUG << "Shader " << _shader << " DELETED" << std::endl;
}

void Shader::LoadFromFile(const Utils::FileName &filename, Enum::ShaderType type)
{
    if (filename.empty() || !filename.IsReadable())
        throw Utils::Exception("Shader", "Can't open the source file GLSL " + filename);

    NewRef();
    _shader = 0;

    // open the file
	// be careful, we open the file in binary mode, because if the file is in "CRLF" format the function tellg return the good size
	// but the function read will bypass the "CRLF" and the size won't match.
	// another solution would have been to fill the buffer of zero before reading.
	ifstream file(filename.c_str(), ios_base::binary | ios::in);

    // get size file
	long pos = file.tellg();
    file.seekg(0, ios_base::end);
    long size = file.tellg();
    file.seekg(pos, ios_base::beg);

    // read file
    char *source = new char[size + 1];
    file.read(source, size);
    source[size] = '\0';
    file.close();

    // create the shader
    if ((_shader = glCreateShader(type)) == 0)
        throw Utils::Exception("Shader", "Can't create the shader !");
    glShaderSource(_shader, 1, (const GLchar**)&source, NULL);
    LOG_DEBUG << "Shader " << _shader << " CREATED" << std::endl;

    // compile the source
    Compile(source, type, filename);
	delete source;
}

void Shader::LoadFromMemory(const char *source, Enum::ShaderType type, const Utils::FileName &name)
{
    NewRef();
    _shader = 0;

    // create the shader
    if ((_shader = glCreateShader(type)) == 0)
        throw Utils::Exception("Shader", "Can't create the shader !");
    glShaderSource(_shader, 1, &source, NULL);
    LOG_DEBUG << "Shader " << _shader << " CREATED" << std::endl;

    // compile the source
    Compile(source, type, name);
}

void    Shader::Compile(const char *source, Enum::ShaderType type, const Utils::FileName &name)
{
    // compilation
    LOG_DEBUG << "Compile `" << name.c_str() << "`...\t\t\t";
    glCompileShader(_shader);

    // check compilation errors
    int compileStatus = 1;
    glGetShaderiv(_shader, GL_COMPILE_STATUS, &compileStatus);
    if (!compileStatus)
    {
		PrintCompilationError(source, type, name);
		throw Utils::Exception("Shader::Compile", "Failed to compile '" + name + "'");
    }
    LOG_DEBUG << "DONE" << std::endl;
}

void Shader::PrintCompilationError(const char *source, Enum::ShaderType type, const Utils::FileName &name)
{
    int     errorSize;
    char    *error;
    glGetShaderiv(_shader, GL_INFO_LOG_LENGTH, &errorSize);
    error = new char[errorSize];
    glGetShaderInfoLog(_shader, errorSize, &errorSize, error);
    std::string se(error);
    delete[] error;

	LOG_ERROR << std::endl;
	if (type == Enum::FragmentShader)
		LOG_ERROR << "Fragment";
	else if (type == Enum::GeometryShader)
		LOG_ERROR << "Geometry";
	else if (type == Enum::VertexShader)
		LOG_ERROR << "Vertex";
	LOG_ERROR << " Shader compilation of `" << name.c_str() << "` Failed." << std::endl;

	unsigned int len = strlen(source);
	unsigned int line = 1;
	for (unsigned int i = 0; i < len; ++i)
	{
		LOG_ERROR << line << "\t: ";
		for (unsigned int lastLine = line; i < len && lastLine == line;)
		{
			if (source[i] == '\n')
				++line;
			else
				++i;
		}
	}
	LOG_ERROR << std::endl;
	LOG_ERROR << se << std::endl;
}
