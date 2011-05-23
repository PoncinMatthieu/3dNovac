
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
#include "../Scene/Basic3dSceneGraph.h"
#include "../Object/Drawable.h"
#include "../Object/3D/Light.h"

using namespace Nc::Graphic;

DefaultLightingMaterialPolitic::DefaultLightingMaterialPolitic()
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
    _shader.Disable();

    _lightMap.GenereSphereMap(64);
}

DefaultLightingMaterialPolitic::~DefaultLightingMaterialPolitic()
{
}

void    DefaultLightingMaterialPolitic::Configure(GL::IGeometryBuffer<BasicVertexType::Textured> &geometry)
{
    GL::VertexDescriptor  &desc = geometry.GetVBO().Descriptor;
    _shader.Enable();
    desc[0].IndexAttrib = _shader.AttribLocation("InCoord");
    desc[1].IndexAttrib = _shader.AttribLocation("InTexCoord");
    desc[2].IndexAttrib = _shader.AttribLocation("InColor");
    desc[3].IndexAttrib = _shader.AttribLocation("InNormal");
    _shader.Disable();
}

#ifdef _DEBUG
// Draw the normals of the given geometry
// for reason of performance we choose to stock the normal drawable with the geometry pointer in a map, and create the drawable only the first time
void DrawNormal(ISceneGraph *scene, const TMatrix &modelMatrix, GL::IGeometryBuffer<BasicVertexType::Textured> *geometry)
{
    // /!\ si le drawable change, pas celui des normals
    static std::map<GL::IGeometryBuffer<BasicVertexType::Textured>*, Drawable<BasicVertexType::Colored, false> >    mapDrawableNormalDebug;
    Drawable<BasicVertexType::Colored, false> *normalDrawable = NULL;

    // recherche le drawable des normals dans la map
    std::map<GL::IGeometryBuffer<BasicVertexType::Textured>*, Drawable<BasicVertexType::Colored, false> >::iterator it = mapDrawableNormalDebug.find(geometry);
    if (it == mapDrawableNormalDebug.end()) // le drawable pour dessiner la normal n'existe encore pas ? on le creer
    {
        normalDrawable = &mapDrawableNormalDebug[geometry];
        normalDrawable->SetPrimitiveType(GL_LINES);

        // to draw the normals, we need to fetch the vertices of the drawable
        geometry->GetVBO().Enable();
        BasicVertexType::Textured *vertices = geometry->GetVBO().MapBuffer(GL_READ_ONLY);
        Color color(1,0,0);

        if (vertices != NULL)
        {
            // and create 2 new vertices per vertex
            unsigned int    size = geometry->GetVBO().Size();
            float           normalPercent = 0.1f;
            Array<BasicVertexType::Colored>   verticesNormal(size * 2);
            for (unsigned int i = 0; i < size; ++i)
            {
                verticesNormal[i*2].Fill(vertices[i].coord[0], vertices[i].coord[1], vertices[i].coord[2], color);
                verticesNormal[(i*2) + 1].Fill(vertices[i].coord[0], vertices[i].coord[1], vertices[i].coord[2], color);
                verticesNormal[(i*2) + 1].coord[0] += (vertices[i].normal[0] * normalPercent);
                verticesNormal[(i*2) + 1].coord[1] += (vertices[i].normal[1] * normalPercent);
                verticesNormal[(i*2) + 1].coord[2] += (vertices[i].normal[2] * normalPercent);
            }
            normalDrawable->GetVBO().UpdateData(verticesNormal, GL_STREAM_DRAW);

        }
        // finaly, unmap the buffer and configure with the material
        geometry->GetVBO().Enable();
        geometry->GetVBO().UnmapBuffer();
        Material<BasicVertexType::Colored>::Instance().Configure(*normalDrawable);
        geometry->GetVBO().Disable();
    }
    else
        normalDrawable = &it->second;

    // draw les normals
    Material<BasicVertexType::Colored>::Instance().Render(scene, modelMatrix, *normalDrawable);
}
#endif

void    DefaultLightingMaterialPolitic::Render(ISceneGraph *scene, const TMatrix &modelMatrix, DrawableType &drawable)
{
// /!\ static_cast, could be dangerous
    const ListPLight    &listLight = static_cast<Basic3dSceneGraph*>(scene)->Lights();
    const Color         &ambiantColor = static_cast<Basic3dSceneGraph*>(scene)->ColorAmbiant();
    unsigned int        lastDesc = drawable.GetGeometry().GetVBO().MaskDescriptor;

    drawable.Enable();
    _shader.Enable(); // enable the shader

    // set the model project and view matrix
    TMatrix pv = scene->ProjectionMatrix() * scene->ViewMatrix();
    glUniformMatrix4fv(_uniformM, 1, GL_TRUE, modelMatrix.Elements());
    glUniformMatrix4fv(_uniformPV, 1, GL_TRUE, pv.Elements());
    // set the normal matrix
    TMatrix normalMatrix;
    modelMatrix.Transpose(normalMatrix);
    normalMatrix.Inverse();
    glUniformMatrix4fv(_uniformNormalMatrix, 1, GL_TRUE, normalMatrix.Elements());

    // set the ambiant color light
    glUniform4f(_uniformLightColor, ambiantColor.R, ambiantColor.G, ambiantColor.B, ambiantColor.A);

    // Texture unit 0 --> the diffuse
    glUniform1i(_uniformTextured, drawable.texture.IsValid());
    if (drawable.texture.IsValid())
    {
        glUniform1i(_uniformDiffuse, 0);
        glActiveTexture(GL_TEXTURE0);
        drawable.texture.Enable();
    }

    // First pass --> render the scene with the ambiant color
    glUniform1i(_uniformLightPass, false);
    drawable.GetGeometry().GetVBO().MaskDescriptor = 0x07;
    drawable.Render();
    drawable.GetGeometry().GetVBO().MaskDescriptor = lastDesc;

    // if the lighting is disabled, stop here
    if (_patternMask.Enabled(Disabled))
        return;

    // next pass --> render the lights
    glUniform1i(_uniformLightPass, true);

    glEnable(GL_BLEND); // active blend
    glBlendFunc(GL_ONE, GL_ONE);
    glDepthMask(GL_FALSE); // disable the depth mask, no need

    // active the light map
    glUniform1i(_uniformLightMap, 1);
    glActiveTexture(GL_TEXTURE1);
    _lightMap.Enable();

    // active the normal mapping
    if (_patternMask.Enabled(BumpMapping) && drawable.normalMap.IsValid())
    {
        glUniform1i(_uniformBumpMapping, true);
        glUniform1i(_uniformNormalMap, 2);
        glActiveTexture(GL_TEXTURE2);
        drawable.normalMap.Enable();
    }
    else
        glUniform1i(_uniformBumpMapping, false);

    for (ListPLight::const_iterator it = listLight.begin(); it != listLight.end(); it++)
    {
        Vector3f  posLight = (*it)->Position();
        (*it)->Matrix.Transform(posLight);
        glUniform4f(_uniformLight, posLight.Data[0], posLight.Data[1], posLight.Data[2], 1.0f / ((*it)->Radius()));
        glUniform4f(_uniformLightColor, (*it)->ColorRGB().R, (*it)->ColorRGB().G, (*it)->ColorRGB().B, (*it)->ColorRGB().A);

        // rendering pass for the current light
        drawable.Render();
    }

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

    _shader.Disable(); // disable the shader
    drawable.Disable();

    // only in debug, draw the normals
    #ifdef _DEBUG
    if (_patternMask.Enabled(DisplayNormal))
        DrawNormal(scene, modelMatrix, &drawable.GetGeometry());
    #endif
}
