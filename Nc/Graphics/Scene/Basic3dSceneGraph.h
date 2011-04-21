
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

    File Created At:        25/01/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_BASIC3DSCENEGRAPH_H_
#define NC_GRAPHICS_BASIC3DSCENEGRAPH_H_

#include "I3dSceneGraph.h"

namespace Nc
{
    namespace Graphic
    {
        /// Define a basic SceneGraph to renderer a scene in 3 dimension.
        /**
            The Basic3dSceneGraph is an I3dSceneGraph used by the SceneGraphManager to render the scene. <br/>
            <br/>
            Could be redefine to render some specifique renderering effect like preprocessing/postprocessing effect. <br/>
            <br/>
            The basic 3d SceneGraph Could be see as a container of drawable Object witch describe and used to renderer the scene.
            But the Objects contained be the Basic3dSceneGraph are not deleted when the scene is destroyed ! <br/>
            Unfortunately there are no routine or abstraction to manage any frustum or space partitionning.
        */
        class LGRAPHICS Basic3dSceneGraph : public I3dSceneGraph
        {
            public:
                Basic3dSceneGraph(Camera3d *camera);
                virtual ~Basic3dSceneGraph();

                // accessors
                /// Return the list of objects contained by the Basic3dSceneGraph
                inline const ListPObject    &Objects() const                {return _listObject;}

                /// Return the list of lights contained by the Basic3dSceneGraph
                inline const ListPLight     &Lights() const                 {return _listLight;}

                /// Return the list of effects contained by the Basic3dSceneGraph
                inline const ListPEffect    &Effects() const                {return _listEffect;}

                // manage content
                /// To Add an object, Add it to the list of light or effect or Object with a dynamic_cast test
                void        AddObject(Graphic::Object* objectToAdd);

                /// To Delete an object, search it in the list of light or effect or object with a dynamic_cast test
                void        DeleteObject(Graphic::Object *objectToDelete, bool del = true);

                /// clear the content of the scene, if del is true, we delete the objects
                void        Clear(bool del);

                /// Return the AmbiantColor of the scene (for lighting effect)
                inline void                 ColorAmbiant(Color c)       {_colorAmbiant = c;};
                inline const Color          &ColorAmbiant() const       {return _colorAmbiant;}

                /// rendering the scene (all Objects, lights and effects), Called by the SceneGraph
                virtual void                Render();

                /// Update all Effects. You will need to call this function in the GameEngine
                virtual void                Update(float runningTime);

            protected:
                ListPObject             _listObject;        ///< the list of object used to render the scene
                ListPLight              _listLight;         ///< the list of light used by the DefaultLightingMaterial
                ListPEffect             _listEffect;        ///< the list of effect
                System::Mutex           _mutex;             ///< the mutex witch protect the lists

                Color                   _colorAmbiant;      ///< the color that used by DefaultLightingMaterial

//                Fog                     _fog;               // objet permettant la gestion du fog
        };
    }
}


#endif
