
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

    File Created At:        30/05/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "Object.h"
#include "../Material/FactoryDefaultMaterials.h"
#include "BoundingBox.h"

using namespace Nc;
using namespace Nc::Graphic;

Object::Object()
    : NodeType(ClassName()),
      _displayBox(false),
      _material(NULL),
      _lastConfiguredMaterial(NULL)
{
}

Object::Object(const char *className)
    : NodeType(className),
      _displayBox(false),
      _material(NULL),
      _lastConfiguredMaterial(NULL)
{
}

Object::Object(const TMatrix &m)
    : NodeType(ClassName(), m),
      _displayBox(false),
      _material(NULL),
      _lastConfiguredMaterial(NULL)
{
}

Object::Object(const Box3f &box)
    : NodeType(ClassName()),
      _displayBox(false),
      _box(box), _material(NULL),
      _lastConfiguredMaterial(NULL)
{
}

Object::Object(const Box3f &box, const TMatrix &m)
    : NodeType(ClassName(), m),
      _displayBox(false),
      _box(box), _material(NULL),
      _lastConfiguredMaterial(NULL)
{
}

Object::Object(const Object &o)
    : NodeType(o),
      _displayBox(o._displayBox),
      _drawables(o._drawables.size()),
      _box(o._box), _material(o._material),
      _lastConfiguredMaterial(NULL)
{
    for (unsigned int i = 0; i < _drawables.size(); ++i)
        _drawables[i] = new Drawable(*o._drawables[i]);
}

Object &Object::operator = (const Object &o)
{
    NodeType::operator = (o);
    _displayBox = o._displayBox;
    _box = o._box;
    _material = o._material;
    _lastConfiguredMaterial = NULL;

    for (unsigned int i = 0; i < _drawables.size(); ++i)
        delete _drawables[i];
    _drawables.resize(o._drawables.size());
    for (unsigned int i = 0; i < _drawables.size(); ++i)
        _drawables[i] = new Drawable(*o._drawables[i]);
    return *this;
}


Object::~Object()
{
    for (unsigned int i = 0; i < _drawables.size(); ++i)
        if (_drawables[i] != NULL)
            delete _drawables[i];
}

void    Object::SetRecursiveMaterial(IMaterial *newMaterial)
{
    SetMaterial(newMaterial);
    SetMaterialFonctor f(newMaterial);
    ForEachChilds<false>(f);
}

bool    Object::SetMaterial(IMaterial *newMaterial)
{
    // configure all drawable with the new material
    for (unsigned int i = 0; i < _drawables.size(); ++i)
    {
        if (!newMaterial->Configure(*_drawables[i]))    // config failed ?
        {
            // so we have to reconfigure the drawables with the current drawable before returning false
            if (_material != NULL)
            {
                for (unsigned int j = i; j < 0; ++j)
                    _material->Configure(*_drawables[i]);
            }
            return false;
        }
    }
    _material = newMaterial;
    _lastConfiguredMaterial = _material;
    return true;
}

void    Object::ChooseDefaultMaterial()
{
    if (_drawables.size() == 0)
        return;

    IMaterial *newMaterial = FactoryDefaultMaterials::Instance().GetBestMaterial(_drawables);

    if (newMaterial != NULL)
        _material = newMaterial;
    else
        throw Utils::Exception("Graphic::Object", "Can't find a suitable DefaultMaterial.");
    ReconfigureDrawables();
}

void    Object::ReconfigureDrawables()
{
    ConfigureDrawables(_material);
}

void    Object::ConfigureDrawables(IMaterial *material)
{
    if (material != NULL)
    {
        for (unsigned int i = 0; i < _drawables.size(); ++i)
        {
            if (!material->Configure(*_drawables[i]))    // config failed ?
            {
                material = NULL;
                throw Utils::Exception("Graphic::Object", "The Configuration of the drawable no " + Utils::Convert::ToString(i) + " failed");
            }
        }
        _lastConfiguredMaterial = material;
    }
}

void    Object::Render(SceneGraph *scene)
{
    if (_enabled)
    {
        scene->PushModelMatrix();
        TransformModelMatrixToRender(scene);

        // rendering
        Draw(scene);

        // rendering childs
        NodeType::RenderChilds(scene);

        // rendering the box
        if (_displayBox)
            BoundingBox::Draw(_box, scene);

        scene->PopModelMatrix();
    }
}

void    Object::Draw(SceneGraph *scene)
{
    // get back the current material
    IMaterial *m = scene->Material();
    if (m == NULL)
        m = _material;

    if (m != NULL)
    {
        // setup the drawables if it's the fisrt render pass with this material
        if (m != _lastConfiguredMaterial)
            ConfigureDrawables(m);

        //LOG << this << " Render with: " << *m << std::endl;

        // rendering
        for (unsigned int i = 0; i < _drawables.size(); ++i)
            m->Render(scene, *_drawables[i]);
    }
}

void    Object::GetReelBox(Box3f &box) const
{
    box = _box;
    box.Transform(Matrix);
}

void    Object::HeightScale(float height)
{
    Box3f b = _box; // create a temporary box to use the current matrix
    b.Transform(Matrix);
    // scale the matrix to obtain the good value
    Matrix.AddScale(height / b.Length(2));
}

void    Object::CenterBase(const Vector3f &centerBase)
{
    Box3f b = _box;
    b.Transform(Matrix);

    Vector3f center = b.Center();
    center.Data[2] = b.Min(2);
    Matrix.AddTranslation(centerBase - center);
}

struct MatrixFonctor
{
    MatrixFonctor(TMatrix &m) : matrix(m) {}

    bool operator () (ISceneNode *node)
    {
        Object *n = node->AsWithoutThrow<Object>();
        if (n != NULL)
            matrix.AddTransformation(n->Matrix);
        return true;
    }

    TMatrix  &matrix;
};

void    Object::GetRecursiveMatrix(TMatrix &m)
{
    m.Init();
    MatrixFonctor f(m);
    ForEachParents<true>(f);
}

void    Object::TransformModelMatrixToRender(SceneGraph *scene)
{
    scene->ModelMatrix().AddTransformation(Matrix);
}
