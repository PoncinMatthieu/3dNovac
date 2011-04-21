
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
/*-----------------------------------------------------------------------------

        include all contants of Nc graphics

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_DEFINE_H_
#define NC_GRAPHICS_DEFINE_H_

// includes extern
    #include <list>
    #include <Nc/Define.h>
    #include <Nc/Core/Core.h>

    //#define DYNAMIC_LINK
    //#define EXPORT_MATH

// linking option (static or shared)
    #ifdef SYSTEM_WINDOWS                                       // windows
        #ifdef DYNAMIC_LINK                                     // dynamic
            #ifdef EXPORT_GRAPHICS
                #define LGRAPHICS     __declspec(dllexport)
            #else
                #define LGRAPHICS     __declspec(dllimport)
            #endif
        #else                                                   // static (do nothing)
            #define LGRAPHICS
        #endif
    #else                                                       // othes os (do nothing)
        #define LGRAPHICS       ///< Linking option of Nc-Graphics module
    #endif

// includes interne
    #include "Window/Window/Window.h"

// define
    #ifdef  _DEBUG
        #define     _DEBUG_LIGHT
        #define     _DEBUG_GLOBJECT
    #endif

// Singleton
    #define EXT                 Nc::Graphic::GL::Extension::Instance()          ///< to access to the Extension class

    namespace Nc
    {
        namespace Graphic
        {
            // definition of the classes to typedef the diferrent types
            // default material definition
            template<typename VertexType>
            class DefaultMaterialPolitics;
            template<typename VertexType>
            class DefaultMaterialConfigPolitics;
            template<typename VertexType, typename MaterialPolitic = DefaultMaterialPolitics<VertexType> >
            class Material;

            // drawable
            template<typename VertexType, typename MaterialConfigPolitic = DefaultMaterialConfigPolitics<VertexType> >
            struct IDrawable;
            template<typename VertexType, bool INDEX = true, typename MaterialConfigPolitic = DefaultMaterialConfigPolitics<VertexType> >
            struct Drawable;

            // Graphical objects
            class Basic3dSceneGraph;
            class Object;
            class Object2d;
            class Object3d;
            template<typename VertexType, bool INDEX = true, typename MaterialPolitic = DefaultMaterialPolitics<VertexType>, typename MaterialConfigPolitic = DefaultMaterialConfigPolitics<VertexType> >
            class Mesh;
            class Light;

            // effects
            class Effect;
            template<typename FRAME>
            class Animation;
            class SpriteAnimFrame;

            // camera
            class Camera;
            class Camera2d;
            class Camera3d;
            class StandardCamera3d;

            // typedef
            typedef Animation<SpriteAnimFrame>      SpriteAnim;
            typedef std::list<Basic3dSceneGraph*>   ListPBasic3dSceneGraph;
            typedef std::list<Object*>              ListPObject;
            typedef std::list<Object2d*>            ListPObject2d;
            typedef std::list<Object3d*>            ListPObject3d;
            typedef std::list<Light*>               ListPLight;
            typedef std::list<Effect*>              ListPEffect;
        }
    }

    using namespace Nc::Math;

#endif

