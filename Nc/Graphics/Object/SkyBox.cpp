
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

    File Created At:        06/09/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "../Camera/Camera3d.h"
#include "SkyBox.h"

using namespace std;
using namespace Nc;
using namespace Nc::Graphic;

SkyBox::SkyBox(const Utils::FileName filenames[])
    : Object()
{
    // construction de la geometry
    UpdateGeometry(filenames);
}

SkyBox::~SkyBox()
{
}

void SkyBox::TransformModelMatrixToRender(SceneGraph *scene)
{
    // positionne la matrice sur l'eye, et rend l'objet
    if (scene->CurrentCamera()->Is3d())
        Matrix.Translation(static_cast<Camera3d*>(scene->CurrentCamera())->Eye() - static_cast<Camera3d*>(scene->CurrentCamera())->Center());

    // transformation basique
    Object::TransformModelMatrixToRender(scene);
}

void SkyBox::UpdateGeometry(const Utils::FileName filenames[])
{
    Array<DefaultVertexType::Textured3d, 14>  vertices;
    Array<unsigned int, 12*3>               indices;
    Color col(1, 1, 1);

    vertices[0].Fill(-1.f,-1.f,-1.f, -1.f,-1.f,-1.f, col);
    vertices[1].Fill(-1.f,1.f,-1.f, -1.f,1.f,-1.f, col);
    vertices[2].Fill(1.f,1.f,-1.f, 1.f,1.f,-1.f, col);
    vertices[3].Fill(1.f,-1.f,-1.f, 1.f,-1.f,-1.f, col);

    vertices[4].Fill(-1.f,-1.f,1.f, -1.f,-1.f,1.f, col);
    vertices[5].Fill(-1.f,1.f,1.f, -1.f,1.f,1.f, col);
    vertices[6].Fill(1.f,1.f,1.f, 1.f,1.f,1.f, col);
    vertices[7].Fill(1.f,-1.f,1.f, 1.f,-1.f,1.f, col);

    // Bottom/Top
    indices[0] = 0; indices[1] = 1; indices[2] = 2;
    indices[3] = 2; indices[4] = 3; indices[5] = 0;
    indices[6] = 4; indices[7] = 5; indices[8] = 7;
    indices[9] = 5; indices[10] = 6; indices[11] = 7;

    // edges
    indices[12] = 0; indices[13] = 4; indices[14] = 7;  // X+
    indices[15] = 0; indices[16] = 7; indices[17] = 3;
    indices[18] = 3; indices[19] = 7; indices[20] = 6;  // Y+
    indices[21] = 3; indices[22] = 6; indices[23] = 2;
    indices[24] = 2; indices[25] = 6; indices[26] = 5;  // X-
    indices[27] = 2; indices[28] = 5; indices[29] = 1;
    indices[30] = 1; indices[31] = 5; indices[32] = 4;  // y-
    indices[33] = 1; indices[34] = 4; indices[35] = 0;

    // creation du drawable
    _drawables.push_back(new Drawable(vertices, GL::Enum::StaticDraw, indices, 3, GL::Enum::Triangles));

    // load la cubemap et disable le test de profondeur
    _drawables[0]->Config->Textures.InitSize(1);
    _drawables[0]->Config->Textures[0].LoadCubeMap(filenames);
    _drawables[0]->Config->RasterMode().SetDepthTest(false);
    ChooseDefaultMaterial();
}
