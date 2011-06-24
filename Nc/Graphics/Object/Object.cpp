
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

using namespace Nc;
using namespace Nc::Graphic;

Object::Object()
    : NodeType(ClassName()),
      _displayStateBox(false),
      _material(NULL)
{
}

Object::Object(const char *className)
    : NodeType(className),
      _displayStateBox(false),
      _material(NULL)
{
}

Object::Object(const TMatrix &m)
    : NodeType(ClassName(), m),
      _displayStateBox(false),
      _material(NULL)
{
}

Object::Object(const Box3f &box)
    : NodeType(ClassName()),
      _displayStateBox(false),
      _box(box), _material(NULL)
{
}

Object::Object(const Box3f &box, const TMatrix &m)
    : NodeType(ClassName(), m),
      _displayStateBox(false),
      _box(box), _material(NULL)
{
}

Object::Object(const Object &o)
    : NodeType(o),
      _displayStateBox(o._displayStateBox),
      _drawables(o._drawables.size()),
      _box(o._box), _material(o._material)
{
    for (unsigned int i = 0; i < _drawables.size(); ++i)
        _drawables[i] = new Drawable(*o._drawables[i]);
}

Object &Object::operator = (const Object &o)
{
    NodeType::operator = (o);
    _displayStateBox = o._displayStateBox;
    _box = o._box;
    _material = o._material;

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
        GL::Shader::ResetCurrentProgram();
    }
    _material = newMaterial;
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
    if (_material != NULL)
    {
        for (unsigned int i = 0; i < _drawables.size(); ++i)
        {
            if (!_material->Configure(*_drawables[i]))    // config failed ?
            {
                _material = NULL;
                throw Utils::Exception("Graphic::Object", "The Configuration of the drawable no " + Utils::Convert::ToString(i) + " failed");
            }
            GL::Shader::ResetCurrentProgram();
        }
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

        scene->PopModelMatrix();
    }
}

void    Object::Draw(SceneGraph *scene)
{
    if (_material != NULL)
    {
//LOG << this << " Render with: " << *_material << std::endl;
        for (unsigned int i = 0; i < _drawables.size(); ++i)
            _material->Render(scene, *_drawables[i]);
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

struct DisplayStateFonctor
{
    DisplayStateFonctor()   : result(true)  {}

    bool operator () (const ISceneNode *node)
    {
        const Object *n = node->AsWithoutThrow<const Object>();
        if (n != NULL)
            result = n->DisplayState();
        return result;
    }

    bool    result;
};

bool    Object::DisplayStateRecursive() const
{
    DisplayStateFonctor f;
    ForEachParents<false>(f);
    return f.result;
}
