
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
#include "../Scene/SceneGraph.h"
#include "Drawable.h"
#include "DefaultSourceShader.h"
#include "DefaultVertexType.h"
#include "IMaterial.h"

namespace Nc
{
    namespace Graphic
    {
        /// Interface used to define a default material for vertex types
        class LGRAPHICS IDefaultMaterial : public IMaterial
        {
            public:
                IDefaultMaterial(const std::string &name)
                    : IMaterial(name), _initialized(false)  {}
                virtual ~IDefaultMaterial()                 {}

                /** Initialize the material */
                virtual void    Initialize()                {_initialized = true;}
                /** \return the init statement */
                bool            Initialized()               {return _initialized;}

            private:
                bool    _initialized;       ///< the init statement
        };

        /** Template Base class of a Default Material, this class specialized with the different vertex types */
        template<typename VertexType>
        class DefaultMaterial   {};

        /// Specialisation of the DefaultMaterial for a BasicVertexType::Colored
        template<>
        class DefaultMaterial<DefaultVertexType::Colored> : public IDefaultMaterial
        {
            public:
                DefaultMaterial()
                    : IDefaultMaterial("DefaultMaterial<Colored>")      {}
                ~DefaultMaterial()      {}

                static unsigned int    GetDescriptorPriority()          {return DefaultVertexType::Colored::GetDescriptorPriority();}

                virtual void    Initialize();

                /** configure the goemetry to render it with the good attributes */
                virtual bool    Configure(Drawable &drawable);

                /** Render the drawable (couple of GeometryBuffer and MaterialConfig) with the given scene and model matrix */
                virtual void    Render(SceneGraph *scene, const TMatrix &modelMatrix, Drawable &drawable);

            protected:
                GL::Shader      _shader;            ///< the shader used to render the drawable
                unsigned int    _uniformMVP;
        };

        /// Specialisation of the DefaultMaterial for a BasicVertexType::Colored2d
        template<>
        class DefaultMaterial<DefaultVertexType::Colored2d> : public IDefaultMaterial
        {
            public:
                DefaultMaterial()
                    : IDefaultMaterial("DefaultMaterial<Colored2d>")      {}
                ~DefaultMaterial()     {}

                static unsigned int    GetDescriptorPriority()         {return DefaultVertexType::Colored2d::GetDescriptorPriority();}

                virtual void    Initialize();

                /** configure the goemetry to render it with the good attributes */
                virtual bool    Configure(Drawable &drawable);

                /** Render the drawable (couple of GeometryBuffer and MaterialConfig) with the given scene and model matrix */
                virtual void    Render(SceneGraph *scene, const TMatrix &modelMatrix, Drawable &drawable);

            protected:
                GL::Shader      _shader;        ///< the shader used to render the drawable
                unsigned int    _uniformMVP;
        };

        /// Specialisation of the DefaultMaterial for a BasicVertexType::Textured
        template<>
        class DefaultMaterial<DefaultVertexType::Textured> : public IDefaultMaterial
        {
            public:
                DefaultMaterial()
                    : IDefaultMaterial("DefaultMaterial<Textured>")      {}
                ~DefaultMaterial()      {}

                static unsigned int    GetDescriptorPriority()         {return DefaultVertexType::Textured::GetDescriptorPriority();}

                virtual void    Initialize();

                /** configure the goemetry to render it with the good attributes */
                virtual bool    Configure(Drawable &drawable);

                /** Render the drawable (couple of GeometryBuffer and MaterialConfig) with the given scene and model matrix */
                virtual void    Render(SceneGraph *scene, const TMatrix &modelMatrix, Drawable &drawable);

            protected:
                GL::Shader      _shader;                ///< the shader used to render the drawable
                unsigned int    _uniformMVP;
                unsigned int    _uniformTextured;
                unsigned int    _uniformTextureId;
        };

        /// Specialisation of the DefaultMaterial for a BasicVertexType::Textured2d
        template<>
        class DefaultMaterial<DefaultVertexType::Textured2d> : public IDefaultMaterial
        {
            public:
                DefaultMaterial()
                    : IDefaultMaterial("DefaultMaterial<Textured2d>")      {}
                ~DefaultMaterial()      {}

                static unsigned int    GetDescriptorPriority()         {return DefaultVertexType::Textured2d::GetDescriptorPriority();}

                virtual void    Initialize();

                /** configure the goemetry to render it with the good attributes */
                virtual bool    Configure(Drawable &drawable);

                /** Render the drawable (couple of GeometryBuffer and MaterialConfig) with the given scene and model matrix */
                virtual void    Render(SceneGraph *scene, const TMatrix &modelMatrix, Drawable &drawable);

            protected:
                GL::Shader      _shader;                ///< the shader used to render the drawable
                unsigned int    _uniformMVP;
                unsigned int    _uniformTextured;
                unsigned int    _uniformTextureId;
        };

        /// Specialisation of the DefaultMaterial for a BasicVertexType::Textured3d
        template<>
        class DefaultMaterial<DefaultVertexType::Textured3d> : public IDefaultMaterial
        {
            public:
                DefaultMaterial()
                    : IDefaultMaterial("DefaultMaterial<Textured3d>")      {}
                ~DefaultMaterial()      {}

                static unsigned int    GetDescriptorPriority()         {return DefaultVertexType::Textured3d::GetDescriptorPriority();}

                virtual void    Initialize();

                /** configure the goemetry to render it with the good attributes */
                virtual bool    Configure(Drawable &drawable);

                /** Render the drawable (couple of GeometryBuffer and MaterialConfig) with the given scene and model matrix */
                virtual void    Render(SceneGraph *scene, const TMatrix &modelMatrix, Drawable &drawable);

            protected:
                GL::Shader      _shader;                ///< the shader used to render the drawable
                unsigned int    _uniformMVP;
                unsigned int    _uniformTextured;
                unsigned int    _uniformTextureId;
        };
    }
}

#endif
