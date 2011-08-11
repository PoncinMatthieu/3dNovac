
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

    File Created At:        05/12/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef POPULOUS_EDITOR_ENGINE_H_
#define POPULOUS_EDITOR_ENGINE_H_

#include <QObject>
#include "QGraphicEngine.h"
#include "../Define.h"
#include <Nc/Core/Engine/MainEngine.h>
#include <Nc/Graphics/Object/Object.h>
#include <Nc/Graphics/Scene/SceneNodeFormatManager.h>
#include <Nc/Graphics/Effect/LightingEffect.h>
#include <Nc/GUI/GUI.h>

namespace Nc
{
  namespace Editor
  {
    class QManager;

    class EditorEngine : public QObject, public Nc::Engine::MainEngine
    {
      Q_OBJECT;

    public:
      enum IndexCmd
	{
	  IndexCreateNewScene,
	  IndexSelectScene,
          IndexSceneDeleted,
	  IndexLoadNode,
	  IndexSelectNode,
	  IndexRemoveNode,
	  IndexMoveNode,
	  IndexChangeRasterMode
	};

      struct MoveStruct
      {
        MoveStruct(Graphic::ISceneNode *i, int pos, Nc::Graphic::ISceneNode *n, Graphic::ISceneNode *p, int oldPos)
          : into(i), at(pos), node(n), parent(p), oldAt(oldPos) {}

        Graphic::ISceneNode     *into;
        int                     at;
        Graphic::ISceneNode     *node;
        Graphic::ISceneNode     *parent;
        int                     oldAt;
      };

    public:
      EditorEngine(Graphic::Engine *graphic, Engine::Manager *gameManager);
      virtual ~EditorEngine();

      virtual void    LoadContent();

      // accessors:
      inline Graphic::ListPScene      &Scenes()             {return _graphic->GetSceneManager()->Scenes();}
      inline Graphic::ListPScene      &OurScenes()          {return _scenes;}
      inline Graphic::SceneGraph      *CurrentScene()       {return _currentScene;}

    signals:
      void    SignalStop();
      void    SignalScenesChanged(Nc::Graphic::ListPScene*);
      void    SignalSceneChanged(Nc::Graphic::SceneGraph*);
      void    SignalCurrentSceneChanged(Nc::Graphic::SceneGraph*);

    protected:
      virtual void    CreateContext()         {_context = _graphic->CreateSharedContext();}
      virtual void    ActiveContext()         {_context->Active();}
      virtual void    DisableContext()        {_context->Disable();}

      // manage events from Nc::MainEngine
      virtual bool    ReleaseContent();      // called when we try to quit the engine
      virtual void    Update(float runningTime);
      virtual void    ManageWindowEvent(System::Event &event);
      virtual void    KeyboardEvent(System::Event &event);
      virtual void    MouseButtonEvent(System::Event &event);
      virtual void    MouseMotionEvent(System::Event &event);

      // method to manipulate contents
      void            CreateNewScene(const std::string &name);
      void            RemoveScene(Graphic::SceneGraph *scene);
      void            SelectScene(Graphic::SceneGraph *scene);
      void            LoadNode(const Utils::FileName &file);
      void            RemoveNode(Graphic::ISceneNode *node);
      void            DeleteNode(Graphic::ISceneNode *node);
      void            MoveNode(MoveStruct &moveArg);
      void            ChangeRasterMode(Graphic::RasterEffect::Pattern mode);

      // commands from other threads
      inline void     CreateNewSceneCmd(Engine::IEvent *e)                {CreateNewScene(e->GetData<std::string>());}
      inline void     SelectSceneCmd(Engine::IEvent *e)                   {SelectScene(e->GetData<Graphic::SceneGraph*>());}
      void            SceneDeletedCmd(Engine::IEvent *e);
      inline void     LoadNodeCmd(Engine::IEvent *e)                      {LoadNode(e->GetData<Utils::FileName>());}
      void            SelectNodeCmd(Engine::IEvent *e);
      inline void     RemoveNodeCmd(Engine::IEvent *e)                    {RemoveNode(e->GetData<Graphic::ISceneNode*>());}
      void            MoveNodeCmd(Engine::IEvent *e)                      {MoveNode(e->GetData<MoveStruct>());}
      inline void     ChangeRasterModeCmd(Engine::IEvent *e)              {ChangeRasterMode(e->GetData<Graphic::RasterEffect::Pattern>());}

    private:
      bool            _emitChanges;

      // la console, le fpsWidget et le repere sont des objets commun a toutes les scenes
      Graphic::Engine                     *_graphic;
      Graphic::GLContext                  *_context;
      GUI::SceneGraph                     *_GUI;
      Graphic::ListPScene                 _scenes;
      Graphic::SceneGraph                 *_currentScene;
      Graphic::SceneNodeFormatManager     _sceneNodeManager;

      Graphic::ISceneNode                 *_currentNodeSelected;
      Graphic::Light                      *_light;
      Graphic::LightingEffect             *_lightingEffect;
    };
  }
}

Q_DECLARE_METATYPE(Nc::Engine::IEngine*);

#endif
