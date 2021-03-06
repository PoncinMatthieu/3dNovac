
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

#include "FactoryDefaultMaterials.h"

using namespace Nc;
using namespace Nc::Graphic;

FactoryDefaultMaterials::~FactoryDefaultMaterials()
{
    for (PriorityMaterialList::iterator it = _materials.begin(); it != _materials.end(); ++it)
    {
        if (it->second != NULL)
            delete it->second;
    }
}

IDefaultMaterial    *FactoryDefaultMaterials::GetBestMaterial(const GL::DrawableArray &drawables)
{
    CALLSTACK_INFO();
    if (drawables.size() == 0)
        return NULL;

    GL::VertexDescriptor    *descriptor = NULL;

    // get back the max priority descriptor
    for (unsigned int i = 0; i < drawables.size(); ++i)
    {
        if (drawables[i] != NULL)
        {
            GL::VertexDescriptor *desc = &drawables[i]->Geometry->Descriptor();
            if (descriptor == NULL || desc->Priority() > descriptor->Priority())
                descriptor = desc;
        }
    }
    return GetBestMaterial(descriptor);
}

IDefaultMaterial    *FactoryDefaultMaterials::GetBestMaterial(const GL::Drawable *drawable)
{
    CALLSTACK_INFO();
    return GetBestMaterial(&drawable->Geometry->Descriptor());
}

IDefaultMaterial        *FactoryDefaultMaterials::GetBestMaterial(GL::VertexDescriptor *descriptor)
{
    CALLSTACK_INFO();
    IDefaultMaterial        *result = NULL;

    // get back the best Material
    if (descriptor != NULL)
    {
        PriorityMaterialList::iterator it = _materials.begin();
        for (; it != _materials.end() && it->first > descriptor->Priority(); ++it);
        for (; result == NULL && it != _materials.end(); ++it)
        {
            if (it->second->IsCompatible(*descriptor))
                result = it->second;
        }
    }
    if (result == NULL)
    {
        LOG_ERROR << "Warning: Can't find any available default material type for the given VertexType with this descriptor: ";
        if (descriptor != NULL)
            LOG_ERROR << std::endl << *descriptor << std::endl;
        else
            LOG_ERROR << "NULL" << std::endl;
    }
    else if (result->Initialized() == false)
        result->Initialize();
    return result;
}

