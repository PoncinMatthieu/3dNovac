
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

#include "Object3d.h"
#include "Light.h"
#include "BoundingBox.h"
#include "Nc/Core/Utils/Debug/OverloadAlloc.h"

using namespace std;
using namespace Nc;
using namespace Nc::Graphic;

Object3d::Object3d(bool createBox)
    : _parent(NULL)
{
    _box = (createBox) ? new BoundingBox() : NULL;
}
/*
Object3d::Object3d(Mesh *mesh)
    : _parent(NULL)
{
    _listMesh.push_back(mesh);
    _box = mesh->Box();
}
Object3d::Object3d(list<Mesh*> &listMesh, const Box3f &box, const TMatrix &mat) : Object(mat)
{
    _listMesh = listMesh;
    _box = box;
}
*/

Object3d::Object3d(const Box3f &box, const TMatrix &mat)
    : Object(mat), _parent(NULL), _box(new BoundingBox(box))
{
}

Object3d::~Object3d()
{
    Free();
}

void Object3d::Free()
{
    Utils::DeleteContainer(_listChild);
    if (_box)
        delete _box;
}

Object3d::Object3d(const Object3d &O) : Object(O)
{
    Object::operator = (O);
    for (ListPObject3D::const_iterator it = O._listChild.begin(); it != O._listChild.end(); it++)
    {
        Object3d *newObj = (*it)->Clone();
        newObj->SetParent(this);
        _listChild.push_back(newObj);
    }
    _parent = O._parent;
    _box = (O._box != NULL) ? new BoundingBox(*O._box) : NULL;
}

Object3d& Object3d::operator = (const Object3d& O)
{
    Free();
    Object::operator = (O);
    for (ListPObject3D::const_iterator it = O._listChild.begin(); it != O._listChild.end(); it++)
    {
        Object3d *newObj = (*it)->Clone();
        newObj->SetParent(this);
        _listChild.push_back(newObj);
    }
    _parent = O._parent;
    _box = (O._box != NULL) ? new BoundingBox(*O._box) : NULL;
    return *this;
}

void Object3d::AddChild(Object3d *o)
{
    _listChild.push_back(o);
    if (_box != NULL && o->_box != NULL)
        _box->SetBox(_box->GetBox() + o->_box->GetBox());
}

const Box3f &Object3d::Box() const
{
    if (_box != NULL)
        return _box->GetBox();
    else
        return Box3f::EmptyBox;
}

void Object3d::Render(ISceneGraph *scene)
{
    scene->PushModelMatrix();

    TransformModelMatrixToRender(scene);

    // rendering de l'objet
    Draw(scene);

    // render the childs
    for (ListPObject3D::iterator it = _listChild.begin(); it != _listChild.end(); it++)
        (*it)->Render(scene);
    scene->PopModelMatrix();
}

void Object3d::Draw(ISceneGraph *scene)
{
    // rendu de la box
    if (_displayStateBox && _box != NULL)
        _box->Render(scene);
}
/*
void Object3d::Transform(const TMatrix& M)
{
    for (ListPObject3D::iterator it = _listChild.begin(); it != _listChild.end(); it++)
        (*it)->Transform(M);
    _box->Transform(M);
}

void Object3d::UpdateBoundingBox()
{
    Box3f b;
    for (ListPObject3D::iterator it = _listChild.begin(); it != _listChild.end(); it++)
        b += (*it)->Box();
    b->SetBox(b);
}
*/
void Object3d::HeightScale(float height)
{
    if (_box != NULL)
    {
        float k;
        k = height / _box->GetBox().Length(2);
        Matrix.AddScale(k);
    }
}
/*
void Object3d::PosBase(Vector3f &centerBase)
{
    Box3f b = _box->GetBox();
    b.Transform(Matrix);

    Vector3f center = _box.Center();
    center.Data[2] = _box.Min().Data[2];
    centerBase -= center;
    Matrix.AddTranslation(centerBase);
}
*/
void Object3d::ComputeReelMatrix(TMatrix &m)
{
    if (_parent != NULL)
        _parent->ComputeReelMatrix(m);
    m.AddTransformation(Matrix);
}
