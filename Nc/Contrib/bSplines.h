
/*--------------------------------------------------------------------------------

	3dNovac Engine
	Copyright (C) 2010, Poncin Matthieu
	All rights reserved.

	This work is licensed under the Creative Commons
	Attribution-Share Alike 2.0 France License.
	To view a copy of this license, visit :
	http://creativecommons.org/licenses/by-sa/2.0/fr/

    cree le :        04/05/2010

--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------

    Implementation de la classe "bSplines"
    Permet de dessiner des splines en 2d

--------------------------------------------------------------------------------*/

#ifndef NOVAC_GRAPHIC_2D_BSPLINES_H_
#define NOVAC_GRAPHIC_2D_BSPLINES_H_

#include "Object2D.h"

namespace Nc
{
    namespace Graphic
    {
        class LGRAPHICS bSplines : public Object2D
        {
            public:
                bSplines(const Array<Vector2f> &controlPoints, const Array<int> &weight, const TMatrix &matrice);
                bSplines(const Array<Vector2f> &controlPoints, const Array<int> &weight, const Array<int> &knots, const TMatrix &matrice);
                virtual ~bSplines();

                void            Update();
                virtual void    Render(GLContext &renderer);

                Vector2f        &Vertex(unsigned int i)     {return _controlPoints[i];}
                unsigned int    NbPoint()                   {return _controlPoints.Size();}

            private:
                void InitOption();

                void DrawControlPoints(const Color &c1, const Color &c2);
                void DrawbSpline(const Color &c);
                void DrawNurb(const Color &c);

                void ComputeKnots(Array<int> &knots);
                void SplinePoint(int n, float v, Vector2f &point);
                void NurbPoint(int n, float v, Vector2f &point);
                float SplineBlend(int k, int t, float v);

                Array<Vector2f>         _controlPoints;     // points de control des splines
                Array<int>              _weight;            // les poids de chaque point de control dans le cas de la representation des nurbs
                Array<int>              _knots;             // noeud
                int                     _degree;            // degree des splines
                unsigned int            _resolution;        // nombres de points de la courbe

                GL::DisplayList         _displayList;
                System::Mutex           _mutex;
        };
    }
}

#endif
