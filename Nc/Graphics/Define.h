
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
    #include <Nc/Core/Core.h>

    //#define NC_DYNAMIC_LINK
    //#define NC_EXPORT_GRAPHICS

// linking option (static or shared)
    #ifdef SYSTEM_WINDOWS                                       // windows
        #ifdef NC_DYNAMIC_LINK                                     // dynamic
            #ifdef NC_EXPORT_GRAPHICS
                #define LIB_NC_GRAPHICS     __declspec(dllexport)
            #else
                #define LIB_NC_GRAPHICS     __declspec(dllimport)
            #endif
        #else                                                   // static (do nothing)
            #define LIB_NC_GRAPHICS
        #endif
    #else                                                       // othes os (do nothing)
        #define LIB_NC_GRAPHICS       ///< Linking option of Nc-Graphics module.
    #endif

    namespace Nc
    {
        /// Base namespace of the 3dNovac Graphical Engine
        namespace Graphic
        {
            /// This namespace reference every classes that abstract the OpenGL librairy.
            namespace GL
            {
                class IGeometryBuffer;
                struct Drawable;
                class MaterialConfig;

                typedef std::vector<Drawable*>      DrawableArray;
            }

            class Font;
            typedef std::map<std::string, Font*>    FontMap;

            // definition of the classes to typedef the diferrent types
            // material definition
            class IMaterial;
            template<typename VertexType>
            class DefaultMaterial;


            // SceneNodes
            //------------------------------------------------------------------------------------
                class ISceneNode;

            // SceneGraph
                class SceneGraph;
                class SceneGraphManager;

            // space partitioning
                //... comming soon

            // camera
                class Camera;
                class Camera2d;
                class Camera3d;
                class StandardCamera3d;

            // Graphical objects
                class Object;
                class Text;
                class Sprite;
                class BoundingBox;
                class SkyBox;

            // effects
                class Effect;
                class Animation;
                template<typename Frame>
                class FramesAnimation;
                class SpriteAnimFrame;
                typedef FramesAnimation<SpriteAnimFrame>        SpriteAnim;
                class LightingEffect;
                struct Light;
                class DefaultLightingMaterial;
            //------------------------------------------------------------------------------------

            // typedef
            typedef std::list<SceneGraph*>          ListPScene;
            typedef std::list<ISceneNode*>          ListPSceneNode;
            typedef std::list<Object*>              ListPObject;
            typedef std::list<Effect*>              ListPEffect;
            typedef std::list<Animation*>           ListPAnimation;
            typedef std::list<Light>                ListLight;
            typedef std::list<Light*>               ListPLight;
        }
    }

    using namespace Nc::Math;

// includes interne
    #include "Window/IWindow.h"

// define
    #ifdef  _DEBUG
        #define     _DEBUG_LIGHT
        #define     _DEBUG_GLOBJECT
        //#define     _DEBUG_MATERIALS_RENDERING
    #endif

// Singleton
    #define EXT                 Nc::Graphic::GL::Extension::Instance()          ///< to access to the Extension class

#endif

