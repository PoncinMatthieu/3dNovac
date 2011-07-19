
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
using namespace Nc::Graphic::DefaultVertexType;
using namespace Nc::Graphic::DefaultSources;

//----------------------
//       Colored
//----------------------
void    DefaultMaterial<Colored>::Initialize()
{
    _program.Attach(ColoredVertexShader, GL::Enum::VertexShader, "BasicColoredVertexShader");
    _program.Attach(ColoredFragmentShader, GL::Enum::FragmentShader, "BasicColoredFragmentShader");
    _program.Link();
    _program.Enable();
    _program.UseUniformLocation("MVPMatrix");
    _program.UseAttribLocation("InCoord");
    _program.UseAttribLocation("InColor");
    IDefaultMaterial::Initialize();
}

bool    DefaultMaterial<Colored>::Configure(Drawable &drawable)
{
    GL::VertexDescriptor  &desc = drawable.Geometry->Descriptor();
    _program.Enable();
    for (unsigned int i = 0; i < desc.Size(); ++i)
    {
        if (desc[i].Name == ComponentsName::Coord)
            desc[i].IndexAttrib = _program.Attrib(0);
        else if (desc[i].Name == ComponentsName::Color)
            desc[i].IndexAttrib = _program.Attrib(1);
    }
    return true;
}

void    DefaultMaterial<Colored>::Render(SceneGraph *scene, const TMatrix &modelMatrix, Drawable &drawable)
{
    drawable.Enable();
    _program.Enable(); // enable the shader

    _program.SetUniform(0, scene->ProjectionMatrix() * scene->ViewMatrix() * modelMatrix);

    drawable.Render(); // render the object

    drawable.Disable();
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
    _program.UseUniformLocation("MVPMatrix");
    _program.UseAttribLocation("InCoord");
    _program.UseAttribLocation("InColor");
    IDefaultMaterial::Initialize();
}

bool    DefaultMaterial<Colored2d>::Configure(Drawable &drawable)
{
    GL::VertexDescriptor  &desc = drawable.Geometry->Descriptor();
    _program.Enable();
    for (unsigned int i = 0; i < desc.Size(); ++i)
    {
        if (desc[i].Name == ComponentsName::Coord)
            desc[i].IndexAttrib = _program.Attrib(0);
        else if (desc[i].Name == ComponentsName::Color)
            desc[i].IndexAttrib = _program.Attrib(1);
    }
    return true;
}

void    DefaultMaterial<Colored2d>::Render(SceneGraph *scene, const TMatrix &modelMatrix, Drawable &drawable)
{
    drawable.Enable();
    _program.Enable(); // enable the shader

    _program.SetUniform(0, scene->ProjectionMatrix() * scene->ViewMatrix() * modelMatrix);

    drawable.Render(); // render the object

    drawable.Disable();
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
    _program.UseUniformLocation("MVPMatrix");
    _program.UseUniformLocation("Textured");
    _program.UseUniformLocation("TextureId");
    _program.UseAttribLocation("InCoord");
    _program.UseAttribLocation("InTexCoord");
    _program.UseAttribLocation("InColor");
    IDefaultMaterial::Initialize();
}

bool    DefaultMaterial<Textured>::Configure(Drawable &drawable)
{
    GL::VertexDescriptor  &desc = drawable.Geometry->Descriptor();
    _program.Enable();
    for (unsigned int i = 0; i < desc.Size(); ++i)
    {
        if (desc[i].Name == ComponentsName::Coord)
            desc[i].IndexAttrib = _program.Attrib(0);
        else if (desc[i].Name == ComponentsName::TexCoord)
            desc[i].IndexAttrib = _program.Attrib(1);
        else if (desc[i].Name == ComponentsName::Color)
            desc[i].IndexAttrib = _program.Attrib(2);
    }
    return true;
}

void    DefaultMaterial<Textured>::Render(SceneGraph *scene, const TMatrix &modelMatrix, Drawable &drawable)
{
    drawable.Enable();
    _program.Enable(); // enable the shader

    _program.SetUniform(0, scene->ProjectionMatrix() * scene->ViewMatrix() * modelMatrix);

    bool textured = (drawable.Config->Textures.Size() > 0 &&
                     drawable.Config->Textures[0].IsValid() &&
                     drawable.Config->Textures[0].Is2d());
    _program.SetUniform(1, textured);
    if (textured)
    {
        _program.SetUniform(2, 0);
        GL::State::Current().ActiveTexture(0);
        drawable.Config->Textures[0].Enable();
    }

    drawable.Render(); // render the object
    drawable.Disable();

    if (textured)
        drawable.Config->Textures[0].Disable();
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
    _program.UseUniformLocation("MVPMatrix");
    _program.UseUniformLocation("Textured");
    _program.UseUniformLocation("TextureId");
    _program.UseAttribLocation("InCoord");
    _program.UseAttribLocation("InTexCoord");
    _program.UseAttribLocation("InColor");
    IDefaultMaterial::Initialize();
}

bool    DefaultMaterial<Textured2d>::Configure(Drawable &drawable)
{
    GL::VertexDescriptor  &desc = drawable.Geometry->Descriptor();
    _program.Enable();
    for (unsigned int i = 0; i < desc.Size(); ++i)
    {
        if (desc[i].Name == ComponentsName::Coord)
            desc[i].IndexAttrib = _program.Attrib(0);
        else if (desc[i].Name == ComponentsName::TexCoord)
            desc[i].IndexAttrib = _program.Attrib(1);
        else if (desc[i].Name == ComponentsName::Color)
            desc[i].IndexAttrib = _program.Attrib(2);
    }
    return true;
}

void    DefaultMaterial<Textured2d>::Render(SceneGraph *scene, const TMatrix &modelMatrix, Drawable &drawable)
{
    drawable.Enable();
    _program.Enable(); // enable the shader

    _program.SetUniform(0, scene->ProjectionMatrix() * scene->ViewMatrix() * modelMatrix);

    // Attention, on veut une texture 2d
    bool textured = (drawable.Config->Textures.Size() > 0 &&
                     drawable.Config->Textures[0].IsValid() &&
                     drawable.Config->Textures[0].Is2d());
    _program.SetUniform(1, textured);
    if (textured)
    {
        _program.SetUniform(2, 0);
        GL::State::Current().ActiveTexture(0);
        drawable.Config->Textures[0].Enable();
    }

    drawable.Render(); // render the object
    drawable.Disable();

    if (textured)
        drawable.Config->Textures[0].Disable();
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
    _program.UseUniformLocation("MVPMatrix");
    _program.UseUniformLocation("Textured");
    _program.UseUniformLocation("TextureId");
    _program.UseAttribLocation("InCoord");
    _program.UseAttribLocation("InTexCoord");
    _program.UseAttribLocation("InColor");
    IDefaultMaterial::Initialize();
}

bool    DefaultMaterial<Textured3d>::Configure(Drawable &drawable)
{
    GL::VertexDescriptor  &desc = drawable.Geometry->Descriptor();
    _program.Enable();
    for (unsigned int i = 0; i < desc.Size(); ++i)
    {
        if (desc[i].Name == ComponentsName::Coord)
            desc[i].IndexAttrib = _program.Attrib(0);
        else if (desc[i].Name == ComponentsName::TexCoord)
            desc[i].IndexAttrib = _program.Attrib(1);
        else if (desc[i].Name == ComponentsName::Color)
            desc[i].IndexAttrib = _program.Attrib(2);
    }
    return true;
}

void    DefaultMaterial<Textured3d>::Render(SceneGraph *scene, const TMatrix &modelMatrix, Drawable &drawable)
{
    drawable.Enable();
    _program.Enable(); // enable the shader

    _program.SetUniform(0, scene->ProjectionMatrix() * scene->ViewMatrix() * modelMatrix);

    bool textured = (drawable.Config->Textures.Size() > 0 &&
                     drawable.Config->Textures[0].IsValid() &&
                     drawable.Config->Textures[0].IsCubeMap());
    _program.SetUniform(1, textured);
    if (textured)
    {
        _program.SetUniform(2, 0);
        GL::State::Current().ActiveTexture(0);
        drawable.Config->Textures[0].Enable();
    }

    drawable.Render(); // render the object

    drawable.Disable();

    if (textured)
        drawable.Config->Textures[0].Disable();
}
