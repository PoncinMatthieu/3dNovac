
/*-----------------------------------------------------------------------------

	3dNovac GUI
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

    File Created At:        02/06/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------


                        Implementation de la classe "Widget"

    permet de definir des objets 2D qui recoive les evenements de la fenetre
  un widget peut avoir un widget parent et s'affichera en fonction de celui-ci

Herite de Object2d

-----------------------------------------------------------------------------*/

#ifndef NOVAC_GRAPHIC_GUI_WIDGET_H_INCLUDED
#define NOVAC_GRAPHIC_GUI_WIDGET_H_INCLUDED

#include "Define.h"

namespace Nc
{
    namespace GUI
    {
        enum Corner { Top, Bottom, Left, Right, Center };

        class LGUI  Widget : public Graphic::Object2d, public Engine::Handler
        {
            public:
                Widget(const Vector2i &pos = Vector2i(0, 0), const Vector2i &size = Vector2i(10, 10), Corner x = Top, Corner y = Left, Widget *parent = NULL);
                virtual ~Widget();

                Widget(const Widget &w);
                Widget &operator = (const Widget &w);
                virtual void    Copy(const Widget &w); // recopie tous les attribus du widget
                virtual Widget* Clone() const                                   {return new Widget(*this);}

                // update et affichage du widget
                void ManageWindowEvent(const Nc::System::Event &event);
                void Resized();
                virtual void Render(Graphic::ISceneGraph *scene);

                // accesseurs
                const std::list<Widget*> &ListChild() const                     {return _listChild;}
                void                    DeleteChilds();
                virtual bool            Enable() const                          {bool e = _enable; if (e && _parent != NULL) e = (e && _parent->Enable());  return e;} // recursif, false if a parent is set to false
                virtual void            Enable(bool state)                      {_enable = state; _stateChange = false;}
                virtual void            DisplayState(bool state)                {_displayState = state; _stateChange = true;}
                virtual bool            DisplayState() const                    {return _displayState;}
                void                    Percent(Vector2f percent);
                virtual Vector2f        GetReelPos() const;
                Vector2f                GetReelPosRecursif() const;
                virtual Vector2f        GetReelSize() const;
                inline const Vector2i   &Pos() const                            {return _pos;}
                inline void             Pos(const Vector2f &pos)                {_pos = pos; _stateChange = true;}
                inline virtual void     Size(const Vector2f &size)              {_size = size; _stateChange = true;}
                inline const Vector2i   &Size() const                           {return _size;}
                inline Widget           *Parent() const                         {return _parent;}
                void                    Focus(bool state);
                bool                    Focus() const                           {return _focus;}
                void                    AddChild(Widget *child);
                inline void             SetCorner(Corner x, Corner y)           {_corner[0] = x; _corner[1] = y; _stateChange = true;}
                inline Corner           GetCorner(unsigned int i)               {if (i < 2) return _corner[i]; throw Utils::Exception("Widget", "Overflow in function GetCorner");}
                inline void             DrawEdge(bool state)                    {_drawEdge = state; _stateChange = true;}
                inline void             EdgeColor(const Color &color)           {_edgeColor = color; _stateChange = true;}
                inline void             GenerateHandleAtEnterFocus(bool state)  {_generateHandleAtEnterFocus = state; _stateChange = true;}
                inline void             Margin(const Vector2i &margin)          {_margin = margin; _stateChange = true;}
                inline void             MarginX(int x)                          {_margin.Data[0] = x; _stateChange = true;}
                inline void             MarginY(int y)                          {_margin.Data[1] = y; _stateChange = true;}

            protected:
                virtual void Draw(Graphic::ISceneGraph *scene);
                virtual void Update();

                // peut etre redefinit dans une autre classe pour gerer un positionnement specifique au widget
                virtual Vector2f            TranslateChild(const Corner[2]) const {return Vector2f();}
                std::list<std::string>      GetParentChildData();
                virtual std::string         GetData()                       {return "";}

                void CheckFocus(const Nc::System::Event &event);           // effectue le test de focus
                void CheckState();                                         // verifie si le widget ou ses enfant on besoin d'etre Update

                // handler d'evenement de la fenetre
                virtual void MouseMotionEvent(const Nc::System::Event&)     {}
                virtual void MouseButtonEvent(const Nc::System::Event&)     {}
                virtual void KeyboardEvent(const Nc::System::Event&)        {}

                Widget                  *_parent;           // le parent du widget (s'il n'y en a pas NULL)
                std::list<Widget*>      _listChild;         // list des child
                Widget                  *_childFocused;     // le child qui a le focus
                bool                    _focus;             // indique si le widget a le focus
                Vector2i                _size;              // taille (carre) du widget
                Vector2i                _pos;               // position du widget (relatif au parent ET/OU au Corner)
                Corner                  _corner[2];         // corner[0]=Top||Botom  corner[1]=Left||Right

                bool                    _enable;            // eg: if a button or a parent of the button is set false, the button don't exec the handler
                bool                    _stateChange;       // si a true, le widget sera update avant d'etre affiche
                bool                    _drawEdge;          // les contours de la box
                bool                    _generateHandleAtEnterFocus;// si a true, genere un handle lorsqu'on a le focus du widget
                Color                   _edgeColor;         // couleur des contours
                Vector2i                _margin;            // marge a l'interieur d'un widget (influ sur le positionnement des childs)

                Vector2f                _percent;           // si le pourcentage est different de nul, la size sera en fonction du pourcentage et de la taille de la fenetre


                Graphic::Material<Graphic::BasicVertexType::Colored2d>                      *_materialColored2d;

            private:
                virtual void DrawEdgeBox(Graphic::ISceneGraph *scene);

                Graphic::MaterialConfig<Graphic::BasicVertexType::Colored2d>                _configEdgeBox;
                Graphic::GL::GeometryBuffer<Graphic::BasicVertexType::Colored2d, false>     _geometryEdgeBox;
        };
    }
}

#endif

