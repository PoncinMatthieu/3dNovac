
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

#ifndef NC_GRAPHICS_MATERIAL_PROGRAMMABLEMATERIAL_H_
#define NC_GRAPHICS_MATERIAL_PROGRAMMABLEMATERIAL_H_

#include "../Core/GL/Uniform.h"
#include "IMaterial.h"

namespace Nc
{
    namespace Graphic
    {
        /// Define a Material witch could be directly configurable outside of the class
        /**
            This class helps to create materials rapidely without inheritance of the IMaterial class.
            Keep in mind that using this class could be slower than redefine your own materials by inherite of the IMaterial class.

            To use this class you should configure it by setting the attributes just after it's creation and setting the uniforms value before the rendering.
        */
        class ProgrammableMaterial : public IMaterial
        {
            private:
                typedef std::map<std::string, GL::IUniform*>    UniformMap;
                typedef std::map<unsigned int, int>             UniformDrawablesTextureUnitMap;
                typedef std::map<std::string, unsigned int>     AttribMap;

            public:
                ProgrammableMaterial(const std::string materialName, const Utils::FileName &vertexShader, const Utils::FileName &pixelShader);
                ~ProgrammableMaterial();

                /** Configure the given goemetry */
                virtual bool        Configure(Drawable &drawable);

                /** Render the given drawable with the given model matrix */
                virtual void        Render(SceneGraph *scene, const TMatrix &modelMatrix, Drawable &drawable);

                /** Fetch the attribute location by using the given \p attribName and set it for the given \p componentName */
                void                SetAttrib(const std::string &componentName, const char *attribName);

                /**
                    \return the uniform linked with the given uniform name
                    If the uniform don't already exist, the uniform location will be fetch and the Uniform object created with the given template parameters.
                    For exemple if the uniform is a vec3 in the shader, the template Nb should be 3, and the type T should be float.
                    If the uniform already exist, the uniform object will be dynamically cast by using the template parameters. So you should try to avoid any unecessary call of this function.
                */
                template<typename T, unsigned int Nb>
                GL::Uniform<T,Nb>   *GetUniform(const std::string &name);

                /**
                    Set the texture unit location witch would be used to set the drawables textures,
                    the unit 0 will be linked with the first texture and the unit 1 with the second texture.
                */
                void                SetDrawableTextureUnitUniform(unsigned int unit, const char *name);
                /** Unset the texture unit location. */
                void                UnsetDrawableTextureUnitUniform(unsigned int unit);

                /** Set the uniform MVP matrix location */
                void                SetMVPMatrixUniform(const char *name);
                /** Set the uniform MVP matrix location */
                void                SetMMatrixUniform(const char *name);
                /** Set the uniform MVP matrix location */
                void                SetVPMatrixUniform(const char *name);
                /** Set the uniform normal matrix location */
                void                SetNormalMatrixUniform(const char *name);

                friend LGRAPHICS std::ostream &operator << (std::ostream &oss, const ProgrammableMaterial &m);

            protected:
                GL::Program                         _program;                       ///< the program used to render the drawables
                UniformMap                          _uniforms;                      ///< the uniforms used to configure the program
                int                                 _uniformMVPMatrix;              ///< the uniform of the matrix mvp
                int                                 _uniformMMatrix;                ///< the uniform of the matrix m if the mvp is not set
                int                                 _uniformVPMatrix;               ///< the uniform of the matrix vp if the mvp is not set
                int                                 _uniformNormalMatrix;           ///< the uniform of the normal matrix
                UniformDrawablesTextureUnitMap      _drawablesTextureUnitUniforms;  ///< the uniforns used to set the textures of the drawables
                AttribMap                           _attribs;                       ///< the attributes used to set the vertex attributes by using their descriptor
        };

        template<typename T, unsigned int Nb>
        GL::Uniform<T,Nb>   *ProgrammableMaterial::GetUniform(const std::string &name)
        {
            UniformMap::iterator it = _uniforms.find(name);
            if (it == _uniforms.end())
            {
                GL::Uniform<T,Nb> *uniform = new GL::Uniform<T,Nb>(_program.GetUniformLocation(name.c_str()));
                _uniforms[name] = uniform;
                return uniform;
            }
            return dynamic_cast<GL::Uniform<T,Nb>*>(it->second);
        }
    }
}

#endif
