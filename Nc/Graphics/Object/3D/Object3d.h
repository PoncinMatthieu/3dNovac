
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

#ifndef NOVAC_GRAPHIC_OBJECT3D_H_
#define NOVAC_GRAPHIC_OBJECT3D_H_

#include <list>
#include "../Object.h"

namespace Nc
{
    namespace Graphic
    {
        class BoundingBox;

        /// Base class to define a 3d graphical object
        /**
            A 3d object can have childs and parent 3d objects, maybe we should put this feature on the Graphic::Object insteed of Graphic::Object3d ?
        */
        class LGRAPHICS Object3d : public Object
        {
            public:
                Object3d(const Box3f &box, const TMatrix &mat = TMatrix::Identity);
                Object3d(bool createBox = true);
                Object3d(const Object3d &O);
                Object3d &operator = (const Object3d &O);
                virtual ~Object3d();

                /** Copy the objet */
                virtual Object3d        *Clone() const              {return new Object3d(*this);}

                /** Render the Box, to redefine in your own object3D */
                virtual void            Render(ISceneGraph *scene);

                // les 2 fonctions suivante sont gourmante en resources, Transform ne devrai etre utiliser qu'apres le chargement d'un model
                //virtual void            Transform(const TMatrix& M);
                //virtual void            UpdateBoundingBox();

                //accesseurs
                /** Return the childs */
                inline ListPObject3d    &Childs()                   {return _listChild;}
                /** Add a child, and set the parent of the child */
                void                    AddChild(Object3d *o);
                /** Return the parent */
                inline Object3d         *Parent()                   {return _parent;}
                /** Set the parent */
                inline void             SetParent(Object3d *p)      {_parent = p;}

                // operations sur la matrice
                /** Scale the object with the given height */
                void                    HeightScale(float height); // /!\ metre a jour la bounding box avant
                //void                    PosBase(Vector3f &CenterBase);
                /** Compute the recursive model matrix with the parents matrix */
                void                    ComputeReelMatrix(TMatrix &m);

                /** Return the box of the object */
                const Box3f             &Box() const;

            protected:
                /** draw the object, display the box if needed, to redefine */
                virtual void            Draw(ISceneGraph *);

                /** Transform the scene model matrix before rendering the object, could be redefine */
                virtual inline void     TransformModelMatrixToRender(ISceneGraph *scene)    {scene->ModelMatrix().AddTransformation(Matrix);}

                /** free the object, delete childs and box */
                void Free();

                ListPObject3d       _listChild;         ///< list of child
                Object3d            *_parent;           ///< parent of the object
                BoundingBox         *_box;              ///< the box of the object
        };
    }
}

#endif // Object3d_H_INCLUDED
