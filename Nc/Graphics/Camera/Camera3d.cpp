
/*-----------------------------------------------------------------------------

	3dNovac Graphics
	Copyright (C) 2010-2011, 3dNovac Team

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

    File Created At:        10/06/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "Camera3d.h"

using namespace Nc;
using namespace Nc::Graphic;

Camera3d::Camera3d(Window *attachedWindow, float ratioAspect, float nearf, float farf, float fieldOfView)
    : Camera(attachedWindow, false),
      _center(0, 0, 0), _up(0, 0, 1),
      _ratioAspect(ratioAspect), _near(nearf), _far(farf), _fieldOfView(fieldOfView),
      _viewMatrixUpdated(true)
{
    UpdateProjectionFrustum();
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

void Camera3d::SetProjection(float ratioAspect, float nearf, float farf, float fieldOfView)
{
    _ratioAspect = ratioAspect;

    _fieldOfView = fieldOfView;
    UpdateProjectionFrustum();
    _resized = true;
	_near = nearf;
    _far = farf;
}

void    Camera3d::UpdateProjection(SceneGraph *scene)
{
    scene->ProjectionMatrix().SetProjection(_ratioAspect, _near, _far, _fieldOfView);
}

void    Camera3d::Fix(SceneGraph *scene)
{
    Camera::Fix(scene);
    if (_viewMatrixUpdated)
        scene->ViewMatrix() = _viewMatrix;
}

void    Camera3d::Resized(const Vector2ui &size)
{
    _ratioAspect = (float)size.Data[0]/(float)size.Data[1];
    Camera::Resized(size);
}

void Camera3d::UpdateViewMatrix()
{
    _viewMatrix.SetLookAt(_eye, _center, _up);
    _viewMatrixUpdated = true;
}

void Camera3d::UpdateProjectionFrustum()
{
    _frustumTanFOV = tanf(Utils::Convert::DegToRad(_fieldOfView / 2.f));
    _hNear  = _frustumTanFOV * _near;
    _hFar   = _frustumTanFOV * _far;
    _wNear  = _hNear * _ratioAspect;
    _wFar   = _hFar * _ratioAspect;
}

void Camera3d::UpdateViewFrustum()
{
    _frustumZ = _eye - _center;
    _frustumZ.Normalize();

    Vector3f    nc = _eye - _frustumZ * _near;
    Vector3f    fc = _eye - _frustumZ * _far;

    _up.Cross(_frustumZ, _frustumX);
	_frustumX.Normalize();
	_frustumZ.Cross(_frustumX, _frustumY);

	_frustumNTL = nc + (_frustumY * _hNear) - (_frustumX * _wNear);
	_frustumNTR = nc + (_frustumY * _hNear) + (_frustumX * _wNear);
	_frustumNBL = nc - (_frustumY * _hNear) - (_frustumX * _wNear);
	_frustumNBR = nc - (_frustumY * _hNear) + (_frustumX * _wNear);

	_frustumFTL = fc + (_frustumY * _hFar) - (_frustumX * _wFar);
	_frustumFTR = fc + (_frustumY * _hFar) + (_frustumX * _wFar);
	_frustumFBL = fc - (_frustumY * _hFar) - (_frustumX * _wFar);
	_frustumFBR = fc - (_frustumY * _hFar) + (_frustumX * _wFar);

	_planes[0].Init(_frustumNTR, _frustumNTL, _frustumFTL);
	_planes[1].Init(_frustumNBL, _frustumNBR, _frustumFBR);
	_planes[2].Init(_frustumNTL, _frustumNBL, _frustumFBL);
	_planes[3].Init(_frustumNBR, _frustumNTR, _frustumFBR);
	_planes[4].Init(-_frustumZ, nc);
	_planes[5].Init(_frustumZ, fc);
}

bool Camera3d::PointInFrustum(const Vector3f &point)
{
	float pcz,pcx,pcy,h;

	// compute vector from camera position to p
	Vector3f v = point - _eye;

	// compute and test the Z coordinate
	pcz = v.Dot(-_frustumZ);
	if (pcz > _far || pcz < _near)
		return false;

	// compute and test the Y coordinate
	pcy = v.Dot(_frustumY);
	h = pcz * _frustumTanFOV;
	if (pcy > h || pcy < -h)
		return false;

	// compute and test the X coordinate
	pcx = v.Dot(_frustumX);
	h = h * _ratioAspect;
	if (pcx > h || pcx < -h)
		return false;
	return true;
}

bool Camera3d::BoxInFrustum(const Vector3f &center, float size)
{
	for(unsigned int i = 0; i < 6; ++i)
    {
        const Vector3f  &normal = _planes[i].Normal();
        Vector3f        p = center;
        //Vector3f        n = center;

        for (unsigned int j = 0; j < 3; ++j)
        {
            if (normal.Data[j] > 0)
            {
                p.Data[j] += size;
                //n.Data[j] -= size;
            }
            else
            {
                p.Data[j] -= size;
                //n.Data[j] += size;
            }
        }

		// is the positive vertex outside?
		if (_planes[i].Distance(p) < 0)
            return false;
        // is the negative vertex outside?
		//else if (_planes[i].Distance(n) < 0)
        //{
            // intersection
        //}
	}
	return true;
}
