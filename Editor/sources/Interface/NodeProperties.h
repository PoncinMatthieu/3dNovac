
#ifndef NC_EDITOR_INTERFACE_NODEPROPERTIES_H_
#define NC_EDITOR_INTERFACE_NODEPROPERTIES_H_

#include <QObject>
#include <Nc/Graphics/Camera/StandardCamera3d.h>
#include "StandardEditorItem.h"

namespace Nc
{
  namespace Editor
  {
    namespace Interface
    {
      class IObjectProperties : public QObject
      {
        Q_OBJECT;
          Q_PROPERTY(QString Name READ Name WRITE Name);
          Q_PROPERTY(int Id READ Id WRITE Id);

        public:
          IObjectProperties(StandardEditorItem *item);
          virtual ~IObjectProperties();

          QString       Name() const;
          void          Name(const QString&) const       {}
          int           Id() const;
          void          Id(int) const       {}

        protected:
          StandardEditorItem	*_item;
      };

      class NodeProperties : public IObjectProperties
      {
	Q_OBJECT;
        Q_PROPERTY(bool Enabled READ Enable WRITE Enable);

      public:
        NodeProperties(StandardEditorItem *item);

        bool	Enable() const;
        void	Enable(bool state);
      };

      class SceneProperties : public NodeProperties
      {
        public:
          SceneProperties(StandardEditorItem *item);
      };

      class CameraProperties : public NodeProperties
      {
          Q_OBJECT;
          Q_PROPERTY(bool UseWindowSizeViewport READ UseWindowSizeViewport WRITE UseWindowSizeViewport);
          Q_PROPERTY(QRect Viewport READ Viewport WRITE Viewport);

      public:
          CameraProperties(StandardEditorItem *item);

          bool      UseWindowSizeViewport() const;
          void      UseWindowSizeViewport(bool state);
          QRect     Viewport() const;
          void      Viewport(const QRect &rect);
      };

      class Camera2dProperties : public CameraProperties
      {
        public:
          Camera2dProperties(StandardEditorItem *item);
      };

      class Camera3dProperties : public CameraProperties
      {
        public:
          Camera3dProperties(StandardEditorItem *item);
      };

      class StandardCamera3dProperties : public Camera3dProperties
      {
        Q_OBJECT;
        Q_PROPERTY(Pattern Pattern READ GetPattern WRITE SetPattern);
        Q_ENUMS(Pattern);

        public:
          enum Pattern
          {
              Trackball = Nc::Graphic::StandardCamera3d::Trackball,
              Turntable = Nc::Graphic::StandardCamera3d::Turntable
          };

        public:
          StandardCamera3dProperties(StandardEditorItem *item);

          Pattern   GetPattern() const;
          void      SetPattern(Pattern p);
      };


      class ObjectProperties : public NodeProperties
      {
          Q_OBJECT;
          Q_PROPERTY(bool DisplayBox READ DisplayBox WRITE DisplayBox);

      public:
          ObjectProperties(StandardEditorItem *item);

          bool	DisplayBox() const;
          void	DisplayBox(bool state);
      };

      class EffectProperties : public NodeProperties
      {
          Q_OBJECT;
          Q_PROPERTY(bool Activated READ Activated WRITE Activated);

        public:
          EffectProperties(StandardEditorItem *item);

          bool	Activated() const;
          void	Activated(bool state);
      };

      class GUISceneProperties : public SceneProperties
      {
        public:
          GUISceneProperties(StandardEditorItem *item);
      };

      class WidgetProperties : public ObjectProperties
      {
        Q_OBJECT;
        Q_PROPERTY(bool Inhibited READ Inhibit WRITE Inhibit);

        public:
          WidgetProperties(StandardEditorItem *item);

          bool  Inhibit() const;
          void  Inhibit(bool state);
      };
    }
  }
}

#endif
