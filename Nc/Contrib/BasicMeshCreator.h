
/*-----------------------------------------------------------------------------

	3dNovac Contrib
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

    File Created At:        24/03/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------

    namespace to create some basic mesh

--------------------------------------------------------------------------------*/

#ifndef NC_CONTRIB_BASICMESHCREATOR_H_
#define NC_CONTRIB_BASICMESHCREATOR_H_

#include <Nc/Graphics/Object/3D/Mesh.h>

namespace Nc
{
    namespace Graphic
    {
        namespace BasicMeshCreator
        {
            // cree un Pave Texture
            //Object3d *Pave(const Vector3f& center, const Vector3f &size, const Color& color, const GL::Texture &texture);

            // cree un plan
            template<typename MaterialPolitic = DefaultMaterialPolitics<BasicVertexType::Textured>, typename MaterialConfigPolitic = DefaultMaterialConfigPolitics<BasicVertexType::Textured> >
            Object3d *Plan(const Vector2f &size, const GL::Texture &texture);

            // cree une grille
            Object3d *Grid(const Vector3f &size, const Color &c);
        }


        namespace BasicMeshCreator
        {
            template<typename MaterialPolitic, typename MaterialConfigPolitic>
            Object3d    *Plan(const Vector2f &size, const GL::Texture &texture)
            {
                Array<BasicVertexType::Textured, 4>     vertices;
                Color color(1, 1, 1);

            // creation des vertex en fontion de la taille de la map
                vertices[0].Fill(0, 0, 0, 0, 0, 0, 0, 1, color);
                vertices[1].Fill(size[0], 0, 0, size[0], 0, 0, 0, 1, color);
                vertices[2].Fill(0, size[1], 0, 0, size[1], 0, 0, 1, color);
                vertices[3].Fill(size[0], size[1], 0, size[0], size[1], 0, 0, 1, color);

            // creation de la box
                Box3f box(Vector3f(0.f, 0.f, -0.1f), Vector3f(size[0], size[1], 0.1f));

            // creation du mesh
                GL::GeometryBuffer<BasicVertexType::Textured, false> geometry(GL::VertexBuffer<BasicVertexType::Textured>(vertices, GL_STATIC_DRAW), GL_TRIANGLE_STRIP);
                Mesh<BasicVertexType::Textured, false, MaterialPolitic, MaterialConfigPolitic>  *mesh = new Mesh<BasicVertexType::Textured, false>(geometry, box);
                mesh->GetMaterialConfig().Texture = texture;
                return mesh;
            }
        }
    }
}

#endif
