
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

#include "BasicMeshCreator.h"
#include "Text.h"
#include "../Effect/Billboard.h"
#include "../Effect/CameraEffect.h"
#include "../Scene/RenderingController.h"
#include "../Camera/Camera3d.h"
#include "Nc/Core/Utils/Debug/OverloadAlloc.h"

using namespace Nc;
using namespace Nc::Graphic;

Object *BasicMeshCreator::Axis(float scale, bool withLegend, const Vector3f &center)
{
    return Axis(Vector3f(scale, scale, scale), withLegend, center);
}

Object *BasicMeshCreator::Axis(const Vector3f &scale, bool withLegend, const Vector3f &center)
{
	Array<Core::DefaultVertexType::Colored, 4>  vertices;
    Array<unsigned int, 3*2>                    indices;

    vertices[0].Fill(center.data[0], center.data[1], center.data[2], Color(1, 1, 1));
    vertices[1].Fill(center.data[0] + scale.data[0], center.data[1], center.data[2], Color(1, 0, 0));
    vertices[2].Fill(center.data[0], center.data[1] + scale.data[1], center.data[2], Color(0, 0, 1));
    vertices[3].Fill(center.data[0], center.data[1], center.data[2] + scale.data[2], Color(0, 1, 0));

    indices[0] = 0; indices[1] = 1;
    indices[2] = 0; indices[3] = 2;
    indices[4] = 0; indices[5] = 3;

    // creation du mesh
    Object *obj = new Object(Box3f(center, center + Vector3f(scale.data[0], scale.data[1], scale.data[2])));
	obj->Drawables().push_back(new Core::Drawable(vertices, GL::Enum::DataBuffer::StaticDraw, indices, 2, GL::Enum::Lines));
    obj->ChooseDefaultMaterial();
    obj->UseSceneMaterial(false);

    // add the 3 labels with billbord effect
    if (withLegend)
    {
        AddLabel("X", 0.2, Color(1, 0, 0), "arial", true, obj, Vector3f(center[0] + scale[0] + 0.2, center[1], center[2]));
        AddLabel("Y", 0.2, Color(0, 0, 1), "arial", true, obj, Vector3f(center[0], center[1] + scale[1] + 0.2, center[2]));
        AddLabel("Z", 0.2, Color(0, 1, 0), "arial", true, obj, Vector3f(center[0], center[1], center[2] + scale[2] + 0.2));
    }
	return obj;
}

Entity *BasicMeshCreator::Axis(float scale, bool withLegend, bool inCorner)
{
    return Axis(Vector3f(scale, scale, scale), withLegend, inCorner);
}


class AxisRenderingController : public RenderingController<AxisRenderingController>
{
    public:
        template<typename ToVisitList>
        AxisRenderingController(const ToVisitList &toVisitList)
            : RenderingController<AxisRenderingController>(toVisitList)
        {
            _center = Vector3f(0,0,0);
            _rasterMode.SetDepthTest(false);
        }

        void VisitNode(const Entity &n)
        {
            if (_currentStade == IRenderingController::Begin)
            {
                _scene->PushViewMatrix();
                Camera3d *cam = static_cast<Camera3d*>(_scene->CurrentCamera());
                _oldEye = cam->Eye();
                _oldCenter = cam->Center();

                Vector3f eye = _oldEye - _oldCenter;
                eye.Normalize();
                eye.Scale(3);
                _scene->ViewMatrix().SetLookAt(eye, _center, cam->Up());

                cam->Eye(eye);
                cam->Center(_center);
                _rasterMode.Enable();
            }
            else
            {
                Camera3d *cam = static_cast<Camera3d*>(_scene->CurrentCamera());
                cam->Eye(_oldEye);
                cam->Center(_oldCenter);

                _rasterMode.Disable();
                _scene->PopViewMatrix();
            }
        }

    private:
        Vector3f            _center;
        GL::RasterMode      _rasterMode;

        Vector3f            _oldEye;
        Vector3f            _oldCenter;
};


Entity *BasicMeshCreator::Axis(const Vector3f &scale, bool withLegend, bool inCorner)
{
    // create the axis
    Object *obj = Axis(scale, withLegend, Vector3f(0,0,0));

    if (!inCorner)
        return obj;

    // create the viewport effect and configure it
    CameraEffect *effect = new CameraEffect();
    effect->Viewport(0, 0, 150, 150);
    effect->Projection(1, 0.1, 5, 60);

    effect->SetRenderingController(new AxisRenderingController(Utils::Metaprog::Seq<Graphic::Entity>::Type()));

    effect->AddChild(obj);
    return effect;
}

Text  *BasicMeshCreator::AddLabel(const std::string &text, float caracterSize, const Color &color, const std::string &fontName, bool centerText, Entity *entity, const Vector3f &position)
{
    // create the billboard
    Billboard *billboard = new Billboard();
    billboard->Matrix.Translation(position);

    // create the label
    Text *label = new Text(text, caracterSize, color, fontName);
    if (centerText)
    {
        Vector2f size = label->Size();
        label->Matrix.Translation(-size[0] / 2, -size[1] / 2, 0);
    }

    billboard->AddChild(label);
    billboard->RotateChilds();
    if (entity != NULL)
        entity->AddChild(billboard);
    return label;
}
