
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

    File Created At:        29/05/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "SceneNode.h"
#include "SceneGraph.h"
#include "../Object/Object.h"
#include "../Effect/Effect.h"
#include "Visitors.h"

using namespace Nc;
using namespace Nc::Graphic;

bool    ISceneNode::EnabledRecursif() const
{
    Visitor::IsEnableRecursive f;
    f(*this);
    return f.result;
}

Entity::Entity()
    : NodePolitic(ClassName())
{
}

Entity::Entity(const char *className)
    : NodePolitic(className)
{
}

Entity::Entity(const char *className, const TMatrix &m)
    : NodePolitic(className), Matrix(m)
{}

Entity::Entity(const char *className, ISceneNode *data)
    : NodePolitic(className, data)
{
}

Entity::Entity(const Entity &n)
    : NodePolitic(n)
{
    Matrix = n.Matrix;
    if (NodePolitic::Data != NULL)
        NodePolitic::Data = n.Data->Clone();
}

Entity &Entity::operator = (const Entity &n)
{
    NodePolitic::operator = (n);
    Matrix = n.Matrix;
    if (NodePolitic::Data != NULL)
        NodePolitic::Data = n.Data->Clone();
    return *this;
}

Entity::~Entity()
{
    if (Data != NULL)
        delete Data;
}

void    Entity::RenderChilds(SceneGraph *scene)
{
    try
    {
        if (NodeType::Data != NULL)
            NodeType::Data->Render(scene);
        for (ContainerType::iterator it = _childs.begin(); it != _childs.end(); ++it)
            (*it)->Render(scene);
    }
    catch (const std::exception &e)
    {
        LOG_ERROR << "Error in the render pass of the node : " << e.what() << std::endl;
    }
}

void    Entity::UpdateChilds(float elapsedTime)
{
    try
    {
        if (NodeType::Data != NULL)
            NodeType::Data->Update(elapsedTime);
        for (ContainerType::iterator it = _childs.begin(); it != _childs.end(); ++it)
            (*it)->Update(elapsedTime);
    }
    catch (const std::exception &e)
    {
        LOG_ERROR << "Error in the update pass of the node : " << e.what() << std::endl;
    }
}

void    Entity::Move(ISceneNode *node, int at, ISceneNode *oldParent, int oldAt)
{

    Entity *oldParentEntity = NULL;
    if (oldParent != NULL)
    {
        oldParentEntity = dynamic_cast<Entity*>(oldParent);
        if (oldParentEntity == NULL)
            throw Utils::Exception("Entity::Move", "Can't move the node from an incompatible parent type.");
    }

    // insert
    Lock();
    Entity *entity = dynamic_cast<Entity*>(node);
    if (entity != NULL)
    {
        if (at < 0)
            AddChild(entity);
        else
            InsertChild(entity, at);
    }
    else
    {
        Unlock();
        throw Utils::Exception("Entity::Move", "Can't move the node. The types are incompatible.");
    }
    Unlock();

    // remove
    if (oldParentEntity != NULL)
    {
        oldParent->Lock();
        oldParentEntity->RemoveChild((oldAt > at && oldParentEntity == this) ? (oldAt + 1) : oldAt);
        oldParent->Unlock();
    }
}

void    Entity::Render(SceneGraph *scene)
{
    if (_enabled)
    {
        _mutex.Lock();
        RenderChilds(scene);
        _mutex.Unlock();
    }
}

void    Entity::Update(float elapsedTime)
{
    if (_enabled)
        UpdateChilds(elapsedTime);
}
