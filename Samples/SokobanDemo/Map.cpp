
#include <Nc/Contrib/BasicMeshCreator.h>
#include <Nc/Graphics/Object/BoundingBox.h>
#include "Map.h"

using namespace Nc;
using namespace Nc::Graphic;

Map::Map(const Utils::FileName &file, Nc::Graphic::SceneNodeFormatManager &sceneNodeFormatManager)
  : Object()
{
  // chargement des objets de bases
  Map::ArrayObject			objects;
  objects[0] = sceneNodeFormatManager.Load("Nc:Mesh:cube/cube.dae")->As<Graphic::Object>();
  objects[1] = BasicMeshCreator::Plan(Vector2f(1,1), GL::Texture());
  objects[2] = sceneNodeFormatManager.Load("Nc:Mesh:Tux/TuxWizard.dae")->As<Graphic::Object>();

  // loading des textures de base
  _textureWall.LoadFromFile("Nc:Mesh:cube/brick086.png");
  _textureWallNormal.LoadFromFile("Nc:Mesh:cube/brick086_normal.png");

  _textureFloor.LoadFromFile("Nc:Mesh:cube/ground006.png");
  _textureFloorNormal.LoadFromFile("Nc:Mesh:cube/ground006_normal.png");

  _textureObjectif.LoadFromFile("Nc:Mesh:cube/ground008.png");
  _textureObjectifNormal.LoadFromFile("Nc:Mesh:cube/ground008_normal.png");

  _textureCase.LoadFromFile("Nc:Mesh:cube/crate03.png");
  _textureCaseNormal.LoadFromFile("Nc:Mesh:cube/crate03_normal.png");

  _textureCaseObjectif.LoadFromFile("Nc:Mesh:cube/crate04.png");
  _textureCaseObjectifNormal.LoadFromFile("Nc:Mesh:cube/crate04_normal.png");

  // lecture du fichier
  ReadFile(file, objects);

  // delete les objets qui sont maintenant inutile
  delete objects[0];
  delete objects[1];

  // set le player
  _player.type = PLAYER;
  _player.object = static_cast<Object*>(objects[2]->Clone());
  _player.identity = objects[2]->Matrix;
  _player.object->AddChild(new BoundingBox(_player.object->GetBox()));

  // ajoute le player
  UpdatePlayerMatrix(Up);
  AddChild(_player.object);
  LOG << "end" << std::endl;
}

Map::~Map()
{
}

void Map::ReadFile(const Utils::FileName &file, ArrayObject &objects)
{
	std::ifstream	f(file.c_str());
  if(!f)
    throw Utils::Exception("Map", "Can't open the file `" + file + "`");

  std::string	line, section;
  while (!f.eof())
    {
		Utils::Getline(f, line);
      // recupere la position du player
      Utils::Convert::SplitStringTo(line, "=", section);

	LOG << Utils::String::npos << std::endl;
	   
	  if (section == "PLAYER")
	{
	  Utils::Convert::SplitStringTo(line, " ", _posPlayer[0]);
	  Utils::Convert::SplitStringTo(line, " ", _posPlayer[1]);
	}
      // recup la position du centre de la map (pour la camera)
      else if (section == "CENTER")
	{
	  Utils::Convert::SplitStringTo(line, " ", _center[0]);
	  Utils::Convert::SplitStringTo(line, " ", _center[1]);
	  _center[2] = 0.5;
	}
      // read toute la map, et creer les objets correspondant
      else if (section == "MAP")
	{
	  std::list<std::string>	listStringMap;
	  for (Utils::Getline(f, line); !f.eof(); Utils::Getline(f, line))
	      listStringMap.push_back(line);
	  FillMap(listStringMap, objects);
	}
    }
}

void Map::FillMap(const std::list<std::string> &listStringMap, ArrayObject &objects)
{
  unsigned int	i = 0;

  _nbObjectif = 0;
  _nbCaisseObjectif = 0;

  _map = new MapObject*[listStringMap.size() + 1];
  for (std::list<std::string>::const_iterator it = listStringMap.begin(); it != listStringMap.end(); ++it, ++i)
    {
      _map[i] = new MapObject[it->size() + 1];
      unsigned int j = 0;
      for (; j < it->size(); j++)
	{
	  _map[i][j].type = (ObjType)it->c_str()[j];

	  // si on a bien un objet
	  if (_map[i][j].type >= WALL && _map[i][j].type <= CASE_OBJECTIF)
	    {
	      // on clone la box ou le sol en fonction du type
	      if(_map[i][j].type == FLOOR || _map[i][j].type == OBJECTIF)
		{
		  _map[i][j].object = static_cast<Object*>(objects[1]->Clone());
		  _map[i][j].identity = objects[1]->Matrix;
		}
	      else
		{
		  _map[i][j].object = static_cast<Object*>(objects[0]->Clone());
		  _map[i][j].identity = objects[0]->Matrix;
		}
	      _map[i][j].object->Matrix.AddTranslation(i, j, 0);

	      // ajoute comme enfant le nouvel objet
	      AddChild(_map[i][j].object);

	      // modification des textures en fonction du type de l'objet
	      switch (_map[i][j].type)
		{
		case WALL:		ChangeTexture(_map[i][j].object, _textureWall, _textureWallNormal);		        break;
		case CASE:		ChangeTexture(_map[i][j].object, _textureCase, _textureCaseNormal);		        break;
		case CASE_OBJECTIF:	ChangeTexture(_map[i][j].object, _textureCaseObjectif, _textureCaseObjectifNormal);     break;
		case FLOOR:		ChangeTexture(_map[i][j].object, _textureFloor, _textureFloorNormal);		        break;
		case OBJECTIF:		ChangeTexture(_map[i][j].object, _textureObjectif, _textureObjectifNormal);	        break;
		default:		break;
		}

	      // on en profite pour comptabiliser le nombre d'objectif et de caisse qui sont deja dessu
	      if (_map[i][j].type == OBJECTIF)
		_nbObjectif++;
	      else if (_map[i][j].type == CASE_OBJECTIF)
		_nbCaisseObjectif++;
	    }
	}
      _map[i][j].type = NONE;
    }
  _map[i] = 0;
}

void Map::ChangeTexture(Object *obj, GL::Texture &diffuse, GL::Texture &normalMap)
{
  for (unsigned int i = 0; i < obj->Drawables().size(); ++i)
    {
      Array<GL::Texture> &textures = obj->Drawables()[i]->Config->Textures;
      if (textures.Size() < 2)
	textures.InitSize(2);
      textures[0] = diffuse;
      textures[1] = normalMap;
    }
}

void	Map::MovePlayerUp()
{
  Vector2i newPos(_posPlayer), newPosCase(_posPlayer);
  newPos[1] += 1;
  newPosCase[1] += 2;

  // si on peut bouger, on bouge et on push la case en fonction du result
  int r = CanMove(newPos, newPosCase);
  if (r > 0)
    _posPlayer = newPos;
  if (r == 2)
    PushCase(newPos, newPosCase);

  UpdatePlayerMatrix(Up);
}

void	Map::MovePlayerDown()
{
  Vector2i newPos(_posPlayer), newPosCase(_posPlayer);
  newPos[1] -= 1;
  newPosCase[1] -= 2;

  // si on peut bouger, on bouge et on push la case en fonction du result
  int r = CanMove(newPos, newPosCase);
  if (r > 0)
    _posPlayer = newPos;
  if (r == 2)
    PushCase(newPos, newPosCase);

  UpdatePlayerMatrix(Down);
}

void	Map::MovePlayerRight()
{
  Vector2i newPos(_posPlayer), newPosCase(_posPlayer);
  newPos[0] += 1;
  newPosCase[0] += 2;

  // si on peut bouger, on bouge et on push la case en fonction du result
  int r = CanMove(newPos, newPosCase);
  if (r > 0)
    _posPlayer = newPos;
  if (r == 2)
    PushCase(newPos, newPosCase);

  UpdatePlayerMatrix(Right);
}

void	Map::MovePlayerLeft()
{
  Vector2i newPos(_posPlayer), newPosCase(_posPlayer);
  newPos[0] -= 1;
  newPosCase[0] -= 2;

  // si on peut bouger, on bouge et on push la case en fonction du result
  int r = CanMove(newPos, newPosCase);
  if (r > 0)
    _posPlayer = newPos;
  if (r == 2)
    PushCase(newPos, newPosCase);

  UpdatePlayerMatrix(Left);
}

void	Map::UpdatePlayerMatrix(Orientation o)
{
  // modifie la matrice du player
  _player.object->Matrix = _player.identity;
  _player.object->HeightScale(1);
  _player.object->CenterBase(Vector3f(0,0,0));
  if (o == Right)
    _player.object->Matrix.AddRotation(Vector3f(0,0,1), 180, false);
  else if (o == Down)
    _player.object->Matrix.AddRotation(Vector3f(0,0,1), 90, false);
  else if (o == Up)
    _player.object->Matrix.AddRotation(Vector3f(0,0,1), -90, false);
  _player.object->CenterBase(Vector3f(0.5f,0.5f,0));
  _player.object->Matrix.AddTranslation(_posPlayer[0], _posPlayer[1], 0);
}

int	Map::CanMove(const Vector2i &newPos, const Vector2i &newPosCase)
{
  // si on est devant un mur
  if (_map[newPos[0]][newPos[1]].type == WALL)
    return 0;

  // si on est devant une caisse
  if (_map[newPos[0]][newPos[1]].type == CASE || _map[newPos[0]][newPos[1]].type == CASE_OBJECTIF)
    {
      // si il y a un deuxieme obstacle
      if (_map[newPosCase[0]][newPosCase[1]].type == CASE ||
	  _map[newPosCase[0]][newPosCase[1]].type == CASE_OBJECTIF ||
	  _map[newPosCase[0]][newPosCase[1]].type == WALL)
	return 0;
      else
	return 2;
    }
  return 1;
}

void	Map::PushCase(const Vector2i &newPos, const Vector2i &newPosCase)
{
  // swap les 2 position
  Math::Swap(_map[newPos[0]][newPos[1]], _map[newPosCase[0]][newPosCase[1]]);

  // update les matrices
  _map[newPos[0]][newPos[1]].object->Matrix = _map[newPos[0]][newPos[1]].identity;
  _map[newPos[0]][newPos[1]].object->Matrix.AddTranslation(newPos[0], newPos[1], 0);

  _map[newPosCase[0]][newPosCase[1]].object->Matrix = _map[newPosCase[0]][newPosCase[1]].identity;
  _map[newPosCase[0]][newPosCase[1]].object->Matrix.AddTranslation(newPosCase[0], newPosCase[1], 0);

  // si on rentre dans un objectif
  if (_map[newPos[0]][newPos[1]].type == OBJECTIF && _map[newPosCase[0]][newPosCase[1]].type == CASE)
    {
      ChangeTexture(_map[newPosCase[0]][newPosCase[1]].object, _textureCaseObjectif, _textureCaseObjectifNormal);
      ChangeTexture(_map[newPos[0]][newPos[1]].object, _textureFloor, _textureFloorNormal);
      _map[newPosCase[0]][newPosCase[1]].type = CASE_OBJECTIF;
      _map[newPos[0]][newPos[1]].type = FLOOR;
      _nbCaisseObjectif++;
    }
  // sinon, si on en sort
  else if (_map[newPos[0]][newPos[1]].type == FLOOR && _map[newPosCase[0]][newPosCase[1]].type == CASE_OBJECTIF)
    {
      ChangeTexture(_map[newPosCase[0]][newPosCase[1]].object, _textureCase, _textureCaseNormal);
      ChangeTexture(_map[newPos[0]][newPos[1]].object, _textureObjectif, _textureObjectifNormal);
      _map[newPosCase[0]][newPosCase[1]].type = CASE;
      _map[newPos[0]][newPos[1]].type = OBJECTIF;
      _nbCaisseObjectif--;
    }
}
