/*
	3dNovac Engine
	Copyright (C) 2010, Poncin Matthieu
	All rights reserved.

	This work is licensed under the Creative Commons
	Attribution-Share Alike 2.0 France License.
	To view a copy of this license, visit :
	http://creativecommons.org/licenses/by-sa/2.0/fr/
*/

#include "LightingMaterial.h"
#include "../Object/Mesh.h"
#include "../Object/Light.h"
#include "../../Basic3DSceneGraph.h"

#include <Utils/Debug/OverloadAlloc.h>

using namespace std;
using namespace Nc;
using namespace Nc::Graphic;
using namespace Nc::GL;

LightingMaterial::LightingMaterial() : Material()
{
    // creation de la light map 3d, ainsi que du shader de lighting pour l'eclairage, do it once
    _lightMap.GenereSphereMap(64);
    const std::string &path = CONFIG->Block("RessourcesPath")->Line("Shader")->Param("path");
    _lightShader.LoadFromFiles(path + "lightVertexShader.glsl", path + "lightPixelShader.glsl");
    DrawLocalSpace = false;
    BumpMapping = true;
}

LightingMaterial::~LightingMaterial()
{
}

void LightingMaterial::Render(Mesh *m, const MaterialConfig *config)
{
// /!\ here we use a static cast for perf, please make sure that you use the good MaterialConfig
    const LightingMaterialConfig    &lightConfig = static_cast<const LightingMaterialConfig&>(*config);
    const Basic3DSceneGraph         &scene = *Basic3DSceneGraph::GetCurrentScene();
    const ListPLight                &listLight = scene.Lights();
    bool                            bumpMaping = (lightConfig.NormalMap.IsValid() && BumpMapping) ? true : false;
    TMatrix                         reelMatrix;
    GL::IVertexBuffer               *VBO = m->GetVBO();

    if (VBO == NULL) // si on a pas de vbo on s'arrete la
        throw Utils::Exception("The VertexBufferObject of the mesh is not set");

    glPushMatrix();
        // transformation de la matrice
        m->TransformMatrixToRender();

        // active la premiere unite de texture avec le texture diffuse, pour rendre la couleur ambiante
        EXT.ActiveTexture(GL_TEXTURE0);
        lightConfig.Texture.Enable();

        if (!lightConfig.GhostMode)
        {
            scene.ColorAmbiant().Enable(); // dessine la scene avec la texture et la couleur ambiante
            VBO->MaskDescriptor = 0x3;
            VBO->Draw(m->GetIBO());
        }

        // active le shader pour rendre l'eclairage
        _lightShader.Enable();

        static const unsigned int indexUniformLight =       _lightShader.Uniform("light");
        static const unsigned int indexUniformDiffuse =     _lightShader.Uniform("Diffuse");
        static const unsigned int indexUniformNormalMap =   _lightShader.Uniform("NormalMap");
        static const unsigned int indexUniformLightMap =    _lightShader.Uniform("LightMap");
        static const unsigned int indexUniformActiveBump =  _lightShader.Uniform("ActiveBump");
        static const unsigned int indexUniformModelMatrix = _lightShader.Uniform("ModelMatrix");

        static const unsigned int indexAttribTexCoord =     _lightShader.Attrib("textureCoord");
        static const unsigned int indexAttribNormal =       _lightShader.Attrib("normal");
        static const unsigned int indexAttribBin =          _lightShader.Attrib("binormal");
        static const unsigned int indexAttribTan =          _lightShader.Attrib("tangente");

        VBO->MaskDescriptor = 0x3B; // all except colors
        VBO->Descriptor.Data[1].DataType = AttribData;
        VBO->Descriptor.Data[1].IndexAttrib = indexAttribTexCoord;
        VBO->Descriptor.Data[3].IndexAttrib = indexAttribNormal;
        VBO->Descriptor.Data[4].IndexAttrib = indexAttribBin;
        VBO->Descriptor.Data[5].IndexAttrib = indexAttribTan;

        EXT.Uniform1i(indexUniformActiveBump, bumpMaping);
        // la 1ere unite de texture contient deja la texture diffuse
        EXT.ActiveTexture(GL_TEXTURE0);
        EXT.Uniform1i(indexUniformDiffuse, 0);
        // la 2eme contient la texture d'attenuation 3d (lightMap)
        EXT.ActiveTexture(GL_TEXTURE1);
        _lightMap.Enable();
        EXT.Uniform1i(indexUniformLightMap, 1);
        // la 3eme contient la normal map si le bump mapping et actif
        if (bumpMaping)
        {
            EXT.ActiveTexture(GL_TEXTURE2);
            lightConfig.NormalMap.Enable();
            EXT.Uniform1i(indexUniformNormalMap, 2);
        }

        glEnable(GL_BLEND); // active le blend et desactive le depth buffer, on en a plus besoin
        glBlendFunc(GL_ONE, GL_ONE);
        glDepthMask(GL_FALSE);

        m->ComputeReelMatrix(reelMatrix);    // compute the reel matrix (recursiv function)

        EXT.UniformMatrix4fv(indexUniformModelMatrix, 1, 0, reelMatrix.Element());
        for (ListPLight::const_iterator it = listLight.begin(); it != listLight.end(); it++)
        {
            const Vector3f  &posLight = (*it)->Position();
            EXT.Uniform4f(indexUniformLight, posLight.Data[0], posLight.Data[1], posLight.Data[2], 1.0f / ((*it)->Radius()));
            (*it)->ColorRGB().Enable(); // set la couleur de la lumiere

            // draw la passe de rendu pour la lumiere courante
            VBO->Draw(m->GetIBO());
        }
        VBO->Descriptor.Data[1].DataType = TexCoordData;
        _lightShader.Disable();

        if (bumpMaping)
        {
            // on desactive la troisieme unite de texture
            EXT.ActiveTexture(GL_TEXTURE2);
            glDisable(GL_TEXTURE_2D);
        }
        // on desactive la seconde unite de texture
        EXT.ActiveTexture(GL_TEXTURE1);
        glDisable(GL_TEXTURE_3D);
        // on desactive la premiere unite de texture
        EXT.ActiveTexture(GL_TEXTURE0);
        glDisable(GL_TEXTURE_2D);
        GL::Texture::ResetCurrentType();

        glDepthMask(GL_TRUE); // reactive le depth mask et desactive le blend
        glDisable(GL_BLEND);

        if (DrawLocalSpace)
            RenderLocalSpace(m);

        m->RenderChilds();
    glPopMatrix();
}

void LightingMaterial::RenderLocalSpace(Mesh *m)
{
    unsigned int *Indices = m->GetIBO()->GetLocalBuffer();

    glDisable(GL_TEXTURE_2D);
    glBegin(GL_LINES);
    for (unsigned int i = 0; i < m->GetIBO()->Size(); i++)
        for (unsigned int j = 0; j < m->GetIBO()->Stride(); j++)
        {
            GL::LightingVertex *v = static_cast<VertexBuffer<LightingVertex>*>(m->GetVBO())->GetLocalBuffer() + Indices[(i * m->GetIBO()->Stride()) + j];
            // tangente en rouge
            GL::Color::Red();
            glVertex3f(v->coord[0], v->coord[1], v->coord[2]);
            glVertex3f(v->coord[0] + v->tan[0], v->coord[1] + v->tan[1], v->coord[2] + v->tan[2]);
            // binormale en vert
            GL::Color::Green();
            glVertex3f(v->coord[0], v->coord[1], v->coord[2]);
            glVertex3f(v->coord[0] + v->bin[0], v->coord[1] + v->bin[1], v->coord[2] + v->bin[2]);
            // normale en bleu
            GL::Color::Blue();
            glVertex3f(v->coord[0], v->coord[1], v->coord[2]);
            glVertex3f(v->coord[0] + v->norm[0], v->coord[1] + v->norm[1], v->coord[2] + v->norm[2]);
        }
    glEnd();
}
