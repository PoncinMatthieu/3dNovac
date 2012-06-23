
#ifndef SOKOBANDEMO_MAP_H_
#define SOKOBANDEMO_MAP_H_

#include <Nc/Graphics/Scene/SceneNodeFormatManager.h>
#include <Nc/Graphics/Object/Object.h>

namespace SokobanDemo
{
    class Map : public Nc::Graphic::Object
    {
     public:
      /// Gestion du type des objects
      enum ObjType
      {
        NONE		= -1,
        NB_OBJECT		= 3,
        WALL		= '1',
        FLOOR		= '2',
        OBJECTIF		= '3',
        CASE		= '4',
        CASE_OBJECTIF	= '5',
        PLAYER
      };

      typedef Array<Object*, Map::NB_OBJECT>	ArrayObject;

      /// Gestion de l'orientation du player
      enum Orientation
      {
        Up,
        Down,
        Right,
        Left
      };

      /// Defini un objet par son type, son model 3d, et sa matrice identity (avant d'etre positionne dans la map)
      struct MapObject
      {
        ObjType	type;
        Object	*object;
        TMatrix	identity;
      };

     public:
      Map(const Nc::Utils::FileName &file, Nc::Graphic::SceneNodeFormatManager &sceneNodeFormatManager);
      ~Map();

      /** \return the center of the map (for the camera) */
      const Vector3f	&Center() const		{return _center;}

      // move le joueur, et manage les deplacements de caisses
      void	MovePlayerUp();
      void	MovePlayerDown();
      void	MovePlayerRight();
      void	MovePlayerLeft();

     private:
      // lecture du fichier
      void	ReadFile(const Nc::Utils::FileName &file, ArrayObject &objects);
      void	FillMap(const std::list<std::string> &listStringMap, ArrayObject &objects);

      // changement des textures (pour simuler les changements de type (case - case sur objectif))
      void	ChangeTexture(Object *obj, Nc::Graphic::GL::Texture &diffuse, Nc::Graphic::GL::Texture &normalMap);

      // update la matrice du player en fonction de son orientation
      void	UpdatePlayerMatrix(Orientation o);

      // return 0 si on peut pas bouger, 2 si il y a une caisse a push
      int	CanMove(const Vector2i &newPos, const Vector2i &newPosCase);

      // push la caisse
      void	PushCase(const Vector2i &newPos, const Vector2i &newPosCase);

     private:
      MapObject						**_map;
      MapObject						_player;

      Vector2i	_posPlayer;
      Vector3f	_center;
      unsigned int	_nbObjectif;
      unsigned int	_nbCaisseObjectif;

      Nc::Graphic::GL::Texture	_textureWall;
      Nc::Graphic::GL::Texture	_textureWallNormal;
      Nc::Graphic::GL::Texture	_textureFloor;
      Nc::Graphic::GL::Texture	_textureFloorNormal;
      Nc::Graphic::GL::Texture	_textureObjectif;
      Nc::Graphic::GL::Texture	_textureObjectifNormal;
      Nc::Graphic::GL::Texture	_textureCase;
      Nc::Graphic::GL::Texture	_textureCaseNormal;
      Nc::Graphic::GL::Texture	_textureCaseObjectif;
      Nc::Graphic::GL::Texture	_textureCaseObjectifNormal;
    };
}

#endif
