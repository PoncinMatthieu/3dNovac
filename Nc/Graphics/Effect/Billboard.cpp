
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

    File Created At:        15/11/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "Billboard.h"
#include "../Scene/SceneGraph.h"
#include "../Scene/Visitors.h"
#include "../Camera/Camera3d.h"

using namespace Nc;
using namespace Nc::Graphic;

Billboard::Billboard(Pattern p, Entity *target)
    : Effect(), _pattern(p), _targetEntity(target)
{
}

void    Billboard::RotateChilds()
{
    for (Entity::ContainerType::iterator it = Childs().begin(); it != Childs().end(); ++it)
    {
        Entity *e = dynamic_cast<Entity*>(*it);
        if (e != NULL)
        {
            e->Matrix.AddRotation(Vector3f(1,0,0), 90, false);
            e->Matrix.AddRotation(Vector3f(0,0,1), 180, false);
        }
    }
}

void    Billboard::Render(SceneGraph *scene)
{
    scene->PushModelMatrix();
    scene->ModelMatrix().AddTransformation(Matrix);
    if (_activated)
        OrientBillboard(scene);
    RenderChilds(scene);
    scene->PopModelMatrix();
}

void    Billboard::GetTargetPosUp(SceneGraph *scene, Vector3f &pos, Vector3f &up)
{
    if (_targetEntity == NULL)
    {
        if (scene->CurrentCamera() == NULL)
            throw Utils::Exception("Billboard", "The current camera is null ! Please setup a camera or another target.");
        Camera3d *cam = static_cast<Camera3d*>(scene->CurrentCamera());
        pos = cam->Eye();
        up = cam->Up();
    }
    else
    {
        pos.Init(0,0,0);
        TMatrix modelMatrix;
        Visitor::GetRecursiveMatrix visitor(modelMatrix);
        visitor(*_targetEntity);
        modelMatrix.Transform(pos);
        up.Init(0,0,1);
    }
}

void    Billboard::GetTargetLookAtCenter(SceneGraph *scene, const Vector3f &pos, Vector3f &lookAt, Vector3f &center)
{
    if (_targetEntity == NULL)
    {
        if (scene->CurrentCamera() == NULL)
            throw Utils::Exception("Billboard", "The current camera is null ! Please setup a camera or another target.");
        Camera3d *cam = static_cast<Camera3d*>(scene->CurrentCamera());
        center = cam->Center();
        lookAt = center - cam->Eye();
        lookAt[2] = 0;
        lookAt.Normalize();
    }
    else
        lookAt.Init(0,1,0);
}

void    Billboard::OrientBillboard(SceneGraph *scene)
{
    // Get the billboard starting attributes
    Vector3f pos;
    Vector3f lookAt(0, 1, 0);
    TMatrix m = scene->ModelMatrix();
    m.Transform(pos);

    // Get the target position
    Vector3f targetPos, targetUp;
    GetTargetPosUp(scene, targetPos, targetUp);

    // get the obj to target vector
    Vector3f objToTarget = targetPos - pos;
    objToTarget[2] = 0;
    objToTarget.Normalize();


    // Firstly, rotate the billboard to have the good orientation with the target
    Vector3f    axis;
    float       alpha = 0;

    if (_pattern == TrueBillboard && !lookAt.Equal(objToTarget, 0.0001))
    {
        // compute the axis and alpha rotation
        lookAt.Cross(objToTarget, axis);
        axis.Normalize();
        alpha = lookAt.Angle(objToTarget);
    }
    else
    {
        // get the look at target vector
        Vector3f targetLookAt, center;
        GetTargetLookAtCenter(scene, targetPos, targetLookAt, center);

        // compute the axis and alpha rotation
        axis.Init(0,0, (targetPos[0] < center[0] || (targetPos[0] < center[0] && targetPos[1] < center[1])) ? -1 : 1);
        alpha = (!lookAt.Equal(targetLookAt, 0.0001)) ? M_PI + lookAt.Angle(targetLookAt) : M_PI;
    }
    if (targetUp[2] < 0)
        alpha -= M_PI;

    if (alpha != 0)
    {
        TMatrix m;
        m.Rotation(axis, alpha);
        scene->ModelMatrix() *= m;
    }



    // Secondly, rotate the billboard to have the same up vectors
    Vector3f objToTarget2 = targetPos - pos;
    objToTarget2.Normalize();

    bool equal = objToTarget.Equal(objToTarget2, 0.0001);
    alpha = 0;
    if (!equal || targetUp[2] < 0)
    {
        if (!equal)
        {
            axis.Init((objToTarget2[2] < 0) ? -1 : 1,0,0);
            alpha = objToTarget.Angle(objToTarget2);
        }

        if (equal)
        {
            axis.Init((objToTarget2[2] < 0) ? -1 : 1,0,0);
            alpha -= M_PI;
        }
        else if (targetUp[2] < 0)
        {
            axis = -axis;
            alpha -= M_PI;
        }

        if (alpha != 0)
        {
            TMatrix m;
            m.Rotation(axis, alpha);
            scene->ModelMatrix() *= m;
        }
    }
}
