
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

    File Created At:        17/09/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_SCENE_VISITORS_H_
#define NC_GRAPHICS_SCENE_VISITORS_H_

#include <Nc/Graphics/Scene/SceneNodeVisitor.h>
#include "../Object/Object.h"

namespace Nc
{
    namespace Graphic
    {
        namespace Visitor
        {
            struct IsEnableRecursive : public Graphic::SceneNodeVisitor<IsEnableRecursive, true>
            {
                IsEnableRecursive()
                    : Graphic::SceneNodeVisitor<IsEnableRecursive, true>( Utils::Metaprog::Seq<ISceneNode>::Type(),
                                                                          Utils::Metaprog::Seq<Graphic::Entity, Graphic::Octree>::Type(), Graph::VisitParents),
                      result(true)
                    {}

                void VisitNode(const ISceneNode &n)         {if (result && !n.Enabled()) result = false;}

                bool    result;
            };


            struct GetRecursiveMatrix : public Graphic::SceneNodeVisitor<GetRecursiveMatrix>
            {
                GetRecursiveMatrix(TMatrix &m)
                    : Graphic::SceneNodeVisitor<GetRecursiveMatrix>(Utils::Metaprog::Seq<Entity>::Type(),
                                                                    Utils::Metaprog::Seq<Graphic::Entity, Graphic::Octree>::Type(), Graph::VisitParents),
                      matrix(m)
                    {}

                void VisitNode(Entity &n)       {matrix.AddTransformation(n.Matrix);}

                TMatrix  &matrix;
            };


            struct SetMaterial : public Graphic::SceneNodeVisitor<SetMaterial>
            {
                SetMaterial(IMaterial *m)
                    : Graphic::SceneNodeVisitor<SetMaterial>(Utils::Metaprog::Seq<Object>::Type(),
                                                             Utils::Metaprog::Seq<Graphic::Entity, Graphic::Octree>::Type()),
                      newMaterial(m)
                    {}

                void VisitNode(Object &n)       {n.SetMaterial(newMaterial);}

                IMaterial *newMaterial;
            };

        }
    }
}

#endif
