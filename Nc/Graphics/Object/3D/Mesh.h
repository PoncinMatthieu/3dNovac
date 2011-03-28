
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

    File Created At:        30/04/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------

                    Implementation de la classe "Mesh"

                  Classe permettant de definir un mesh 3D,
                a partir de gVertexBuffer et de gIndexBuffer

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHIC_MESH_H_
#define NC_GRAPHIC_MESH_H_

#include "Object3d.h"

namespace Nc
{
    namespace Graphic
    {
        template<typename VertexType, bool INDEX, typename MaterialPolitic, typename MaterialConfigPolitic>
        class Mesh : public Object3d
        {
            public:
                Mesh(const GL::GeometryBuffer<VertexType, INDEX> &geometry, const Box3f &box, const TMatrix &mat = TMatrix::Identity);
                virtual ~Mesh();

                virtual inline Object3d     *Clone() const          {return new Mesh<VertexType,INDEX, MaterialPolitic, MaterialConfigPolitic>(*this);}

                // accesseur
                inline void     SetMaterialConfig(MaterialConfig<VertexType, MaterialConfigPolitic> *c)     {_materialConfig = c;}

                /** Set the material */
                inline void     SetMaterial(Material<VertexType, MaterialPolitic> *m)                       {_material = m; _material->Configure(_geometry);}

                /** Return the material used to render the mesh */
                inline Material<VertexType, MaterialPolitic>                *GetMaterial()                  {return _material;}

                /** Return the material used to render the mesh */
                inline MaterialConfig<VertexType, MaterialConfigPolitic>    &GetMaterialConfig()            {return _materialConfig;}

                /** Return the GeometryBuffer associated to the mesh */
                inline GL::GeometryBuffer<VertexType,INDEX>  &GetGeometryBuffer()                           {return _geometry;}

            protected:
                /**
                    Display the object, by using the material of the object
                    Could be redefine be your own object
                */
                virtual void Draw(ISceneGraph *scene);

                Material<VertexType, MaterialPolitic>               *_material;             ///< a pointer to the rendering material, if the material isn't set, we use the default material (Material::DefaultMaterial())
                MaterialConfig<VertexType, MaterialConfigPolitic>   _materialConfig;        ///< a pointer to the material config class to configure the rendering material (for exemple to set a texture)
                GL::GeometryBuffer<VertexType, INDEX>               _geometry;
        };

        template<typename VertexType, bool INDEX, typename MaterialPolitic, typename MaterialConfigPolitic>
        Mesh<VertexType, INDEX, MaterialPolitic, MaterialConfigPolitic>::Mesh(const GL::GeometryBuffer<VertexType, INDEX> &geometry, const Box3f &box, const TMatrix &mat)
            : Object3d(box, mat), _geometry(geometry)
        {
            SetMaterial(&Material<VertexType>::Instance());
        }

        template<typename VertexType, bool INDEX, typename MaterialPolitic, typename MaterialConfigPolitic>
        Mesh<VertexType, INDEX, MaterialPolitic, MaterialConfigPolitic>::~Mesh()
        {
        }

        template<typename VertexType, bool INDEX, typename MaterialPolitic, typename MaterialConfigPolitic>
        void Mesh<VertexType, INDEX, MaterialPolitic, MaterialConfigPolitic>::Draw(ISceneGraph *scene)
        {
            _material->Render(scene, _geometry, _materialConfig);
            Object3d::Draw(scene);
        }
    }
}

#endif

