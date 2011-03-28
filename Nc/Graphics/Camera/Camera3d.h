
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

    File Created At:        13/09/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------


                Implementation de la classe abstraite "gCamera"

                Classe mere permettant la definition d'une camera

On considere qu'une camera doit pouvoir recevoir les evennements souris et claviers
de l'application pour pouvoir effecuer correctement les calcul de deplacement de
camera

ainsi qu'une fonction de calcul avec le temps ecoulé en paramêtre pour certaine
camera comme la camera freefly

Et enfin une fonction permettant de fixé la camera sur la scene


Derivé par : gCameraTrackball
             gCameraFreefly     TODO

-----------------------------------------------------------------------------*/


#ifndef NC_GRAPHIC_CAMERA_CAMERA3D_H_
#define NC_GRAPHIC_CAMERA_CAMERA3D_H_

#include "Camera.h"
#include "../Object/3D/BasicMeshCreator.h"

namespace Nc
{
    namespace Graphic
    {
        class LGRAPHICS Camera3d : public Camera, public Object3d
        {
            public:
                Camera3d(double ratioAspect, double near, double far, double fielOfView)
                    : Object3d(false), _center(0, 0, 0), _up(0, 0, 1), _ratioAspect(ratioAspect), _near(near), _far(far), _fieldOfView(fielOfView)
                {}
                virtual ~Camera3d() {};

                virtual void    MouseMotionEvent(const Nc::System::Event &event) = 0;
                virtual void    MouseButtonEvent(const Nc::System::Event &event) = 0;
                virtual void    KeyboardEvent(const Nc::System::Event &event) = 0;

                virtual void    Update(float RunningTime) = 0; // Running Time in second

                inline void     UpdateProjection()          {_projectionMatrix->SetProjection(_ratioAspect, _near, _far, _fieldOfView);}
                virtual void    Fix()                       {Camera::Fix(); _viewMatrix->SetLookAt(_eye, _center, _up);}

//                Vector3f                Get3dCoordinateFromProjection(int x, int y); // TODO: recode gluUnProject who is deprecated

                /// accesseurs
                inline const    Vector3f &Eye() const           {return _eye;}
                inline void     Eye(Vector3f &eye)              {_eye = eye;}
                inline const    Vector3f &Center() const        {return _center;}
                inline void     Center(Vector3f &pos)           {_center = pos; MajEye();}

            protected:
                virtual void MajEye() = 0;

                // eye properties
                Vector3f    _eye;
                Vector3f    _center;
                Vector3f    _up;

                // projection properties
                float       _ratioAspect;
                float       _near;
                float       _far;
                float       _fieldOfView;
        };
    }
}

/*
// deprecated, recode gluUnProject
Vector3f GLContext::Get3dCoordinateFromProjection(int x, int y)
{
    int viewport[4];
//	double modelview[16], projection[16];
	float winX, winY, winZ;
	double posX, posY, posZ;

//	glGetDoublev(GL_MODELVIEW_MATRIX, modelview); // deprecated
//	glGetDoublev(GL_PROJECTION_MATRIX, projection); // deprecated

	glGetIntegerv(GL_VIEWPORT, viewport);

	winX = (float)x;
	winY = (float)viewport[3] - (float)y; // soustrait directement la taille de la fenetre a la coordonnee y (car l'origine de y est positionne en haut de la fenetre)
	glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ); // recupere la coordonnee z de la fenetre

    // transformer les coordonnees de la fenetre en coordonnees
	gluUnProject(winX, winY, winZ, ModelViewMatrix.Element(), ProjectionMatrix.Element(), viewport, &posX, &posY, &posZ); // deprecated
	return Vector3f(posX, posY, posZ);
}
*/


#endif
