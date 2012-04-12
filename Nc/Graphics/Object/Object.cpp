
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
    : NodeType(),
      _useSceneMaterial(true),
      _material(NULL),
      _lastConfiguredMaterial(NULL)
{
}

Object::Object(const TMatrix &m)
    : NodeType(m),
      _useSceneMaterial(true),
      _material(NULL),
      _lastConfiguredMaterial(NULL)
{
}

Object::Object(const Box3f &box)
    : NodeType(box),
      _useSceneMaterial(true),
      _material(NULL),
      _lastConfiguredMaterial(NULL)
{
}

Object::Object(const Box3f &box, const TMatrix &m)
    : NodeType(box, m),
      _useSceneMaterial(true),
      _material(NULL),
      _lastConfiguredMaterial(NULL)
{
}

Object::Object(const Object &o)
    : NodeType(o),
      _drawables(o._drawables.size()),
      _useSceneMaterial(o._useSceneMaterial),
      _material(o._material),
      _lastConfiguredMaterial(NULL)
{
    for (unsigned int i = 0; i < _drawables.size(); ++i)
        _drawables[i] = new Drawable(*o._drawables[i]);
}

Object &Object::operator = (const Object &o)
{
    NodeType::operator = (o);
    _useSceneMaterial = o._useSceneMaterial;
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
    RenderBegin(scene);

    // rendering
    Draw(scene);

    // rendering childs
    RenderChildsBegin(scene);
    NodeType::RenderChilds(scene);
    RenderChildsEnd(scene);

    RenderEnd(scene);
}

void    Object::RenderBegin(SceneGraph *scene)
{
    scene->PushModelMatrix();
    TransformModelMatrixToRender(scene);
}

void    Object::RenderEnd(SceneGraph *scene)
{
    scene->PopModelMatrix();
}

void    Object::Draw(SceneGraph *scene)
{
    // get back the current material set in the scene
    IMaterial *m = (_useSceneMaterial) ? scene->Material() : NULL;

	// if there is no material in the scene, take the material of the object
	if (m == NULL)
        m = _material;

    if (m != NULL)
    {
        // setup the drawables if it's the fisrt render pass with this material
        if (m != _lastConfiguredMaterial)
            ConfigureDrawables(m);

        // rendering
        #ifdef _DEBUG_MATERIALS_RENDERING
            LOG_DEBUG << this << " " << *this << " Rendering with: " << *m << std::endl;
        #endif
        for (unsigned int i = 0; i < _drawables.size(); ++i)
            m->Render(scene, *_drawables[i]);
    }
}

void    Object::TransformModelMatrixToRender(SceneGraph *scene)
{
    scene->ModelMatrix().AddTransformation(Matrix);
}
