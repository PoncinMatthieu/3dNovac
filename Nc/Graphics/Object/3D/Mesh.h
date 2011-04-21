
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

#ifndef NC_GRAPHIC_MESH_H_
#define NC_GRAPHIC_MESH_H_

#include "Object3d.h"

namespace Nc
{
    namespace Graphic
    {
        /// Base class to render a 3d mesh using a vertex type a material politic and a material config politic
        /**
            A mesh is composed by a list of Drawable
        */
        template<typename VertexType, bool INDEX, typename MaterialPolitic, typename MaterialConfigPolitic>
        class Mesh : public Object3d
        {
            public:
                typedef std::list<Drawable<VertexType, INDEX, MaterialConfigPolitic> >      ListDrawable;       ///< the list of drawable used by the mesh

            public:
                Mesh(const Box3f &box, const TMatrix &mat = TMatrix::Identity);
                virtual ~Mesh();

                /** Copy the mesh */
                virtual inline Object3d     *Clone() const          {return new Mesh<VertexType,INDEX, MaterialPolitic, MaterialConfigPolitic>(*this);}

                /** Set the material instance used to render the mesh */
                inline void     SetMaterial(Material<VertexType, MaterialPolitic> *m)           {_material = m;}

                /** Return the material used to render the mesh */
                inline Material<VertexType, MaterialPolitic>                *GetMaterial()      {return _material;}

                /** Create a new Drawable, and return its pointer */
                Drawable<VertexType, INDEX, MaterialConfigPolitic>          *NewDrawable()      {_drawables.push_back(Drawable<VertexType, INDEX, MaterialConfigPolitic>()); return &(*_drawables.begin());}

                /** Add an existing drawable */
                void    AddDrawable(const Drawable<VertexType, INDEX, MaterialConfigPolitic> &drawable)     {_drawables.push_back(drawable);}

                /** Configure the list of drawables with the good material */
                void    ConfigureDrawables();

            protected:
                /**
                    Display the object, by using the material of the object.
                    Could be redefine be your own object
                */
                virtual void Draw(ISceneGraph *scene);

                Material<VertexType, MaterialPolitic>       *_material;         ///< A pointer to the rendering material, if the material isn't set, we use the default material (Material::DefaultMaterial())
                ListDrawable                                _drawables;         ///< The list of drawable that define the configs and geometries of the mesh
        };

        template<typename VertexType, bool INDEX, typename MaterialPolitic, typename MaterialConfigPolitic>
        Mesh<VertexType, INDEX, MaterialPolitic, MaterialConfigPolitic>::Mesh(const Box3f &box, const TMatrix &mat)
            : Object3d(box, mat)
        {
            SetMaterial(&Material<VertexType, MaterialPolitic>::Instance());
        }

        template<typename VertexType, bool INDEX, typename MaterialPolitic, typename MaterialConfigPolitic>
        Mesh<VertexType, INDEX, MaterialPolitic, MaterialConfigPolitic>::~Mesh()
        {
        }

        template<typename VertexType, bool INDEX, typename MaterialPolitic, typename MaterialConfigPolitic>
        void Mesh<VertexType, INDEX, MaterialPolitic, MaterialConfigPolitic>::Draw(ISceneGraph *scene)
        {
            for (typename ListDrawable::iterator it = _drawables.begin(); it != _drawables.end(); ++it)
                _material->Render(scene, *it);
            Object3d::Draw(scene);
        }

        template<typename VertexType, bool INDEX, typename MaterialPolitic, typename MaterialConfigPolitic>
        void Mesh<VertexType, INDEX, MaterialPolitic, MaterialConfigPolitic>::ConfigureDrawables()
        {
            for (typename ListDrawable::iterator it = _drawables.begin(); it != _drawables.end(); ++it)
                _material->Configure(*it);
        }
    }
}

#endif

