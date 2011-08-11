
/*-----------------------------------------------------------------------------

	3dNovac Editor
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

    File Created At:        20/03/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------

	Description : Manage the editor (connect all parts Interface+Engines+Editor)

-----------------------------------------------------------------------------*/

#ifndef EDITOR_EDITORMANAGER_H_
#define EDITOR_EDITORMANAGER_H_

#include "../Engines/QManager.h"
#include "../Interface/MainWindow.h"
#include "../Engines/EditorEngine.h"
#include <Nc/Contrib/GameEngine.h>
#include "Project.h"

namespace Nc
{
  namespace Editor
  {
    class EditorManager : public QObject
    {
      Q_OBJECT;

    private:
        typedef std::pair<std::string, System::PluginLoader<Contrib::GameEngine> >  PluginLoader;

    public:
      EditorManager(Interface::MainWindow *mainWin);
      virtual ~EditorManager();

      void    Start();

      // accessors
      QGraphicEngine		*Graphic()          {return _graphic;}
      EditorEngine		*Editor()           {return _editor;}
      Project			*CurrentProject()   {return _currentProject;}
      Interface::MainWindow     *GUI()              {return _GUI;}
      QManager			*ThreadManager()    {return _threadManager;}

      // operation sur la scene
      void      LoadNode(const Utils::FileName &path)                                   {_editor->PushEvent(EditorEngine::IndexLoadNode, path);}

      void      DestroyCurrentSample();

    public slots:
      void      SlotLoadSample(const std::string &filename);
      void      SlotRemoveNode(Nc::Graphic::ISceneNode *obj)                                    {_editor->PushEvent(EditorEngine::IndexRemoveNode, obj);}
      void      SlotChangeCurrentScene(Nc::Graphic::SceneGraph *scene)                          {_editor->PushEvent(EditorEngine::IndexSelectScene, scene);}
      void      SlotSelectNode(Nc::Graphic::ISceneNode *node)                                   {_editor->PushEvent(EditorEngine::IndexSelectNode, node);}
      void      SlotChangeRasterMode(Nc::Graphic::RasterEffect::Pattern mode)                   {_editor->PushEvent(EditorEngine::IndexChangeRasterMode, mode);}
      void      SlotMoveNode(Nc::Graphic::ISceneNode *into, int pos, Nc::Graphic::ISceneNode *node, Nc::Graphic::ISceneNode *parent, int oldPos);

    private:
      void    CreateThreadManager();

    private:
      QManager			*_threadManager;
      QGraphicEngine		*_graphic;
      EditorEngine		*_editor;
      Interface::MainWindow     *_GUI;
      Project			*_currentProject;

      PluginLoader              _pluginLoader;
    };
  }
}

#endif
