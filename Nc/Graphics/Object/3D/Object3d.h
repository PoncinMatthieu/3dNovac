
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
/*-----------------------------------------------------------------------------


                    Implementation de la classe "Object3d"

        Classe de base permettant de definir un objet graphique 3D,
                        contient une list de meshs

Hérite de Object

-----------------------------------------------------------------------------*/


#ifndef NOVAC_GRAPHIC_OBJECT3D_H_
#define NOVAC_GRAPHIC_OBJECT3D_H_

#include <list>
#include "../Object.h"

namespace Nc
{
    namespace Graphic
    {
        class BoundingBox;

        class LGRAPHICS Object3d : public Object
        {
            public:
                //Object3d(std::list<Mesh*> &listMesh, const Box3f &box = Box3f(), const TMatrix &mat = TMatrix::Identity);
                //Object3d(Mesh *mesh);
                Object3d(bool createBox = true);
                Object3d(const Object3d &O);
                Object3d &operator = (const Object3d &O);
                virtual ~Object3d();

                virtual Object3d        *Clone() const              {return new Object3d(*this);}

                /** Render the Box, to redefine in your own object3D */
                virtual void            Render(ISceneGraph *scene);

                // les 2 fonctions suivante sont gourmante en resources, Transform ne devrai etre utiliser qu'apres le chargement d'un model
                //virtual void            Transform(const TMatrix& M);
                //virtual void            UpdateBoundingBox();

                //accesseurs
                inline ListPObject3D    &Childs()                   {return _listChild;}
                void                    AddChild(Object3d *o);
                inline void             SetParent(Object3d *p)      {_parent = p;}
                inline Object3d         *Parent()                   {return _parent;}

                // operations sur la matrice
                void                    HeightScale(float height); // /!\ metre a jour la bounding box avant
                //void                    PosBase(Vector3f &CenterBase);
                void                    ComputeReelMatrix(TMatrix &m);

                // box
                const Box3f             &Box() const;

            protected:
                /** draw the object, to redefine */
                virtual void            Draw(ISceneGraph *);

                virtual inline void     TransformModelMatrixToRender(ISceneGraph *scene)    {scene->ModelMatrix().AddTransformation(Matrix);}

                Object3d(const Box3f &box, const TMatrix &mat = TMatrix::Identity);
                void Free();

                ListPObject3D       _listChild;
                Object3d            *_parent;
                BoundingBox         *_box;
        };
    }
}

#endif // Object3d_H_INCLUDED
