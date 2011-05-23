
/*-----------------------------------------------------------------------------

	3dNovac PluginFileFormatCollada
	Copyright (C) 2010-2011, 3dNovac Team

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

    File Created At:        28/03/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_FILEOBJ_PLUGIN_H_
#define NC_FILEOBJ_PLUGIN_H_

#include <dae.h>
#include <dom/domCOLLADA.h>
#include <dom/domProfile_COMMON.h>
#include <Nc/Graphics/Graphics.h>
#include <Nc/Graphics/Object/3D/MeshFormatPlugin.h>
#include <Nc/Graphics/Material/DefaultLightingMaterial.h>
#include "Define.h"

namespace Nc
{
    class Plugin : public Graphic::MeshFormatPlugin
    {
        private:
            typedef Graphic::MaterialConfig<Graphic::BasicVertexType::Textured, Graphic::DefaultLightingMaterialConfigPolitic>                                      DefaultMaterialConfig;
            typedef Graphic::Drawable<Graphic::BasicVertexType::Textured, true, Graphic::DefaultLightingMaterialConfigPolitic>                                      DefaultDrawable;
            typedef Graphic::Mesh<Graphic::BasicVertexType::Textured, true, Graphic::DefaultLightingMaterialPolitic, Graphic::DefaultLightingMaterialConfigPolitic> DefaultMesh;

            typedef std::map<std::string, Graphic::GL::Texture>         MapTexture;
            typedef std::map<std::string, DefaultMaterialConfig>        MapMaterialConfig;
            typedef std::map<void*, DefaultDrawable *>                  MapDrawable;
            typedef std::map<std::string, std::string>                  MapMaterial;        // just to link with the material config (effect with collada), weird !
            typedef std::map<std::string, Graphic::Object3d*>           MapNode;

        public:
            Plugin();
            virtual ~Plugin();

            virtual Graphic::Object3d   *Load(const Utils::FileName &file);
            virtual void                Save(const Utils::FileName &file, Graphic::Object3d *obj);

        private:
            Graphic::Object3d *GetFinalObject();

            // images (-> Texture)
            void    ReadImageLibrary(domLibrary_imagesRef lib);
            Graphic::GL::Texture    *ReadImage(domImageRef lib);
            // TODO: en dur, on regarde s'il y a une normal map, et la load automatiquement si elle existe
            Graphic::GL::Texture    *ReadNormalMap(domImageRef lib);

            // effects (-> materilConfig)
            void    ReadEffectLibrary(domLibrary_effectsRef lib);
            void    ReadEffect(domEffectRef lib);

            void    ReadConstant(DefaultMaterialConfig &config, std::map<std::string, domCommon_newparam_type*> &newParams, domProfile_COMMON::domTechnique::domConstant *constant);
            void    ReadLambert(DefaultMaterialConfig &config, std::map<std::string, domCommon_newparam_type*> &newParams, domProfile_COMMON::domTechnique::domLambert *lambert);
            void    ReadPhong(DefaultMaterialConfig &config, std::map<std::string, domCommon_newparam_type*> &newParams, domProfile_COMMON::domTechnique::domPhong *phong);
            void    ReadBlinn(DefaultMaterialConfig &config, std::map<std::string, domCommon_newparam_type*> &newParams, domProfile_COMMON::domTechnique::domBlinn *blinn);
            void    ReadTextureFromTechniqueEffect(DefaultMaterialConfig &config, std::map<std::string, domCommon_newparam_type*> &newParams, domCommon_color_or_texture_type *shader);

            // materials (-> effects)
            void    ReadMaterialLibrary(domLibrary_materialsRef lib);
            void    ReadMaterial(domMaterialRef lib);

            // scene
            void    ReadScene(domVisual_sceneRef scene);
            void    ReadNode(domNodeRef node, Graphic::Object3d *parentNode);
            void    ReadNodeTranforms(Graphic::Object3d *obj, domNodeRef node, Graphic::Object3d *);

            // Drawable
            void    ReadDrawable(domGeometry *lib, DefaultMesh *mesh);
            void    ParseGeometry(domGeometry *domGeometry, DefaultMesh *mesh);
            Box3f   BuildTriangles(domTriangles *domTriangles, DefaultDrawable &drawable);


            DAE                 *_dae;
            MapMaterialConfig   _mapMaterialConfig;
            MapMaterial         _mapMaterial;
            MapTexture          _mapTexture;
            MapTexture          _mapNormalMap;
            MapDrawable         _mapDrawable;
            MapNode             _mapNode;
            TMatrix             _globalMatrix;
            Graphic::Object3d   *_root;
    };
}

#endif // NC_FILEOBJ_PLUGIN_H_
