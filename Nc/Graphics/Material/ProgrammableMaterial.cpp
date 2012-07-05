
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

    File Created At:        23/07/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "../Core/Drawable.h"
#include "ProgrammableMaterial.h"

using namespace Nc;
using namespace Nc::Graphic;

ProgrammableMaterial::ProgrammableMaterial(const std::string materialName, const Utils::FileName &vertexShader, const Utils::FileName &pixelShader)
    : IMaterial(materialName)
{
    _uniformMVPMatrix = -1;
    _uniformMMatrix = -1;
    _uniformVPMatrix = -1;
    _uniformNormalMatrix = -1;

    _program.Attach(vertexShader, GL::Enum::VertexShader);
    _program.Attach(pixelShader, GL::Enum::FragmentShader);
    _program.Link();
}

ProgrammableMaterial::~ProgrammableMaterial()
{
}

bool    ProgrammableMaterial::Configure(Core::Drawable &drawable)
{
	GL::VertexDescriptor  &desc = drawable.Geometry->Descriptor();
    _program.Enable();
    for (unsigned int i = 0; i < desc.Size(); ++i)
    {
		bool find = false;
		for (AttribMap::iterator it = _attribs.begin(); it != _attribs.end(); ++it)
        {
            if (desc[i].Name == it->first)
			{
                desc[i].IndexAttrib = it->second;
				find = true;
			}
		}
		if (!find)
			desc[i].IndexAttrib = -1;
    }
	return true;
}

void    ProgrammableMaterial::Render(SceneGraph *scene, const TMatrix &modelMatrix, Core::Drawable &drawable)
{
    drawable.Enable();
    _program.Enable(); // enable the shader

    // set the matrix uniforms
    if (_uniformMVPMatrix != -1)
        _program.SetUniform(_uniformMVPMatrix, scene->ProjectionMatrix() * scene->ViewMatrix() * modelMatrix);
    else
    {
        if (_uniformMMatrix != -1)
            _program.SetUniform(_uniformMMatrix, modelMatrix);
        if (_uniformVPMatrix != -1)
            _program.SetUniform(_uniformVPMatrix, scene->ProjectionMatrix() * scene->ViewMatrix());
    }
	// set the normal matrix uniform
	if (_uniformNormalMatrix != -1)
    {
        TMatrix normalMatrix(modelMatrix);
        normalMatrix.Inverse();
        _program.SetUniform(_uniformNormalMatrix, normalMatrix, false);
    }

	// set the basic uniforms
	for (UniformMap::iterator it = _uniforms.begin(); it != _uniforms.end(); ++it)
		if (it->second->activated)
			it->second->Set();

	// Set the texture unit uniforms and active textures
	for (UniformDrawablesTextureUnitMap::iterator it = _drawablesTextureUnitUniforms.begin(); it != _drawablesTextureUnitUniforms.end(); ++it)
    {
        if (it->second != -1 && drawable.Config->Textures.Size() > it->first)
        {
            _program.SetUniform(it->second, it->first);
            scene->GLState()->ActiveTexture(it->first);
            drawable.Config->Textures[it->first].Enable();
        }
    }

    drawable.Render(); // render the object
    drawable.Disable();
}

void    ProgrammableMaterial::SetAttrib(const std::string &componentName, const char *attribName)
{
    _attribs[componentName] = _program.GetAttribLocation(attribName);
}

void    ProgrammableMaterial::SetDrawableTextureUnitUniform(unsigned int unit, const char *name)
{
    _drawablesTextureUnitUniforms[unit] = _program.GetUniformLocation(name);
}

void    ProgrammableMaterial::UnsetDrawableTextureUnitUniform(unsigned int unit)
{
    _drawablesTextureUnitUniforms[unit] = -1;
}

void    ProgrammableMaterial::SetMVPMatrixUniform(const char *name)
{
    _uniformMVPMatrix = _program.GetUniformLocation(name);
}

void    ProgrammableMaterial::SetNormalMatrixUniform(const char *name)
{
    _uniformNormalMatrix = _program.GetUniformLocation(name);
}

void    ProgrammableMaterial::SetMMatrixUniform(const char *name)
{
    _uniformMMatrix = _program.GetUniformLocation(name);
}

void    ProgrammableMaterial::SetVPMatrixUniform(const char *name)
{
    _uniformVPMatrix = _program.GetUniformLocation(name);
}

namespace Nc
{
    namespace Graphic
    {
        std::ostream &operator << (std::ostream &oss, const ProgrammableMaterial &m)
        {
            if (!m._attribs.empty())
            {
                oss << "Attributes:" << std::endl;
                for (ProgrammableMaterial::AttribMap::const_iterator it = m._attribs.begin(); it != m._attribs.end(); ++it)
                    oss << "\t" << it->first << "\t" << it->second << std::endl;
            }

            oss << "Matrix:" << std::endl;
            if (m._uniformMVPMatrix != -1)
                oss << "\tMVP: " << m._uniformMVPMatrix << std::endl;
            else
            {
                if (m._uniformMMatrix != -1)
                    oss << "\tM: " << m._uniformMMatrix << std::endl;
                if (m._uniformVPMatrix != -1)
                    oss << "\tVP: " << m._uniformVPMatrix << std::endl;
            }
            if (m._uniformNormalMatrix != -1)
                oss << "\tNormal: " << m._uniformNormalMatrix << std::endl;

            if (!m._drawablesTextureUnitUniforms.empty())
            {
                oss << "Textures drawable uniform:" << std::endl;
                for (ProgrammableMaterial::UniformDrawablesTextureUnitMap::const_iterator it = m._drawablesTextureUnitUniforms.begin(); it != m._drawablesTextureUnitUniforms.end(); ++it)
                {
                    if (it->second != -1)
                        oss << "\t" << it->first << "\t" << it->second << std::endl;
                }
            }

            if (!m._uniforms.empty())
            {
                oss << "Uniforms:" << std::endl;
                for (ProgrammableMaterial::UniformMap::const_iterator it = m._uniforms.begin(); it != m._uniforms.end(); ++it)
                    if (it->second->activated)
                        oss << "\t" << it->first << "\t" << *it->second << std::endl;
            }
            return oss;
        }
    }
}
