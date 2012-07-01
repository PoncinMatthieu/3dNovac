
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

#ifndef NOVAC_GRAPHIC_OBJECT_H_
#define NOVAC_GRAPHIC_OBJECT_H_

#include "../Scene/SceneNode.h"
#include "../Material/Drawable.h"
#include "../Material/DefaultMaterials.h"
#include "../Material/DefaultVertexType.h"

namespace Nc
{
    namespace Graphic
    {
        /// Define a Graphical object entity
        class LGRAPHICS Object : public Entity
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_VISITABLE(Entity, System::Object, Nc::Graphic::Object);

                typedef Entity                      NodeType;
                typedef std::vector<Drawable*>      DrawableArray;

            public:
            // construct
                Object();
                Object(const TMatrix &m);
                Object(const Box3f &box);
                Object(const Box3f &box, const TMatrix &m);
                Object(const Object &o);
                Object &operator = (const Object &o);
                virtual ~Object();

                virtual ISceneNode  *Clone() const                      {return new Object(*this);}

                /** \return the material used to render the drawables */
                inline IMaterial    *GetMaterial()                      {return _material;}
                /** \return the material used to render the drawables */
                inline const IMaterial *GetMaterial() const             {return _material;}
                /**
                    Set the material, reconfigure the drawables.
                    The configuration could failed if the given material is not compatible. In this case the current material don't change and return false.
                    \return true the material transfer succeed.
                */
                bool                SetMaterial(IMaterial *newMaterial);
                /** Choose the best default material to display the drawables */
                void                ChooseDefaultMaterial();

                /** \return the useMaterial statement, if true, the object will be rendered with the current scene material. Otherwise the object is rendered with the object material. Default value:true */
                inline bool         UseSceneMaterial() const            {return _useSceneMaterial;}
                /** Set the useMaterial statement, if true, the object will be rendered with the current scene material. Otherwise the object is rendered with the object material. Default value:true  */
                inline void         UseSceneMaterial(bool state)        {_useSceneMaterial = state;}

                /** \return the drawables */
                DrawableArray       &Drawables()                        {return _drawables;}
                /** Configure the drawables by using the current Material */
                void                ReconfigureDrawables();

            protected:
                /**
                    Call the Draw method and render the childs of the object.
                    Call the RenderBegin and RenderEnd method before and after the rendering.
                */
                virtual void        Render(SceneGraph *scene);

                /**
                    Method called just before the Draw method.
                    Update the model matrix of the scene before drawing.
                */
                virtual void        RenderBegin(SceneGraph *scene);

                /**
                    Method called just after the draw method.
                    Update the model matric of the scene after drawing.
                */
                virtual void        RenderEnd(SceneGraph *scene);

                /**
                    Method called just before the RenderChilds method.
                    Do nothing but can be redefined.
                */
                virtual void        RenderChildsBegin(SceneGraph *scene)        {}

                /**
                    Method called just after the RenderChilds method.
                    Do nothing but can be redefined.
                */
                virtual void        RenderChildsEnd(SceneGraph *scene)          {}

                /**
                    Render the drawables.
                    To redefine in your own objects.
                */
                virtual void        Draw(SceneGraph *scene);

                /**
                    Transform the scene model matrix before rendering the object.
                    Could be redefine to set a specific comportement.
                */
                virtual void        TransformModelMatrixToRender(SceneGraph *scene);

                /** Configure the drawables by using the given \p material */
                void                ConfigureDrawables(IMaterial *material);

            // fields
            protected:
                DrawableArray   _drawables;                 ///< the array of drawbles
                bool            _useSceneMaterial;          ///< if true, the object will be rendered with the current scene material. Otherwise the object is rendered with the object material. Default value:true

            private:
                IMaterial       *_material;                 ///< pointer to the material used to render the drawables
                IMaterial       *_lastConfiguredMaterial;   ///< pointer to the last configured material
        };
    }
}

#endif
