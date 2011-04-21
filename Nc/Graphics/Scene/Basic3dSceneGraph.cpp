
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

#include "Basic3dSceneGraph.h"
#include "../Object/3D/Light.h"
#include "../Effect/Effect.h"

using namespace Nc;
using namespace Nc::Graphic;

Basic3dSceneGraph::Basic3dSceneGraph(Camera3d *camera)
    : I3dSceneGraph(camera), _colorAmbiant(0.2, 0.2, 0.2, 0)
{}

Basic3dSceneGraph::~Basic3dSceneGraph()
{
}

void Basic3dSceneGraph::Clear(bool del)
{
    if (del)
        delete _camera;
    _camera = NULL;
    if (del)
    {
        Utils::DeleteContainer(_listObject);
        Utils::DeleteContainer(_listLight);
        Utils::DeleteContainer(_listEffect);
    }
    else
    {
        _listObject.clear();
        _listLight.clear();
        _listEffect.clear();
    }
}

void Basic3dSceneGraph::Update(float runningTime)
{
    if (_camera)
        _camera->Update(runningTime);
    for(ListPEffect::iterator it = _listEffect.begin(); it != _listEffect.end();)
    {
        if ((*it)->Alive())
        {
            (*it)->Update(runningTime);
            ++it;
        }
        else
            it = _listEffect.erase(it);
    }
}

void Basic3dSceneGraph::Render()
{
    if (!ModelMatrix().IsIdentity())
        ModelMatrix().SetIdentity();

    _mutex.Lock(); // lock la mutex
    try
    {
        // Fixe la camera :
        _camera->Fix();
        _camera->Render(this);

        // rend les objets un par un
        if (!_listObject.empty() || !_listLight.empty() || !_listEffect.empty())
        {
            for(ListPObject::iterator it = _listObject.begin(); it != _listObject.end(); it++)
                if ((*it)->DisplayState())
                    (*it)->Render(this);
            for(ListPLight::iterator it = _listLight.begin(); it != _listLight.end(); it++)
                if ((*it)->DisplayState())
                    (*it)->Render(this);
            for(ListPEffect::iterator it = _listEffect.begin(); it != _listEffect.end(); it++)
                if ((*it)->DisplayState())
                    (*it)->Render(this);
        }
    }
    catch (const std::exception &e)
    {
        LOG_ERROR << "Error in the render pass: " << e.what() << std::endl;
    }
    _mutex.Unlock(); // unlock la mutex
}

void Basic3dSceneGraph::AddObject(Graphic::Object* objectToAdd)
{
    if (objectToAdd != NULL)
    {
        _mutex.Lock();

        Effect *effect = dynamic_cast<Effect*>(objectToAdd);
        if (effect != NULL)
            _listEffect.push_back(effect);
        else
        {
            Graphic::Light *light = dynamic_cast<Graphic::Light*>(objectToAdd);
            if (light != NULL)
                _listLight.push_back(light);
            else
                _listObject.push_back(objectToAdd);
        }

        _mutex.Unlock();
    }
}

void Basic3dSceneGraph::DeleteObject(Graphic::Object *objectToDelete, bool del)
{
    _mutex.Lock();

    Effect *effect = dynamic_cast<Effect*>(objectToDelete);
    if (effect != NULL)
        _listEffect.remove(effect);
    else
    {
        Graphic::Light *light = dynamic_cast<Graphic::Light*>(objectToDelete);
        if (light != NULL)
            _listLight.remove(light);
        else
            _listObject.remove(objectToDelete);
    }

    if (del)
        delete objectToDelete;

    _mutex.Unlock();
}
