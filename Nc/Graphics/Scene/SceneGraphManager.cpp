
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

#include "SceneGraphManager.h"
#include "../Window/Context/GLContext.h"

using namespace Nc::Graphic;

SceneGraphManager::SceneGraphManager() : _3dSceneGraph(NULL), _2dSceneGraph(NULL)
{
    _clearMask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
}

SceneGraphManager::~SceneGraphManager()
{
}

void SceneGraphManager::InitGL(bool multisampling)
{
// defini les valeur des tompon a vider
    glClearColor(_clearColor.R, _clearColor.G, _clearColor.B, _clearColor.A);

// enable l'antialiasing
    if (multisampling)
        glEnable(GL_MULTISAMPLE);

// enable le Z-buffer
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);

// initialistation des extensions
    if(!EXT.Init())
        throw Utils::Exception("SceneGraphManager", "Can't initialize gl extensions");
}

void SceneGraphManager::Render(GLContext *context)
{
// vide les tampons
    glClear(_clearMask);

// render les 2 scenes
    if (_3dSceneGraph)
        _3dSceneGraph->Render();
    if (_2dSceneGraph)
        _2dSceneGraph->Render();

// refresh et swap les buffers :
    glFlush();
    context->SwapBuffers();
}
