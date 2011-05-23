
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
        /// The default template material politics for the basic VertexType that define patterns of Material
        /**
            Show the specialisations for more detail, there are one specialisation per VertexType
        */
        template<typename VertexType>
        class DefaultMaterialPolitics   {};

        /// Specialisation of the DefaultMaterialPolitics for a BasicVertexType::Colored
        template<>
        class DefaultMaterialPolitics<BasicVertexType::Colored>
        {
            public:
                DefaultMaterialPolitics()
                {
                    _shader.LoadFromMemory(BasicSource::ColoredVertexShader, BasicSource::ColoredFragmentShader, "BasicColoredShader");
                    _shader.Enable();
                    _uniformMVP = _shader.UniformLocation("MVPMatrix");
                    _shader.Disable();
                }
                ~DefaultMaterialPolitics()     {}

                /** configure the goemetry to render it with the good attributes */
                void    Configure(GL::IGeometryBuffer<BasicVertexType::Colored> &geometry)
                {
                    GL::VertexDescriptor  &desc = geometry.GetVBO().Descriptor;
                    _shader.Enable();
                    desc[0].IndexAttrib = _shader.AttribLocation("InCoord");
                    desc[1].IndexAttrib = _shader.AttribLocation("InColor");
                    _shader.Disable();
                }

                /** Render the drawable (couple of GeometryBuffer and MaterialConfig) with the given scene and model matrix */
                template<bool INDEX, typename ConfigPolitic>
                void    Render(ISceneGraph *scene, const TMatrix &modelMatrix, Drawable<BasicVertexType::Colored, INDEX, ConfigPolitic> &drawable)
                {
                    drawable.Enable();
                    _shader.Enable(); // enable the shader

                    TMatrix mvp =  scene->ProjectionMatrix() * scene->ViewMatrix() * modelMatrix;
                    glUniformMatrix4fv(_uniformMVP, 1, GL_TRUE, mvp.Elements());

                    drawable.Render(); // render the object

                    _shader.Disable(); // disable the shader
                    drawable.Disable();
                }

            protected:
                GL::Shader      _shader;            ///< the shader used to render the drawable
                unsigned int    _uniformMVP;
        };

        /// Specialisation of the DefaultMaterialPolitics for a BasicVertexType::Colored2d
        template<>
        class DefaultMaterialPolitics<BasicVertexType::Colored2d>
        {
            public:
                DefaultMaterialPolitics()
                {
                    _shader.LoadFromMemory(BasicSource::Colored2dVertexShader, BasicSource::Colored2dFragmentShader, "BasicColored2dShader");
                    _shader.Enable();
                    _uniformMVP = _shader.UniformLocation("MVPMatrix");
                    _shader.Disable();
                }
                ~DefaultMaterialPolitics()     {}

                /** configure the goemetry to render it with the good attributes */
                void    Configure(GL::IGeometryBuffer<BasicVertexType::Colored2d> &geometry)
                {
                    GL::VertexDescriptor  &desc = geometry.GetVBO().Descriptor;
                    _shader.Enable();
                    desc[0].IndexAttrib = _shader.AttribLocation("InCoord");
                    desc[1].IndexAttrib = _shader.AttribLocation("InColor");
                    _shader.Disable();
                }

                /** Render the drawable (couple of GeometryBuffer and MaterialConfig) with the given scene and model matrix */
                template<bool INDEX, typename ConfigPolitic>
                void    Render(ISceneGraph *scene, const TMatrix &modelMatrix, Drawable<BasicVertexType::Colored2d, INDEX, ConfigPolitic> &drawable)
                {
                    drawable.Enable();
                    _shader.Enable(); // enable the shader

                    TMatrix mvp =  scene->ProjectionMatrix() * scene->ViewMatrix() * modelMatrix;
                    glUniformMatrix4fv(_uniformMVP, 1, GL_TRUE, mvp.Elements());

                    drawable.Render(); // render the object

                    _shader.Disable(); // disable the shader
                    drawable.Disable();
                }

            protected:
                GL::Shader      _shader;        ///< the shader used to render the drawable
                unsigned int    _uniformMVP;
        };

        /// Specialisation of the DefaultMaterialPolitics for a BasicVertexType::Textured
        template<>
        class DefaultMaterialPolitics<BasicVertexType::Textured>
        {
            public:
                DefaultMaterialPolitics()
                {
                    _shader.LoadFromMemory(BasicSource::TexturedVertexShader, BasicSource::TexturedFragmentShader, "BasicTexturedShader");
                    _shader.Enable();
                    _uniformMVP = _shader.UniformLocation("MVPMatrix");
                    _uniformTextured = _shader.UniformLocation("Textured");
                    _uniformTextureId = _shader.UniformLocation("TextureId");
                    _shader.Disable();
                }
                ~DefaultMaterialPolitics()     {}

                /** configure the goemetry to render it with the good attributes */
                void    Configure(GL::IGeometryBuffer<BasicVertexType::Textured> &geometry)
                {
                    GL::VertexDescriptor  &desc = geometry.GetVBO().Descriptor;
                    _shader.Enable();
                    desc[0].IndexAttrib = _shader.AttribLocation("InCoord");
                    desc[1].IndexAttrib = _shader.AttribLocation("InTexCoord");
                    desc[2].IndexAttrib = _shader.AttribLocation("InColor");
                    _shader.Disable();
                }

                /** Render the drawable (couple of GeometryBuffer and MaterialConfig) with the given scene and model matrix */
                template<typename ConfigPolitic>
                void    Render(ISceneGraph *scene, const TMatrix &modelMatrix, IDrawable<BasicVertexType::Textured, ConfigPolitic> &drawable)
                {
                    drawable.Enable();
                    _shader.Enable(); // enable the shader

                    TMatrix mvp =  scene->ProjectionMatrix() * scene->ViewMatrix() * modelMatrix;
                    glUniformMatrix4fv(_uniformMVP, 1, GL_TRUE, mvp.Elements());
                    glUniform1i(_uniformTextured, drawable.texture.IsValid());
                    if (drawable.texture.IsValid())
                    {
                        glUniform1i(_uniformTextureId, 0);
                        glActiveTexture(GL_TEXTURE0);
                        drawable.texture.Enable();
                    }

                    drawable.Render(); // render the object
                    _shader.Disable(); // disable the shader
                    drawable.Disable();
                }

            protected:
                GL::Shader      _shader;                ///< the shader used to render the drawable
                unsigned int    _uniformMVP;
                unsigned int    _uniformTextured;
                unsigned int    _uniformTextureId;
        };

        /// Specialisation of the DefaultMaterialPolitics for a BasicVertexType::Textured2d
        template<>
        class DefaultMaterialPolitics<BasicVertexType::Textured2d>
        {
            public:
                DefaultMaterialPolitics()
                {
                    _shader.LoadFromMemory(BasicSource::Textured2dVertexShader, BasicSource::Textured2dFragmentShader, "BasicTextured2dShader");
                    _shader.Enable();
                    _uniformMVP = _shader.UniformLocation("MVPMatrix");
                    _uniformTextured = _shader.UniformLocation("Textured");
                    _uniformTextureId = _shader.UniformLocation("TextureId");
                    _shader.Disable();
                }
                ~DefaultMaterialPolitics()     {}

                /** configure the goemetry to render it with the good attributes */
                void    Configure(GL::IGeometryBuffer<BasicVertexType::Textured2d> &geometry)
                {
                    GL::VertexDescriptor  &desc = geometry.GetVBO().Descriptor;
                    _shader.Enable();
                    desc[0].IndexAttrib = _shader.AttribLocation("InCoord");
                    desc[1].IndexAttrib = _shader.AttribLocation("InTexCoord");
                    desc[2].IndexAttrib = _shader.AttribLocation("InColor");
                    _shader.Disable();
                }

                /** Render the drawable (couple of GeometryBuffer and MaterialConfig) with the given scene and model matrix */
                template<bool INDEX, typename ConfigPolitic>
                void    Render(ISceneGraph *scene, const TMatrix &modelMatrix, Drawable<BasicVertexType::Textured2d, INDEX, ConfigPolitic> &drawable)
                {
                    drawable.Enable();
                    _shader.Enable(); // enable the shader

                    TMatrix mvp =  scene->ProjectionMatrix() * scene->ViewMatrix() * modelMatrix;
                    glUniformMatrix4fv(_uniformMVP, 1, GL_TRUE, mvp.Elements());
                    glUniform1i(_uniformTextured, drawable.texture.IsValid());
                    if (drawable.texture.IsValid())
                    {
                        glUniform1i(_uniformTextureId, 0);
                        glActiveTexture(GL_TEXTURE0);
                        drawable.texture.Enable();
                    }
                    drawable.Render(); // render the object
                    _shader.Disable(); // disable the shader
                    drawable.Disable();
                }

            protected:
                GL::Shader      _shader;                ///< the shader used to render the drawable
                unsigned int    _uniformMVP;
                unsigned int    _uniformTextured;
                unsigned int    _uniformTextureId;
        };

        /// Specialisation of the DefaultMaterialPolitics for a BasicVertexType::Textured3d
        template<>
        class DefaultMaterialPolitics<BasicVertexType::Textured3d>
        {
            public:
                DefaultMaterialPolitics()
                {
                    _shader.LoadFromMemory(BasicSource::Textured3dVertexShader, BasicSource::Textured3dFragmentShader, "BasicTextured3dShader");
                    _shader.Enable();
                    _uniformMVP = _shader.UniformLocation("MVPMatrix");
                    _uniformTextureId = _shader.UniformLocation("TextureId");
                    _uniformTextured = _shader.UniformLocation("Textured");
                    _shader.Disable();
                }
                ~DefaultMaterialPolitics()     {}

                /** configure the goemetry to render it with the good attributes */
                void    Configure(GL::IGeometryBuffer<BasicVertexType::Textured3d> &geometry)
                {
                    GL::VertexDescriptor  &desc = geometry.GetVBO().Descriptor;
                    _shader.Enable();
                    desc[0].IndexAttrib = _shader.AttribLocation("InCoord");
                    desc[1].IndexAttrib = _shader.AttribLocation("InTexCoord");
                    desc[2].IndexAttrib = _shader.AttribLocation("InColor");
                    _shader.Disable();
                }

                /** Render the drawable (couple of GeometryBuffer and MaterialConfig) with the given scene and model matrix */
                template<bool INDEX, typename ConfigPolitic>
                void    Render(ISceneGraph *scene, const TMatrix &modelMatrix, Drawable<BasicVertexType::Textured3d, INDEX, ConfigPolitic> &drawable)
                {
                    drawable.Enable();
                    _shader.Enable(); // enable the shader

                    TMatrix mvp =  scene->ProjectionMatrix() * scene->ViewMatrix() * modelMatrix;
                    glUniformMatrix4fv(_uniformMVP, 1, GL_TRUE, mvp.Elements());
                    glUniform1i(_uniformTextured, drawable.texture.IsValid());
                    if (drawable.texture.IsValid())
                    {
                        glUniform1i(_uniformTextureId, 0);
                        glActiveTexture(GL_TEXTURE0);
                        drawable.texture.Enable();
                    }
                    drawable.Render(); // render the object
                    _shader.Disable(); // disable the shader
                    drawable.Disable();
                }

            protected:
                GL::Shader      _shader;                ///< the shader used to render the drawable
                unsigned int    _uniformMVP;
                unsigned int    _uniformTextured;
                unsigned int    _uniformTextureId;
        };
    }
}

#endif
