
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
    _shader.LoadFromMemory(ColoredVertexShader, ColoredFragmentShader, "BasicColoredShader");
    _shader.Enable();
    _uniformMVP = _shader.UniformLocation("MVPMatrix");
    //_shader.Disable();
    IDefaultMaterial::Initialize();
}

bool    DefaultMaterial<Colored>::Configure(Drawable &drawable)
{
    GL::VertexDescriptor  &desc = drawable.Geometry->Descriptor();
    _shader.Enable();
    for (unsigned int i = 0; i < desc.Size(); ++i)
    {
        if (desc[i].Name == ComponentsName::Coord)
            desc[i].IndexAttrib = _shader.AttribLocation("InCoord");
        else if (desc[i].Name == ComponentsName::Color)
            desc[i].IndexAttrib = _shader.AttribLocation("InColor");
    }
    //_shader.Disable();
    return true;
}

void    DefaultMaterial<Colored>::Render(SceneGraph *scene, const TMatrix &modelMatrix, Drawable &drawable)
{
    drawable.Enable();
    _shader.Enable(); // enable the shader

    TMatrix mvp =  scene->ProjectionMatrix() * scene->ViewMatrix() * modelMatrix;
    glUniformMatrix4fv(_uniformMVP, 1, GL_TRUE, mvp.Elements());

    drawable.Render(); // render the object

    //_shader.Disable(); // disable the shader
    drawable.Disable();
}

//----------------------
//       Colored2d
//----------------------
void    DefaultMaterial<Colored2d>::Initialize()
{
    _shader.LoadFromMemory(Colored2dVertexShader, Colored2dFragmentShader, "BasicColored2dShader");
    _shader.Enable();
    _uniformMVP = _shader.UniformLocation("MVPMatrix");
    //_shader.Disable();
    IDefaultMaterial::Initialize();
}

bool    DefaultMaterial<Colored2d>::Configure(Drawable &drawable)
{
    GL::VertexDescriptor  &desc = drawable.Geometry->Descriptor();
    _shader.Enable();
    for (unsigned int i = 0; i < desc.Size(); ++i)
    {
        if (desc[i].Name == ComponentsName::Coord)
            desc[i].IndexAttrib = _shader.AttribLocation("InCoord");
        else if (desc[i].Name == ComponentsName::Color)
            desc[i].IndexAttrib = _shader.AttribLocation("InColor");
    }
    //_shader.Disable();
    return true;
}

void    DefaultMaterial<Colored2d>::Render(SceneGraph *scene, const TMatrix &modelMatrix, Drawable &drawable)
{
    drawable.Enable();
    _shader.Enable(); // enable the shader

    TMatrix mvp =  scene->ProjectionMatrix() * scene->ViewMatrix() * modelMatrix;
    glUniformMatrix4fv(_uniformMVP, 1, GL_TRUE, mvp.Elements());

    drawable.Render(); // render the object

    //_shader.Disable(); // disable the shader
    drawable.Disable();
}


//----------------------
//       Textured
//----------------------
void    DefaultMaterial<Textured>::Initialize()
{
    _shader.LoadFromMemory(TexturedVertexShader, TexturedFragmentShader, "BasicTexturedShader");
    _shader.Enable();
    _uniformMVP = _shader.UniformLocation("MVPMatrix");
    _uniformTextured = _shader.UniformLocation("Textured");
    _uniformTextureId = _shader.UniformLocation("TextureId");
    //_shader.Disable();
    IDefaultMaterial::Initialize();
}

bool    DefaultMaterial<Textured>::Configure(Drawable &drawable)
{
    GL::VertexDescriptor  &desc = drawable.Geometry->Descriptor();
    _shader.Enable();
    for (unsigned int i = 0; i < desc.Size(); ++i)
    {
        if (desc[i].Name == ComponentsName::Coord)
            desc[i].IndexAttrib = _shader.AttribLocation("InCoord");
        else if (desc[i].Name == ComponentsName::TexCoord)
            desc[i].IndexAttrib = _shader.AttribLocation("InTexCoord");
        else if (desc[i].Name == ComponentsName::Color)
            desc[i].IndexAttrib = _shader.AttribLocation("InColor");
    }
    //_shader.Disable();
    return true;
}

void    DefaultMaterial<Textured>::Render(SceneGraph *scene, const TMatrix &modelMatrix, Drawable &drawable)
{
    drawable.Enable();
    _shader.Enable(); // enable the shader

    TMatrix mvp =  scene->ProjectionMatrix() * scene->ViewMatrix() * modelMatrix;
    glUniformMatrix4fv(_uniformMVP, 1, GL_TRUE, mvp.Elements());
    bool textured = (drawable.Config->Textures.Size() > 0 &&
                     drawable.Config->Textures[0].IsValid() &&
                     drawable.Config->Textures[0].Is2d());
    glUniform1i(_uniformTextured, textured);
    if (textured)
    {
        glUniform1i(_uniformTextureId, 0);
        GL::Texture::ActiveTexture(0);
        drawable.Config->Textures[0].Enable();
    }

    drawable.Render(); // render the object
    //_shader.Disable(); // disable the shader
    drawable.Disable();

    if (textured)
        drawable.Config->Textures[0].Disable();
}

//----------------------
//       Textured2d
//----------------------
void    DefaultMaterial<Textured2d>::Initialize()
{
    _shader.LoadFromMemory(Textured2dVertexShader, Textured2dFragmentShader, "BasicTextured2dShader");
    _shader.Enable();
    _uniformMVP = _shader.UniformLocation("MVPMatrix");
    _uniformTextured = _shader.UniformLocation("Textured");
    _uniformTextureId = _shader.UniformLocation("TextureId");
    //_shader.Disable();
    IDefaultMaterial::Initialize();
}

bool    DefaultMaterial<Textured2d>::Configure(Drawable &drawable)
{
    GL::VertexDescriptor  &desc = drawable.Geometry->Descriptor();
    _shader.Enable();
    for (unsigned int i = 0; i < desc.Size(); ++i)
    {
        if (desc[i].Name == ComponentsName::Coord)
            desc[i].IndexAttrib = _shader.AttribLocation("InCoord");
        else if (desc[i].Name == ComponentsName::TexCoord)
            desc[i].IndexAttrib = _shader.AttribLocation("InTexCoord");
        else if (desc[i].Name == ComponentsName::Color)
            desc[i].IndexAttrib = _shader.AttribLocation("InColor");
    }
    //_shader.Disable();
    return true;
}

void    DefaultMaterial<Textured2d>::Render(SceneGraph *scene, const TMatrix &modelMatrix, Drawable &drawable)
{
    drawable.Enable();
    _shader.Enable(); // enable the shader

    TMatrix mvp =  scene->ProjectionMatrix() * scene->ViewMatrix() * modelMatrix;
    glUniformMatrix4fv(_uniformMVP, 1, GL_TRUE, mvp.Elements());
    // Attention, on veut une texture 2d
    bool textured = (drawable.Config->Textures.Size() > 0 &&
                     drawable.Config->Textures[0].IsValid() &&
                     drawable.Config->Textures[0].Is2d());
    glUniform1i(_uniformTextured, textured);
    if (textured)
    {
        glUniform1i(_uniformTextureId, 0);
        GL::Texture::ActiveTexture(0);
        drawable.Config->Textures[0].Enable();
    }

    drawable.Render(); // render the object
    //_shader.Disable(); // disable the shader
    drawable.Disable();

    if (textured)
        drawable.Config->Textures[0].Disable();
}

//----------------------
//       Textured3d
//----------------------
void    DefaultMaterial<Textured3d>::Initialize()
{
    _shader.LoadFromMemory(Textured3dVertexShader, Textured3dFragmentShader, "BasicTextured3dShader");
    _shader.Enable();
    _uniformMVP = _shader.UniformLocation("MVPMatrix");
    _uniformTextureId = _shader.UniformLocation("TextureId");
    _uniformTextured = _shader.UniformLocation("Textured");
    //_shader.Disable();
    IDefaultMaterial::Initialize();
}

bool    DefaultMaterial<Textured3d>::Configure(Drawable &drawable)
{
    GL::VertexDescriptor  &desc = drawable.Geometry->Descriptor();
    _shader.Enable();
    for (unsigned int i = 0; i < desc.Size(); ++i)
    {
        if (desc[i].Name == ComponentsName::Coord)
            desc[i].IndexAttrib = _shader.AttribLocation("InCoord");
        else if (desc[i].Name == ComponentsName::TexCoord)
            desc[i].IndexAttrib = _shader.AttribLocation("InTexCoord");
        else if (desc[i].Name == ComponentsName::Color)
            desc[i].IndexAttrib = _shader.AttribLocation("InColor");
    }
    //_shader.Disable();
    return true;
}

void    DefaultMaterial<Textured3d>::Render(SceneGraph *scene, const TMatrix &modelMatrix, Drawable &drawable)
{
    drawable.Enable();
    _shader.Enable(); // enable the shader

    TMatrix mvp =  scene->ProjectionMatrix() * scene->ViewMatrix() * modelMatrix;
    glUniformMatrix4fv(_uniformMVP, 1, GL_TRUE, mvp.Elements());
    bool textured = (drawable.Config->Textures.Size() > 0 &&
                     drawable.Config->Textures[0].IsValid() &&
                     drawable.Config->Textures[0].IsCubeMap());
    glUniform1i(_uniformTextured, textured);
    if (textured)
    {
        glUniform1i(_uniformTextureId, 0);
        GL::Texture::ActiveTexture(0);
        drawable.Config->Textures[0].Enable();
    }

    drawable.Render(); // render the object

    //_shader.Disable(); // disable the shader
    drawable.Disable();

    if (textured)
        drawable.Config->Textures[0].Disable();
}
