
/*-----------------------------------------------------------------------------

	3dNovac Core
	Copyright (C) 2010-2011, 3dNovac Team

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

    File Created At:        31/05/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "DefaultMaterials.h"

using namespace Nc;
using namespace Nc::Graphic;
using namespace Nc::Graphic::Core::DefaultVertexType;
using namespace Nc::Graphic::DefaultSources;

DefaultMaterialColored::DefaultMaterialColored(const std::string &name)
    : IDefaultMaterial(name)
{}

bool    DefaultMaterialColored::Configure(Core::Drawable &drawable)
{
    GL::VertexDescriptor  &desc = drawable.Geometry->Descriptor();
    _program.Enable();
    for (unsigned int i = 0; i < desc.Size(); ++i)
    {
        if (desc[i].Name == ComponentsName::Coord)
            desc[i].IndexAttrib = _attribCoord;
        else if (desc[i].Name == ComponentsName::Color)
            desc[i].IndexAttrib = _attribColor;
        else
            desc[i].IndexAttrib = -1;
    }
    return true;
}

void    DefaultMaterialColored::Render(SceneGraph *scene, const TMatrix &modelMatrix, Core::Drawable &drawable)
{
    drawable.Enable();
    _program.Enable(); // enable the shader

    _program.SetUniform(_uniformMVP, scene->ProjectionMatrix() * scene->ViewMatrix() * modelMatrix);
	drawable.Render(); // render the object

    drawable.Disable();
}

DefaultMaterialTextured::DefaultMaterialTextured(const std::string &name)
    : IDefaultMaterial(name)
{}

bool    DefaultMaterialTextured::Configure(Core::Drawable &drawable)
{
    GL::VertexDescriptor  &desc = drawable.Geometry->Descriptor();
    _program.Enable();
    for (unsigned int i = 0; i < desc.Size(); ++i)
    {
        if (desc[i].Name == ComponentsName::Coord)
            desc[i].IndexAttrib = _attribCoord;
        else if (desc[i].Name == ComponentsName::TexCoord)
            desc[i].IndexAttrib = _attribTexCoord;
        else if (desc[i].Name == ComponentsName::Color)
            desc[i].IndexAttrib = _attribColor;
        else
            desc[i].IndexAttrib = -1;
    }
    return true;
}

void    DefaultMaterialTextured::Render(SceneGraph *scene, const TMatrix &modelMatrix, Core::Drawable &drawable)
{
    drawable.Enable();
    _program.Enable(); // enable the shader

    _program.SetUniform(_uniformMVP, scene->ProjectionMatrix() * scene->ViewMatrix() * modelMatrix);

    bool textured = (drawable.Config->Textures.Size() > 0 &&
                     drawable.Config->Textures[0].IsValid() &&
                     drawable.Config->Textures[0].Is2d());
    _program.SetUniform(_uniformTextured, textured);
    if (textured)
    {
        _program.SetUniform(_uniformTexturedId, 0);
        scene->GLState()->ActiveTexture(0);
        drawable.Config->Textures[0].Enable();
    }

    drawable.Render(); // render the object
    drawable.Disable();

    if (textured)
        drawable.Config->Textures[0].Disable();
}

//----------------------
//       Colored
//----------------------
void    DefaultMaterial<Colored>::Initialize()
{
    _program.Attach(ColoredVertexShader, GL::Enum::VertexShader, "BasicColoredVertexShader");
    _program.Attach(ColoredFragmentShader, GL::Enum::FragmentShader, "BasicColoredFragmentShader");
    _program.Link();
    _program.Enable();
    _uniformMVP = _program.GetUniformLocation("MVPMatrix");
    _attribCoord = _program.GetAttribLocation("InCoord");
    _attribColor = _program.GetAttribLocation("InColor");
    IDefaultMaterial::Initialize();
}

//----------------------
//       Colored2d
//----------------------
void    DefaultMaterial<Colored2d>::Initialize()
{
    _program.Attach(Colored2dVertexShader, GL::Enum::VertexShader, "BasicColored2dVertexShader");
    _program.Attach(Colored2dFragmentShader, GL::Enum::FragmentShader, "BasicColored2dFragmentShader");
    _program.Link();
    _program.Enable();
    _uniformMVP = _program.GetUniformLocation("MVPMatrix");
    _attribCoord = _program.GetAttribLocation("InCoord");
    _attribColor = _program.GetAttribLocation("InColor");
    IDefaultMaterial::Initialize();
}

//----------------------
//       Textured
//----------------------
void    DefaultMaterial<Textured>::Initialize()
{
    _program.Attach(TexturedVertexShader, GL::Enum::VertexShader, "BasicTexturedVertexShader");
    _program.Attach(TexturedFragmentShader, GL::Enum::FragmentShader, "BasicTexturedFragmentShader");
    _program.Link();
    _program.Enable();
    _uniformMVP = _program.GetUniformLocation("MVPMatrix");
    _uniformTextured = _program.GetUniformLocation("Textured");
    _uniformTexturedId = _program.GetUniformLocation("TextureId");
    _attribCoord = _program.GetAttribLocation("InCoord");
    _attribTexCoord = _program.GetAttribLocation("InTexCoord");
    _attribColor = _program.GetAttribLocation("InColor");
    IDefaultMaterial::Initialize();
}

//----------------------
//       Textured2d
//----------------------
void    DefaultMaterial<Textured2d>::Initialize()
{
    _program.Attach(Textured2dVertexShader, GL::Enum::VertexShader, "BasicTextured2dVertexShader");
    _program.Attach(Textured2dFragmentShader, GL::Enum::FragmentShader, "BasicTextured2dFragmentShader");
    _program.Link();
    _program.Enable();
    _uniformMVP = _program.GetUniformLocation("MVPMatrix");
    _uniformTextured = _program.GetUniformLocation("Textured");
    _uniformTexturedId = _program.GetUniformLocation("TextureId");
    _attribCoord = _program.GetAttribLocation("InCoord");
    _attribTexCoord = _program.GetAttribLocation("InTexCoord");
    _attribColor = _program.GetAttribLocation("InColor");
    IDefaultMaterial::Initialize();
}

//----------------------
//       Textured3d
//----------------------
void    DefaultMaterial<Textured3d>::Initialize()
{
    _program.Attach(Textured3dVertexShader, GL::Enum::VertexShader, "BasicTextured3dVertexShader");
    _program.Attach(Textured3dFragmentShader, GL::Enum::FragmentShader, "BasicTextured3dFragmentShader");
    _program.Link();
    _program.Enable();
    _uniformMVP = _program.GetUniformLocation("MVPMatrix");
    _uniformTextured = _program.GetUniformLocation("Textured");
    _uniformTexturedId = _program.GetUniformLocation("TextureId");
    _attribCoord = _program.GetAttribLocation("InCoord");
    _attribTexCoord = _program.GetAttribLocation("InTexCoord");
    _attribColor = _program.GetAttribLocation("InColor");
    IDefaultMaterial::Initialize();
}

void    DefaultMaterial<Textured3d>::Render(SceneGraph *scene, const TMatrix &modelMatrix, Core::Drawable &drawable)
{
    drawable.Enable();
    _program.Enable(); // enable the shader

    _program.SetUniform(_uniformMVP, scene->ProjectionMatrix() * scene->ViewMatrix() * modelMatrix);

    bool textured = (drawable.Config->Textures.Size() > 0 &&
                     drawable.Config->Textures[0].IsValid() &&
                     drawable.Config->Textures[0].IsCubeMap());
    _program.SetUniform(_uniformTextured, textured);
    if (textured)
    {
        _program.SetUniform(_uniformTexturedId, 0);
        scene->GLState()->ActiveTexture(0);
        drawable.Config->Textures[0].Enable();
    }

    drawable.Render(); // render the object

    drawable.Disable();

    if (textured)
        drawable.Config->Textures[0].Disable();
}
