
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

    File Created At:        04/06/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "../Object/BoundingBox.h"
#include "../Camera/Camera.h"
#include "Octree.h"

using namespace Nc;
using namespace Nc::Graphic;

BoundingBox      *Octree::_boundingBoxObject = NULL;

Octree::Octree(const Vector3f &center, unsigned int size)
    : Graphic::SubTree<Graph::OctreeNodePolitic>(ClassName()),
      _drawOutlines(false), _pattern(FrustumCulling)
{
    Init(center, size);
}

Octree::Octree(const Box3f &box)
    : Graphic::SubTree<Graph::OctreeNodePolitic>(ClassName()),
      _drawOutlines(false), _pattern(FrustumCulling)
{
    Vector3f s = box.Length();
    unsigned int size = std::max(s.Data[0], s.Data[1]);
    size = std::max(size, (unsigned int)s.Data[2]);

    Init(box.Center(), Math::NearestPowerOf2(size));
}

Octree::Octree(const Octree &oct)
    : Graphic::SubTree<Graph::OctreeNodePolitic>(oct),
      _drawOutlines(oct._drawOutlines), _pattern(oct._pattern)
{
    Data.clear();
    for (std::list<ISceneNode*>::const_iterator it = oct.Data.begin(); it != oct.Data.end(); ++it)
        Data.push_back((*it)->Clone());
}

Octree &Octree::operator = (const Octree &oct)
{
    _drawOutlines = oct._drawOutlines;
    _pattern = oct._pattern;
    for (std::list<ISceneNode*>::iterator it = Data.begin(); it != Data.end(); ++it)
        delete (*it);
    Data.clear();
    for (std::list<ISceneNode*>::const_iterator it = oct.Data.begin(); it != oct.Data.end(); ++it)
        Data.push_back((*it)->Clone());
    return *this;
}

const ISceneNode *Octree::SubTree(unsigned int i) const
{
    for (std::list<ISceneNode*>::const_iterator it = Data.begin(); it != Data.end(); ++it, --i)
        if (i == 0)
            return *it;
    return NULL;
}

void        Octree::DrawOutlines(bool state)
{
    _drawOutlines = state;
    for (unsigned int i = 0; i < 8; ++i)
    {
        if (_childs[i] != NULL)
            static_cast<Octree*>(_childs[i])->DrawOutlines(state);
    }
}

void        Octree::RenderChilds(SceneGraph *scene)
{
    if (_pattern == Nop || (scene->CurrentCamera() != NULL && scene->CurrentCamera()->BoxInFrustum(_center, _size)))
    {
        if (_drawOutlines)
        {
            if (_boundingBoxObject == NULL)
                _boundingBoxObject = new BoundingBox();
            _boundingBoxObject->Init(_center, _size);
            _boundingBoxObject->Render(scene);
        }

        for (std::list<ISceneNode*>::iterator it = Data.begin(); it != Data.end(); ++it)
            (*it)->Render(scene);
        for (unsigned int i = 0; i < 8; ++i)
        {
            if (_childs[i] != NULL)
                _childs[i]->Render(scene);
        }
    }
}

void        Octree::UpdateChilds(float elapsedTime)
{
    for (std::list<ISceneNode*>::iterator it = Data.begin(); it != Data.end(); ++it)
        (*it)->Update(elapsedTime);
    for (unsigned int i = 0; i < 8; ++i)
    {
        if (_childs[i] != NULL)
            _childs[i]->Update(elapsedTime);
    }
}

void    Octree::Insert(const std::list<std::pair<Math::Box3f,ISceneNode*> > &datasToInsert, unsigned int depthMax)
{
    // if we are at the maximum depth, insert the datas
    if (depthMax == 0)
    {
        for (std::list<std::pair<Math::Box3f,ISceneNode*> >::const_iterator it = datasToInsert.begin(); it != datasToInsert.end(); ++it)
            Data.push_back(it->second);
        return;
    }

    // for each childs, we build a list of data to insert
    std::list<std::pair<Math::Box3f,ISceneNode*> >      newDatasToInsert[8];
    unsigned int                                        childsSize = _size / 2;
    for (std::list<std::pair<Math::Box3f,ISceneNode*> >::const_iterator it = datasToInsert.begin(); it != datasToInsert.end(); ++it)
    {
        // get back the box of the data
        const Math::Box3f &box = it->first;

        // check the size
        // if the box size is superior to the size of the childs, we can insert the data into the current node
        // and if the box is crossing the box of the childs
        if (box.Length(0) > childsSize || box.Length(1) > childsSize || box.Length(2) > childsSize)
        {
            Data.push_back(it->second);
        }
        else
        {
            // identify the position of the data, to put it in the good list
            int noToInsert = -1;
            if (box.Min(0) < _center[0] && box.Min(1) < _center[1] && box.Min(2) < _center.Data[2])
                noToInsert = 0;
            if (box.Max(0) > _center[0] && box.Min(1) < _center[1] && box.Min(2) < _center.Data[2])
                noToInsert = (noToInsert == -1) ? 1 : -1;
            if (box.Min(0) < _center[0] && box.Max(1) > _center[1] && box.Min(2) < _center.Data[2])
                noToInsert = (noToInsert == -1) ? 2 : -1;
            if (box.Max(0) > _center[0] && box.Max(1) > _center[1] && box.Min(2) < _center.Data[2])
                noToInsert = (noToInsert == -1) ? 3 : -1;
            if (box.Min(0) < _center[0] && box.Min(1) < _center[1] && box.Min(2) > _center.Data[2])
                noToInsert = (noToInsert == -1) ? 4 : -1;
            if (box.Max(0) > _center[0] && box.Min(1) < _center[1] && box.Min(2) > _center.Data[2])
                noToInsert = (noToInsert == -1) ? 5 : -1;
            if (box.Min(0) < _center[0] && box.Max(1) > _center[1] && box.Min(2) > _center.Data[2])
                noToInsert = (noToInsert == -1) ? 6 : -1;
            if (box.Max(0) > _center[0] && box.Max(1) > _center[1] && box.Min(2) > _center.Data[2])
                noToInsert = (noToInsert == -1) ? 7 : -1;

            if (noToInsert != -1)
                newDatasToInsert[noToInsert].push_back(std::pair<Math::Box3f,ISceneNode*>(box, it->second));
            else
                Data.push_back(it->second);
        }
    }

    // next we have to insert recursively the datas in the childs
    // and if the childs doesn't exist, we create it
    for (unsigned int i = 0; i < 8; ++i)
    {
        if (!newDatasToInsert[i].empty())
        {
            if (_childs[i] == NULL)
                _childs[i] = new Octree(GetPositionFromIndex(i), childsSize);
            static_cast<Octree*>(_childs[i])->Insert(newDatasToInsert[i], depthMax-1);
        }
    }
}
