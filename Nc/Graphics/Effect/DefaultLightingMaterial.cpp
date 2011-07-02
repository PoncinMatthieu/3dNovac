
/*-----------------------------------------------------------------------------

	3dNovac Graphics
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

    File Created At:        04/04/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "DefaultLightingMaterial.h"
#include "../Material/Drawable.h"
#include "LightingEffect.h"

using namespace Nc::Graphic;
using namespace Nc::Graphic::DefaultVertexType;

DefaultLightingMaterial::DefaultLightingMaterial() : ILightingMaterial("DefaultLightingMaterial")
{
    _shader.LoadFromFiles("Nc:Shader:lightShader.vs", "Nc:Shader:lightShader.fs");
    _shader.Enable();
    _uniformM = _shader.UniformLocation("MMatrix");
    _uniformPV = _shader.UniformLocation("VPMatrix");
    _uniformNormalMatrix = _shader.UniformLocation("NormalMatrix");
    _uniformLight = _shader.UniformLocation("Light");
    _uniformLightPass = _shader.UniformLocation("LightPass");
    _uniformLightColor = _shader.UniformLocation("LightColor");
    _uniformTextured = _shader.UniformLocation("Textured");
    _uniformDiffuse = _shader.UniformLocation("Diffuse");
    _uniformLightMap = _shader.UniformLocation("LightMap");
    _uniformNormalMap = _shader.UniformLocation("NormalMap");
    _uniformBumpMapping = _shader.UniformLocation("BumpMapping");
    //_shader.Disable();

    _lightMap.GenereSphereMap(64);      // creation d'une light map 3d de 64 float de diametre
    _patternMask.Enable(BumpMapping);   // active par defaut le bump maping
}

DefaultLightingMaterial::~DefaultLightingMaterial()
{
}

bool    DefaultLightingMaterial::Configure(Drawable &drawable)
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
        else if (desc[i].Name == ComponentsName::Normal)
            desc[i].IndexAttrib = _shader.AttribLocation("InNormal");
    }
    //_shader.Disable();
    return true;
}

#ifdef _DEBUG
// Draw the normals of the given geometry
// for reason of performance we choose to stock the normal drawable with the geometry pointer in a map, and create the drawable only the first time
void DrawNormal(SceneGraph *scene, const TMatrix &modelMatrix, GL::IGeometryBuffer *geometry)
{
    //check if the geometry has a Normal default componant
    GL::VertexDescriptor    &descript = geometry->Descriptor();
    int                     normalIndex = -1;
    for (unsigned int i = 0; i < descript.Size(); ++i)
        if (descript[i].Name == ComponentsName::Normal)
            normalIndex = i;
    if (normalIndex == -1)
        return; // there are no normal informations, so no need to display it :(

    // /!\ here we use a static to stock the drawables witch draw the normals, so if the drawable changes, we are sucked
    /// \todo recode this ligthing debug with a geometry shader insteed of creating drawables in the wind
    static std::map<GL::IGeometryBuffer*, Object>   mapObjectNormalDebug;
    Object      *normalObject = NULL;

    // searching for the drawable associated to the geometry
    std::map<GL::IGeometryBuffer*, Object>::iterator it = mapObjectNormalDebug.find(geometry);
    if (it == mapObjectNormalDebug.end()) // if it doesn't exit, we have to create it
    {
        // fetch the vertices and normal coords
        Array<float>    coords;
        Array<float>    normals;
        geometry->MapVertexBuffer(GL::Enum::ReadOnly);
        geometry->FetchComponent(ComponentsName::Coord, coords);
        geometry->FetchComponent(ComponentsName::Normal, normals);
        geometry->UnmapVertexBuffer();

        normalObject = &mapObjectNormalDebug[geometry];
        if (coords.Size() > 0)
        {
            Color           color(1,0,0);
            float           normalPercent = 0.1f;
            Array<Colored>  verticesNormal((coords.Size() / 3) * 2);
            for (unsigned int i = 0; i < verticesNormal.Size() / 2; ++i)
            {
                verticesNormal[i*2].Fill(coords[i*3], coords[(i*3) + 1], coords[(i*3) + 2], color);
                verticesNormal[(i*2) + 1].Fill(coords[i*3], coords[(i*3) + 1], coords[(i*3) + 2], color);
                verticesNormal[(i*2) + 1].coord[0] += (normals[i*3] * normalPercent);
                verticesNormal[(i*2) + 1].coord[1] += (normals[(i*3) + 1] * normalPercent);
                verticesNormal[(i*2) + 1].coord[2] += (normals[(i*3) + 2] * normalPercent);
            }
            normalObject->Drawables().push_back(new Drawable(verticesNormal, GL::Enum::StreamDraw, GL::Enum::Lines));
            normalObject->ChooseDefaultMaterial();
        }
    }
    else
        normalObject = &it->second;

    // draw
    if (normalObject->GetMaterial() != NULL)
        normalObject->GetMaterial()->Render(scene, modelMatrix, *normalObject->Drawables()[0]);
}
#endif

void    DefaultLightingMaterial::Render(SceneGraph *scene, const TMatrix &modelMatrix, Drawable &drawable)
{
    if (_currentLightingEffect == NULL)
        return;

    const ListPLight        &listLight = _currentLightingEffect->Lights();
    const Color             &ambiantColor = _currentLightingEffect->ColorAmbiant();
    //GL::VertexDescriptor    descriptor = drawable.Geometry->Descriptor();

    drawable.Enable();
    _shader.Enable(); // enable the shader

    // set the model project and view matrix
    TMatrix pv = scene->ProjectionMatrix() * scene->ViewMatrix();
    glUniformMatrix4fv(_uniformM, 1, GL_TRUE, modelMatrix.Elements());
    glUniformMatrix4fv(_uniformPV, 1, GL_TRUE, pv.Elements());
    // set the normal matrix
    TMatrix normalMatrix(modelMatrix);
    normalMatrix.Inverse();
    glUniformMatrix4fv(_uniformNormalMatrix, 1, GL_FALSE, normalMatrix.Elements());


    // set the ambiant color light
    glUniform4f(_uniformLightColor, ambiantColor.R, ambiantColor.G, ambiantColor.B, ambiantColor.A);

    // Texture unit 0 --> the diffuse
    if (drawable.Config->Textures.Size() > 0 &&
        drawable.Config->Textures[0].IsValid() &&
        drawable.Config->Textures[0].Is2d())
    {
        glUniform1i(_uniformTextured, true);
        glUniform1i(_uniformDiffuse, 0);
        GL::Texture::ActiveTexture(0);
        drawable.Config->Textures[0].Enable();
    }
    else
        glUniform1i(_uniformTextured, false);

    // First pass --> render the scene with the ambiant color
    glUniform1i(_uniformLightPass, false);
//    drawable.GetGeometry().GetVBO().MaskDescriptor = 0x07;
/// \todo in the first pass of the lightingMaterial, no need to bind the normal attrib
    drawable.Render();
//    drawable.GetGeometry().GetVBO().MaskDescriptor = lastDesc;

    // if the lighting is disabled, stop here
    if (_patternMask.Enabled(Disabled))
        return;

    // next pass --> render the lights
    glUniform1i(_uniformLightPass, true);

    glEnable(GL::Enum::Blend); // active blend
    glBlendFunc(GL_ONE, GL_ONE);
    glDepthMask(GL_FALSE); // disable the depth mask, no need

    // active the light map
    glUniform1i(_uniformLightMap, 1);
    GL::Texture::ActiveTexture(1);
    _lightMap.Enable();

    // active the normal mapping
    if (_patternMask.Enabled(BumpMapping) && drawable.Config->Textures.Size() > 1 &&
        drawable.Config->Textures[1].IsValid() &&
        drawable.Config->Textures[1].Is2d())
    {
        glUniform1i(_uniformBumpMapping, true);
        glUniform1i(_uniformNormalMap, 2);
        GL::Texture::ActiveTexture(2);
        drawable.Config->Textures[1].Enable();
    }
    else
        glUniform1i(_uniformBumpMapping, false);

    TMatrix matrixLight;
    for (ListPLight::const_iterator it = listLight.begin(); it != listLight.end(); it++)
    {
        Vector3f pos;
        (*it)->GetRecursiveMatrix(matrixLight); /// \todo To obtain the light position, we need to caculate recursively it's matrix, this could decrease the performance
        matrixLight.Transform(pos);
        glUniform4f(_uniformLight, pos.Data[0], pos.Data[1], pos.Data[2], 1.0f / ((*it)->radius));
        glUniform4f(_uniformLightColor, (*it)->color.R, (*it)->color.G, (*it)->color.B, (*it)->color.A);

        // rendering pass for the current light
        drawable.Render();
    }

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

    //_shader.Disable(); // disable the shader
    drawable.Disable();

    // only in debug, draw the normals
    #ifdef _DEBUG
    if (_patternMask.Enabled(DisplayNormal))
        DrawNormal(scene, modelMatrix, drawable.Geometry);
    #endif
}
