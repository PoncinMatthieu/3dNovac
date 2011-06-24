
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

using namespace Nc;
using namespace Nc::Graphic;
/*
std::string ISceneNode::NodeTypeToString() const
{
    switch (_nodeType)
    {
        case ENTITY:        return "Entity";
        case SCENE:         return "SceneGraph";
        case OBJECT:        return "IObject";
        case EFFECT:        return "Effect";
        case PARTITION:     return "Partition";
        default:            return "Unknown";
    }
}
*/
/*
namespace Nc
{
    namespace Graphic
    {
        template<>
        Graphic::Object *ISceneNode::GetNode<>()
        {
            if (_nodeType == OBJECT)
                return static_cast<Graphic::Object*>(this);
            throw Utils::Exception("ISceneNode", "Cannot convert the scene node into a " + std::string(typeid(Graphic::Object).name()) +
                                   ". The type of the node is " + NodeTypeToString());
        }

        template<>
        SceneGraph *ISceneNode::GetNode<>()
        {
            if (_nodeType == SCENE)
                return static_cast<SceneGraph*>(this);
            throw Utils::Exception("ISceneNode", "Cannot convert the scene node into a " + std::string(typeid(SceneGraph).name()) +
                                   ". The type of the node is " + NodeTypeToString());
        }

        template<>
        Effect *ISceneNode::GetNode<>()
        {
            if (_nodeType == EFFECT)
                return static_cast<Effect*>(this);
            throw Utils::Exception("ISceneNode", "Cannot convert the scene node into a " + std::string(typeid(Effect).name()) +
                                   ". The type of the node is " + NodeTypeToString());
        }
    }
}
*/

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
