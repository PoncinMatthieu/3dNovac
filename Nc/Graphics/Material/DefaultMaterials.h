
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

    File Created At:        25/03/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_MATERIAL_DEFAULTMATERIALS_H_
#define NC_GRAPHICS_MATERIAL_DEFAULTMATERIALS_H_

#include "../Define.h"
#include "../Core/GL/Shader.h"
#include "../Core/GL/GeometryBuffer.h"
#include "../Scene/ISceneGraph.h"
#include "BasicSourceShader.h"
#include "MaterialConfig.h"

namespace Nc
{
    namespace Graphic
    {
        template<typename VertexType>
        class DefaultMaterialPolitics   {};

        template<>
        class DefaultMaterialPolitics<BasicVertexType::Colored>
        {
            public:
                DefaultMaterialPolitics()
                {
                    _shader.LoadFromMemory(BasicSource::ColoredVertexShader, BasicSource::ColoredFragmentShader, "BasicColoredShader");
                    _shader.Enable();
                    _uniformMVP = _shader.Uniform("MVPMatrix");
                    _shader.Disable();
                }
                ~DefaultMaterialPolitics()     {}

                template<bool INDEX>
                void    Configure(GL::GeometryBuffer<BasicVertexType::Colored, INDEX> &geometry)
                {
                    GL::VertexDescriptor  &desc = geometry.GetVBO().Descriptor;
                    _shader.Enable();
                    desc[0].IndexAttrib = _shader.Attrib("InCoord");
                    desc[1].IndexAttrib = _shader.Attrib("InColor");
                    _shader.Disable();
                }

                template<bool INDEX, typename ConfigPolitic>
                void    Render(ISceneGraph *scene, TMatrix &modelMatrix, GL::GeometryBuffer<BasicVertexType::Colored, INDEX> &geometry, MaterialConfig<BasicVertexType::Colored, ConfigPolitic> &config)
                {
                    config.Enable();
                    _shader.Enable(); // enable the shader

                    TMatrix mvp =  scene->ProjectionMatrix() * scene->ViewMatrix() * modelMatrix;
                    glUniformMatrix4fv(_uniformMVP, 1, GL_TRUE, mvp.Elements());

                    geometry.Render(); // render the object

                    _shader.Disable(); // disable the shader
                    config.Disable();
                }

            protected:
                GL::Shader      _shader;
                unsigned int    _uniformMVP;
        };

        template<>
        class DefaultMaterialPolitics<BasicVertexType::Colored2d>
        {
            public:
                DefaultMaterialPolitics()
                {
                    _shader.LoadFromMemory(BasicSource::Colored2dVertexShader, BasicSource::Colored2dFragmentShader, "BasicColored2dShader");
                    _shader.Enable();
                    _uniformMVP = _shader.Uniform("MVPMatrix");
                    _shader.Disable();
                }
                ~DefaultMaterialPolitics()     {}

                template<bool INDEX>
                void    Configure(GL::GeometryBuffer<BasicVertexType::Colored2d, INDEX> &geometry)
                {
                    GL::VertexDescriptor  &desc = geometry.GetVBO().Descriptor;
                    _shader.Enable();
                    desc[0].IndexAttrib = _shader.Attrib("InCoord");
                    desc[1].IndexAttrib = _shader.Attrib("InColor");
                    _shader.Disable();
                }

                template<bool INDEX, typename ConfigPolitic>
                void    Render(ISceneGraph *scene, TMatrix &modelMatrix, GL::GeometryBuffer<BasicVertexType::Colored2d, INDEX> &geometry, MaterialConfig<BasicVertexType::Colored2d, ConfigPolitic> &config)
                {
                    config.Enable();
                    _shader.Enable(); // enable the shader

                    TMatrix mvp =  scene->ProjectionMatrix() * scene->ViewMatrix() * modelMatrix;
                    glUniformMatrix4fv(_uniformMVP, 1, GL_TRUE, mvp.Elements());

                    geometry.Render(); // render the object

                    _shader.Disable(); // disable the shader
                    config.Disable();
                }

            protected:
                GL::Shader      _shader;
                unsigned int    _uniformMVP;
        };

        template<>
        class DefaultMaterialPolitics<BasicVertexType::Textured>
        {
            public:
                DefaultMaterialPolitics()
                {
                    _shader.LoadFromMemory(BasicSource::TexturedVertexShader, BasicSource::TexturedFragmentShader, "BasicTexturedShader");
                    _shader.Enable();
                    _uniformMVP = _shader.Uniform("MVPMatrix");
                    _uniformTextureId = _shader.Uniform("TextureId");
                    _shader.Disable();
                }
                ~DefaultMaterialPolitics()     {}

                template<bool INDEX>
                void    Configure(GL::GeometryBuffer<BasicVertexType::Textured, INDEX> &geometry)
                {
                    GL::VertexDescriptor  &desc = geometry.GetVBO().Descriptor;
                    _shader.Enable();
                    desc[0].IndexAttrib = _shader.Attrib("InCoord");
                    desc[1].IndexAttrib = _shader.Attrib("InTexCoord");
                    desc[2].IndexAttrib = _shader.Attrib("InColor");
                    _shader.Disable();
                }

                template<bool INDEX, typename ConfigPolitic>
                void    Render(ISceneGraph *scene, TMatrix &modelMatrix, GL::GeometryBuffer<BasicVertexType::Textured, INDEX> &geometry, MaterialConfig<BasicVertexType::Textured, ConfigPolitic> &config)
                {
                    config.Enable();
                    _shader.Enable(); // enable the shader

                    TMatrix mvp =  scene->ProjectionMatrix() * scene->ViewMatrix() * modelMatrix;
                    glUniformMatrix4fv(_uniformMVP, 1, GL_TRUE, mvp.Elements());
                    glUniform1i(_uniformTextureId, 0);
                    glActiveTexture(GL_TEXTURE0);
                    config.Texture.Enable();
                    geometry.Render(); // render the object
                    _shader.Disable(); // disable the shader
                    config.Disable();
                }

            protected:
                GL::Shader      _shader;
                unsigned int    _uniformMVP;
                unsigned int    _uniformTextureId;
        };

        template<>
        class DefaultMaterialPolitics<BasicVertexType::Textured2d>
        {
            public:
                DefaultMaterialPolitics()
                {
                    _shader.LoadFromMemory(BasicSource::Textured2dVertexShader, BasicSource::Textured2dFragmentShader, "BasicTextured2dShader");
                    _shader.Enable();
                    _uniformMVP = _shader.Uniform("MVPMatrix");
                    _uniformTextureId = _shader.Uniform("TextureId");
                    _shader.Disable();
                }
                ~DefaultMaterialPolitics()     {}

                template<bool INDEX>
                void    Configure(GL::GeometryBuffer<BasicVertexType::Textured2d, INDEX> &geometry)
                {
                    GL::VertexDescriptor  &desc = geometry.GetVBO().Descriptor;
                    _shader.Enable();
                    desc[0].IndexAttrib = _shader.Attrib("InCoord");
                    desc[1].IndexAttrib = _shader.Attrib("InTexCoord");
                    desc[2].IndexAttrib = _shader.Attrib("InColor");
                    _shader.Disable();
                }

                template<bool INDEX, typename ConfigPolitic>
                void    Render(ISceneGraph *scene, TMatrix &modelMatrix, GL::GeometryBuffer<BasicVertexType::Textured2d, INDEX> &geometry, MaterialConfig<BasicVertexType::Textured2d, ConfigPolitic> &config)
                {
                    config.Enable();
                    _shader.Enable(); // enable the shader

                    TMatrix mvp =  scene->ProjectionMatrix() * scene->ViewMatrix() * modelMatrix;
                    glUniformMatrix4fv(_uniformMVP, 1, GL_TRUE, mvp.Elements());
                    glUniform1i(_uniformTextureId, 0);
                    glActiveTexture(GL_TEXTURE0);
                    config.Texture.Enable();
                    geometry.Render(); // render the object
                    _shader.Disable(); // disable the shader
                    config.Disable();
                }

            protected:
                GL::Shader      _shader;
                unsigned int    _uniformMVP;
                unsigned int    _uniformTextureId;
        };

        template<>
        class DefaultMaterialPolitics<BasicVertexType::Textured3d>
        {
            public:
                DefaultMaterialPolitics()
                {
                    _shader.LoadFromMemory(BasicSource::Textured3dVertexShader, BasicSource::Textured3dFragmentShader, "BasicTextured3dShader");
                    _shader.Enable();
                    _uniformMVP = _shader.Uniform("MVPMatrix");
                    _uniformTextureId = _shader.Uniform("TextureId");
                    _shader.Disable();
                }
                ~DefaultMaterialPolitics()     {}

                template<bool INDEX>
                void    Configure(GL::GeometryBuffer<BasicVertexType::Textured3d, INDEX> &geometry)
                {
                    GL::VertexDescriptor  &desc = geometry.GetVBO().Descriptor;
                    _shader.Enable();
                    desc[0].IndexAttrib = _shader.Attrib("InCoord");
                    desc[1].IndexAttrib = _shader.Attrib("InTexCoord");
                    desc[2].IndexAttrib = _shader.Attrib("InColor");
                    _shader.Disable();
                }

                template<bool INDEX, typename ConfigPolitic>
                void    Render(ISceneGraph *scene, TMatrix &modelMatrix, GL::GeometryBuffer<BasicVertexType::Textured3d, INDEX> &geometry, MaterialConfig<BasicVertexType::Textured3d, ConfigPolitic> &config)
                {
                    config.Enable();
                    _shader.Enable(); // enable the shader

                    TMatrix mvp =  scene->ProjectionMatrix() * scene->ViewMatrix() * modelMatrix;
                    glUniformMatrix4fv(_uniformMVP, 1, GL_TRUE, mvp.Elements());
                    glUniform1i(_uniformTextureId, 0);
                    glActiveTexture(GL_TEXTURE0);
                    config.Texture.Enable();
                    geometry.Render(); // render the object
                    _shader.Disable(); // disable the shader
                    config.Disable();
                }

            protected:
                GL::Shader      _shader;
                unsigned int    _uniformMVP;
                unsigned int    _uniformTextureId;
        };
    }
}

#endif // NC_GRAPHICS_MATERIAL_BASICMATERIAL_H_
