
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

    File Created At:        18/05/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------

                    Implementation de la classe "gSkyBox"

                Classe permettant d'afficher une sky box en background
                la skyBox doit etre affiche la premiere dans le rendu

-----------------------------------------------------------------------------*/

#ifndef NOVAC_GRAPHIC_SKYBOX_H_
#define NOVAC_GRAPHIC_SKYBOX_H_

#include "Object3d.h"

namespace Nc
{
    namespace Graphic
    {
        class Camera3d;

        class LGRAPHICS SkyBox : public Object3d
        {
            public:
                SkyBox(const Camera3d *camera, const Utils::FileName filenames[]);
                virtual ~SkyBox();

                virtual inline Object3d     *Clone() const          {return new SkyBox(*this);}

            protected:
                virtual void    TransformModelMatrixToRender(ISceneGraph *scene);
                virtual void    Draw(ISceneGraph *scene);
                void            UpdateGeometry();

                Material<BasicVertexType::Textured3d>                   *_material;
                MaterialConfig<BasicVertexType::Textured3d>             _config;
                GL::GeometryBuffer<BasicVertexType::Textured3d>         _geometry;

                const Camera3d      *_camera;
        };
    }
}

#endif
