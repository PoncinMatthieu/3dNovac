
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
            // typedefs
                typedef Entity                      NodeType;
                typedef std::vector<Drawable*>      DrawableArray;

                struct SetMaterialFonctor
                {
                    SetMaterialFonctor(IMaterial *m) : newMaterial(m)  {}

                    bool operator () (ISceneNode *node)
                    {
                        Object *n = node->AsWithoutThrow<Object>();
                        if (n != NULL)
                            n->SetMaterial(newMaterial);
                        return true;
                    }

                    IMaterial *newMaterial;
                };

            public:
            // construct
                Object();
                Object(const char *className);
                Object(const TMatrix &m);
                Object(const Box3f &box);
                Object(const Box3f &box, const TMatrix &m);
                Object(const Object &o);
                Object &operator = (const Object &o);
                virtual ~Object();

                static const char *ClassName()                          {return "Object";}
                virtual ISceneNode  *Clone() const                      {return new Object(*this);}

                /** Set the display statement */
                inline virtual void DisplayState(bool state)            {_enabled = state;}
                /** \return the display statement */
                inline bool         DisplayState() const                {return _enabled;}
                /** \return false if the object or one of its parents has the display statement to false */
                bool                DisplayStateRecursive() const;

                /** Set the display box statement */
				inline void         DisplayStateBox(bool state)         {_displayStateBox = state;}
                /** Return the display box statement */
                inline bool         DisplayStateBox() const             {return _displayStateBox;}

                /** Compute the recursive model matrix with the parents matrix. */
                void                GetRecursiveMatrix(TMatrix &m);

                /** Set the box of the object */
                inline void         SetBox(const Box3f &box)            {_box = box;}
                /** \return the box of the object (the box is not modified by the matrix, so if you want the reel box call the methode GetReelBox) */
                inline const Box3f  &GetBox() const                     {return _box;}
                /** \param box filled by the box transformed by the model matrix */
                void                GetReelBox(Box3f &box) const;

                // operations sur la matrice
                /** Scale the object with the given height */
                void                HeightScale(float height); // /!\ metre a jour la bounding box avant

                /** Translate the model Matrix to the given vector using the xy center of the box */
                void                CenterBase(const Vector3f &centerBase);

                void                SetRecursiveMaterial(IMaterial *newMaterial);

                /** Set the material, reconfigure the drawables.
                    The configuration could failed if the given material is not compatible. In this case the current material don't change and return false.
                    \return true the material transfer succeed.
                */
                bool                SetMaterial(IMaterial *newMaterial);

                void                ChooseDefaultMaterial();

                inline IMaterial    *GetMaterial()                      {return _material;}


                /** \return the drawables */
                DrawableArray       &Drawables()                        {return _drawables;}

                /** Configure the drawables by using the current Material */
                void                ReconfigureDrawables();

                /** Render the drawables with the material set in the object */
                virtual void        Render(SceneGraph *scene);

            protected:
                /**
                    Transform the scene model matrix before rendering the object.
                    Could be redefine to set a specific comportement.
                */
                virtual void        TransformModelMatrixToRender(SceneGraph *scene)    {scene->ModelMatrix().AddTransformation(Matrix);}

                /**
                    Render the drawables.
                    To redefine in your own objects.
                */
                virtual void        Draw(SceneGraph *scene);

            // fields
            protected:
                bool            _displayStateBox;       ///< if true, draw the box of the object]

                DrawableArray   _drawables;             ///< the array of drawbles
                Box3f           _box;                   ///< the box of the object

            private:
                IMaterial       *_material;             ///< pointer to the material used to render the drawables
        };

        // specialization of the method GetNode<> for an IObject
        //template<>
        //Object *ISceneNode::GetNode<>();
    }
}

#endif
