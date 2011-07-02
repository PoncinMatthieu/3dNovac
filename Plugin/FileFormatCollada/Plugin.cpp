
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

#include "Plugin.h"
#include "Triangulate.h"

using namespace Nc;
using namespace Nc::Graphic;


Plugin::Plugin() : SceneNodeFormatPlugin("dae")
{
}

Plugin::~Plugin()
{
}

ISceneNode   *Plugin::Load(const Utils::FileName &file)
{
    LOG << "COLLADA_DOM Load Started " << file << std::endl;

    _dae = new DAE();
    if (_dae->load(file.c_str()) != DAE_OK)
        throw Utils::Exception("Error loading the COLLADA file `" + file + "` make sure it is COLLADA 1.4 or greater");

    // !!!GAC The dom (beta 10 and up) now identifies a collection by a full URI rather than the base file name.
    // !!!GAC getDOM automatically resolves relative URIs before searching, "nameOnly" should probably be replaced
    // !!!GAC with LFileName if it's contents will always be a valid relative or absolute URI...need to check this out
    // !!!GAC make the change and retest.
    domCOLLADA *dom = _dae->getDom(file.Filename().c_str());
    if (!dom)
        dom = _dae->getDom(file.c_str());
    if (!dom)
        throw Utils::Exception("COLLADA File loaded to the dom, but query for the dom assets failed \nCOLLADA Load Aborted!");

    Nc::Triangulate t(_dae);
    t.Process();

    // Need to now get the asset tag which will determine what vector x y or z is up.  Typically y or z.
    if (dom->getAsset()->getUp_axis())
    {
        domAsset::domUp_axis * up = dom->getAsset()->getUp_axis();
        switch(up->getValue())
        {
            case UPAXISTYPE_X_UP:
                LOG << "X Axis is Up axis!  The ModelMatrix will be adjusted" << std::endl;
                _globalMatrix.RotationY(90, false);
                break;
            case UPAXISTYPE_Y_UP:
                LOG << "Y Axis is Up axis!  The ModelMatrix will be adjusted" << std::endl;
                _globalMatrix.RotationX(90, false);
                break;
            case UPAXISTYPE_Z_UP:
                break;
            default:
                break;
        }
    }

    // Load all the image libraries
    LOG << "Load image libraries\nThere are " << dom->getLibrary_images_array().getCount() << " images libraries" << std::endl;
    for (unsigned int i = 0; i < dom->getLibrary_images_array().getCount(); i++)
        ReadImageLibrary(dom->getLibrary_images_array()[i]);

    // Load all the effect libraries
    LOG << "Load effect libraries\nThere are " << dom->getLibrary_effects_array().getCount() << " effect libraries" << std::endl;
    for (unsigned int i = 0; i < dom->getLibrary_effects_array().getCount(); i++)
        ReadEffectLibrary(dom->getLibrary_effects_array()[i]);

    // Load all the material libraries
    LOG << "Load material libraries\nThere are " << dom->getLibrary_materials_array().getCount() << " material libraries" << std::endl;
    for (unsigned int i = 0; i < dom->getLibrary_materials_array().getCount(); i++)
        ReadMaterialLibrary(dom->getLibrary_materials_array()[i]);

/*
    // Load all the animation libraries
    for (unsigned int i = 0; i < dom->getLibrary_animations_array().getCount(); i++)
        ReadAnimationLibrary( dom->getLibrary_animations_array()[i] );
*/

    // Load the Scene
    domCOLLADA::domSceneRef domScene = dom->getScene();
    daeElement *defaultScene = NULL;
    if (domScene && domScene->getInstance_visual_scene() && domScene->getInstance_visual_scene())
        defaultScene = domScene->getInstance_visual_scene()->getUrl().getElement();
    if (defaultScene)
        ReadScene((domVisual_scene*)defaultScene);

    ISceneNode *finalNode = GetFinalNode();

    _mapTexture.clear();
    _mapNormalMap.clear();
    _mapMaterialConfig.clear();
    _mapDrawable.clear();
    _mapNode.clear();
    _globalMatrix.SetIdentity();
    delete _dae;
    return finalNode;
}

ISceneNode *Plugin::GetFinalNode()
{
    if (_root != NULL)
        return _root;
    else if (_mapNode.size() != 0)
        return _mapNode.begin()->second;
    return NULL;
}

void        Plugin::Save(const Utils::FileName &file, ISceneNode *node)
{
    LOG << "Can't save the object, function not implemented" << std::endl;
}

void Plugin::ReadImageLibrary(domLibrary_imagesRef lib)
{
	for (unsigned int i = 0; i < lib->getImage_array().getCount(); i++)
        ReadImage(lib->getImage_array()[i]);
}

Graphic::GL::Texture    *Plugin::ReadImage(domImageRef lib)
{
    if (lib==NULL || lib->getId()==NULL)
        return NULL;

    MapTexture::iterator it = _mapTexture.find(lib->getID());
    if (it != _mapTexture.end()) // if already exist
        return &it->second;

    LOG << "Create new image " << lib->getId() << std::endl;
    domImage *imageElement = (domImage*)(domElement*)lib;
    if (imageElement)
    {
        GL::Texture *texture = &_mapTexture[imageElement->getId()];
        texture->LoadFromFile(cdom::uriToNativePath(imageElement->getInit_from()->getValue().str()));
        return texture;
    }
    return NULL;
}

Graphic::GL::Texture    *Plugin::ReadNormalMap(domImageRef lib)
{
    if (lib==NULL || lib->getId()==NULL)
        return NULL;

    MapTexture::iterator it = _mapNormalMap.find(lib->getID());
    if (it != _mapNormalMap.end()) // if already exist
        return &it->second;

    domImage *imageElement = (domImage*)(domElement*)lib;
    if (imageElement)
    {
        Utils::FileName f = cdom::uriToNativePath(imageElement->getInit_from()->getValue().str());
        f.SetShortFilename(f.ShortFilename() + "Normal");
        if (!f.IsReadable())
            return NULL;
        LOG << "Create new NormalMap " << f << std::endl;
        GL::Texture *texture = &_mapNormalMap[imageElement->getId()];
        texture->LoadFromFile(f);
        return texture;
    }
    return NULL;
}

void    Plugin::ReadEffectLibrary(domLibrary_effectsRef lib)
{
    for (unsigned int i = 0; i < lib->getEffect_array().getCount(); i++)
        ReadEffect(lib->getEffect_array()[i]);
}

void    Plugin::ReadEffect(domEffectRef lib)
{
    if (lib->getId()==NULL)
        return;

    MapMaterialConfig::iterator it = _mapMaterialConfig.find(lib->getId());
    if (it != _mapMaterialConfig.end())   // material config is found
        return;

    LOG << "Create new Effect (MaterialConfig) " << lib->getId() << std::endl;
    MaterialConfig *config = &_mapMaterialConfig[lib->getId()];

    // Get a pointer to the effect element
    domEffect *EffectElement = (domEffect*)(domElement*)lib;
    if (EffectElement)
    {
        // even if we could have many profile, we take only the first
        // How many profiles are there
        int numProfiles = (int) EffectElement->getFx_profile_abstract_array().getCount();
        LOG << "There are " << numProfiles << " profiles and only one is supported" << std::endl;
        // Scan the profiles to find the profile_COMMON
        if (numProfiles > 0)
        //for (int p = 0; p < numProfiles;  p ++)
        {
            int p = 0;
            char *typeName = (char*)EffectElement->getFx_profile_abstract_array()[p]->getTypeName();
            if (strcmp("profile_COMMON", typeName) == 0)
            {
                // Found the common profile, get the technique from it as well
                domProfile_COMMON *common = (domProfile_COMMON*)(domFx_profile_abstract*)EffectElement->getFx_profile_abstract_array()[p];

                // get params
                domCommon_newparam_type_Array newparam_array = common->getNewparam_array();
                std::map<std::string, domCommon_newparam_type*> newParams;
                for (unsigned int i=0; i<newparam_array.getCount(); i++)
                    newParams[newparam_array[i]->getSid()] = newparam_array[i];

                // Get all images in profile_COMMON
                domProfile_COMMON::domTechnique *technique = common->getTechnique();
                if (technique != NULL)
                {
                    domProfile_COMMON::domTechnique::domConstant *constant = technique->getConstant();
                    if (constant)
                        ReadConstant(*config, newParams, constant);
                    domProfile_COMMON::domTechnique::domLambert *lambert = technique->getLambert();
                    if (lambert)
                        ReadLambert(*config, newParams, lambert);
                    domProfile_COMMON::domTechnique::domPhong *phong = technique->getPhong();
                    if (phong)
                        ReadPhong(*config, newParams, phong);
                    domProfile_COMMON::domTechnique::domBlinn *blinn = technique->getBlinn();
                    if(blinn)
                        ReadBlinn(*config, newParams, blinn);
                }
            }
            else
                LOG << typeName << " is not supported" << std::endl;
        }
    }
}

void Plugin::ReadConstant(MaterialConfig &config, std::map<std::string, domCommon_newparam_type*> &newParams, domProfile_COMMON::domTechnique::domConstant *constant)
{
    // TODO
}

void Plugin::ReadLambert(MaterialConfig &config, std::map<std::string, domCommon_newparam_type*> &newParams, domProfile_COMMON::domTechnique::domLambert *lambert)
{
    // TODO: take only the texture from diffuse for now
    if (lambert->getDiffuse())
    {
        ReadTextureFromTechniqueEffect(config, newParams, lambert->getDiffuse());
    }
}

void Plugin::ReadPhong(MaterialConfig &config, std::map<std::string, domCommon_newparam_type*> &newParams, domProfile_COMMON::domTechnique::domPhong *phong)
{
    // TODO: take only the texture from diffuse for now
    if (phong->getDiffuse())
    {
        ReadTextureFromTechniqueEffect(config, newParams, phong->getDiffuse());
    }
}

void Plugin::ReadBlinn(MaterialConfig &config, std::map<std::string, domCommon_newparam_type*> &newParams, domProfile_COMMON::domTechnique::domBlinn *blinn)
{
    // TODO: take only the texture from diffuse for now
    if (blinn->getDiffuse())
    {
        ReadTextureFromTechniqueEffect(config, newParams, blinn->getDiffuse());
    }
}

void Plugin::ReadTextureFromTechniqueEffect(MaterialConfig &config, std::map<std::string, domCommon_newparam_type*> &newParams, domCommon_color_or_texture_type *shader)
{
    domCommon_color_or_texture_type::domTexture *textureElement = shader->getTexture();
    if (textureElement != NULL)
    {
        std::string sampler2D_SID = textureElement->getTexture();
        if (newParams[sampler2D_SID] == 0)
        {
            xsIDREF idref(sampler2D_SID.c_str());
            idref.setContainer(shader);
            idref.resolveElement();
            domImage* image_element = (domImage*)(domElement*)idref.getElement();
            GL::Texture *t = ReadImage(image_element);
            GL::Texture *n = ReadNormalMap(image_element);
            unsigned int nbTextures = 0;
            if (t != NULL)
                nbTextures++;
            if (n != NULL)
                nbTextures++;
            config.Textures.InitSize(nbTextures);
            if (t != NULL)
                config.Textures[0] = *t;
            if (n != NULL)
                config.Textures[1] = *n;
        }
        else
        {
            std::string surface_SID = newParams[sampler2D_SID]->getSampler2D()->getSource()->getValue();
            if (newParams[surface_SID]->getSurface()->getFx_surface_init_common())
            {
                xsIDREF& idRef = newParams[surface_SID]->getSurface()->getFx_surface_init_common()->getInit_from_array()[0]->getValue();
                idRef.resolveElement();
                domImage* image_element = (domImage*)(domElement*) idRef.getElement();;
                GL::Texture *t = ReadImage(image_element);
                GL::Texture *n = ReadNormalMap(image_element);
                unsigned int nbTextures = 0;
                if (t != NULL)
                    nbTextures++;
                if (n != NULL)
                    nbTextures++;
                config.Textures.InitSize(nbTextures);
                if (t != NULL)
                config.Textures[0] = *t;
                if (n != NULL)
                    config.Textures[1] = *n;
            }
        }
    }
}

void Plugin::ReadMaterialLibrary(domLibrary_materialsRef lib)
{
    for (unsigned int i = 0; i < lib->getMaterial_array().getCount(); i++)
        ReadMaterial(lib->getMaterial_array()[i]);
}

void Plugin::ReadMaterial(domMaterialRef lib)
{
    if (lib->getId()==NULL)
        return;

    MapMaterial::iterator it = _mapMaterial.find(lib->getID());
    if (it != _mapMaterial.end())   // material is found
        return;

    domMaterial *MaterialElement = (domMaterial*)(domElement*)lib;
    if (MaterialElement)
    {
        if (!MaterialElement->getInstance_effect())
            return;

        domElement *element = (domElement*) MaterialElement->getInstance_effect()->getUrl().getElement();
        if (element == NULL || ((domEffect*)element)->getId() == NULL)
            return;

        LOG << "Attaching effect " << ((domEffect*)element)->getId() << " to Material " << lib->getId() << std::endl;
        _mapMaterial[lib->getId()] = ((domEffect*)element)->getId();
    }
}

void Plugin::ReadScene(domVisual_sceneRef scene)
{
    LOG << "Reading Collada Scene " << scene->getName() << std::endl;

    // recurse through the scene, read and add nodes
    _root = (scene->getNode_array().getCount() > 1) ? new Object() : NULL;
    for (unsigned int i = 0; i < scene->getNode_array().getCount(); i++)
    {
        ReadNode(scene->getNode_array()[i], _root);
    }

/*
    // for each skin controller find the nodes needed for skinning
    for (int i=0; i<Controllers.size(); i++)
    {       // for each controller
        CrtController * controller = Controllers[i];
        if (controller->type==CrtController_Skin_Types)
        {
            CrtSkin * skin = (CrtSkin *) controller;
            if (skin->JointsType == CrtJoint::CrtJointsType_SID)
            {       // bind joints using SID
                for(size_t j=0; j<Controllers[i]->skeletons.size(); j++)
                {       // for each skeletons
                        string skeleton_id = Controllers[i]->skeletons[j];
                        CrtNode * node = Nodes[skeleton_id];
                        if (node)
                                skin->FindJointNodes( node );
                }
            } else if (skin->JointsType == CrtJoint::CrtJointsType_ID)
            {       // bind joints using ID
                skin->FindJointNodes( SceneRoot );
            }
        }
    }
*/
}

void    Plugin::ReadNode(domNodeRef node, ISceneNode *parentNode)
{
    MapNode::iterator it = _mapNode.find(node->getId());
    if (it != _mapNode.end())
        return;

    LOG << "Reading Scene Node " << node->getId() << std::endl;

    Object *newNode = new Object(Box3f(), _globalMatrix);
    if (parentNode != NULL)
        parentNode->As<Object>()->AddChild(newNode);
    _mapNode[node->getId()] = newNode;

    // get back the matrix tranformation
    ReadNodeTranforms(newNode, node, (parentNode != NULL) ? parentNode->As<Object>() : NULL);

    // Process Instance Geometries
    for (unsigned int i = 0; i < node->getInstance_geometry_array().getCount(); i++)
    {
        domInstance_geometryRef lib = node->getInstance_geometry_array()[i];
        xsAnyURI &urltype  = lib->getUrl();
        urltype.resolveElement();
        domElement *element = (domElement*)urltype.getElement();
        if (element == NULL) // this instance geometry is not found skip to the next one
            continue;

        ReadDrawable((domGeometry*)element, newNode);
    }
    if (node->getInstance_geometry_array().getCount() > 0)
        newNode->ChooseDefaultMaterial();

/*
    // Process Instance Controllers
    for (int i = 0; i < node->getInstance_controller_array().getCount(); i++)
    {
        domInstance_controller *icontroller  = node->getInstance_controller_array()[i];
        CrtInstanceController * instanceController = ReadInstanceController(icontroller);
        if (instanceController==NULL) //if instance Controller can not be created, skip to the next one
            continue;

        instanceController->Parent = crtNode;
        crtNode->InstanceControllers.push_back(instanceController);
        ControllerInstances.push_back(instanceController);
    }
*/

    // read childrens
    for (unsigned int i = 0; i < node->getNode_array().getCount(); i++)
        ReadNode(node->getNode_array()[i], newNode);

    // read children <instance_nodes>
    for (unsigned int i = 0; i < node->getInstance_node_array().getCount(); i++)
    {
        domInstance_node *instance_node = node->getInstance_node_array()[i];
        domNode *urlnode = (domNode*) (domElement*) instance_node->getUrl().getElement();
        if (urlnode)
            ReadNode(urlnode, newNode);
    }
}

void    Plugin::ReadDrawable(domGeometry *lib, Object *mesh)
{
    if (lib->getId() == NULL)
        return;

    // See what kind of data is in the geometry (there can be only one)
    domAsset            *assetElement           = lib->getAsset();
    domMesh             *meshElement            = lib->getMesh();
    domConvex_mesh      *convexMeshElement      = lib->getConvex_mesh();
    domExtra_Array      &extraElement           = lib->getExtra_array();
    domSpline           *splineElement          = lib->getSpline();

    if(assetElement)
        LOG << "Item " << lib->getId() << " has an <asset> which RT doesn't support reading, skipped it." << std::endl;
    else if (convexMeshElement)
        LOG << "Item " << lib->getId() << " has an <convex_mesh> which RT doesn't support reading, skipped it." << std::endl;
    else if (extraElement.getCount())
        LOG << "Item " << lib->getId() << " has an <extra> which RT doesn't support reading, skipped it." << std::endl;
    else if (splineElement)
        LOG << "Item " << lib->getId() << " has an <spline> which RT doesn't support reading, skipped it." << std::endl;
    if (meshElement == NULL)
        return;

    ParseGeometry(lib, mesh);
};

void Plugin::ParseGeometry(domGeometry *dom_geometry, Object *mesh)
{
    domMesh *meshElement = dom_geometry->getMesh();
/*
    newGeo->SetName(dom_geometry->getId());
    newGeo->SetDocURI(dom_geometry->getDocumentURI()->getURI());
*/
    //not sure if we should get primitives by groups or by whatever comes first, I think it shouldn't matter, let's confirm later.
    int numPolylistGroups = (int)meshElement->getPolylist_array().getCount();
    if (numPolylistGroups != 0)
        LOG << "there are " << numPolylistGroups << " PolylistGroups, but the reader does not supported this type of geometry" << std::endl;
/*
    for (int i=0; i< numPolylistGroups; i++)
    {
        CrtPolyGroup *newprimitives = BuildPolygons(meshElement->getPolylist_array()[i], newGeo);
        newGeo->Groups.push_back(newprimitives);
    }
*/
    int numPolygonGroups = (int)meshElement->getPolygons_array().getCount();
    if (numPolygonGroups != 0)
        LOG << "there are " << numPolygonGroups << " PolygonGroups, but the reader does not supported this type of geometry" << std::endl;
/*
    for (int i=0; i< numPolygonGroups; i++)
    {
        CrtPolyGroup *newprimitives = BuildPolygons(meshElement->getPolygons_array()[i], newGeo);
        newGeo->Groups.push_back(newprimitives);
    }
*/
    int numTriangleGroups = (int)meshElement->getTriangles_array().getCount();
    for (int i=0; i< numTriangleGroups; i++)
    {
        domTriangles *domTriangles = meshElement->getTriangles_array()[i];

        MapDrawable::iterator it = _mapDrawable.find(domTriangles);
        if (it != _mapDrawable.end())   // drawable is found
        {
            //mesh->AddDrawable(*(it->second));
            mesh->Drawables()[i] = it->second; // init le nombre de drawable
            continue;
        }
        LOG << "Create new Triangles Drawable `" << domTriangles->getName() << "`" << std::endl;

        _mapDrawable[domTriangles];
        Drawable *newDrawable = _mapDrawable[domTriangles]; // creation du drawable
        Box3f b = BuildTriangles(domTriangles, newDrawable);
        mesh->SetBox(mesh->GetBox() + b);
        mesh->Drawables().push_back(newDrawable);
    }

    int numTriStripsGroups = (int)meshElement->getTristrips_array().getCount();
    if (numTriStripsGroups != 0)
        LOG << "there are " << numTriStripsGroups << " TriStripsGroups, but the reader does not supported this type of geometry" << std::endl;
/*
    for (int i=0; i< numTriStripsGroups ; i++)
    {
        CrtPolyGroup *newprimitives = BuildTriStrips(meshElement->getTristrips_array()[i], newGeo);
        newGeo->Groups.push_back(newprimitives);
    }
*/
    int numTriFansGroups = (int)meshElement->getTrifans_array().getCount();
    if (numTriFansGroups != 0)
        LOG << "there are " << numTriFansGroups << " TriFansGroups, but the reader does not supported this type of geometry" << std::endl;
/*
    for (int i=0; i< numTriFansGroups ; i++)
    {
        CrtPolyGroup *newprimitives = BuildTriFans(meshElement->getTrifans_array()[i], newGeo);
        newGeo->Groups.push_back(newprimitives);
    }
*/
    int numLinesGroups = (int)meshElement->getLines_array().getCount();
    if (numLinesGroups != 0)
        LOG << "there are " << numLinesGroups << " LinesGroups, but the reader does not supported this type of geometry" << std::endl;
/*
    for (int i=0; i< numLinesGroups ; i++)
    {
        CrtPolyGroup *newprimitives = BuildLines(meshElement->getLines_array()[i], newGeo);
        newGeo->Groups.push_back(newprimitives);
    }
*/
    int numLineStripsGroups = (int)meshElement->getLinestrips_array().getCount();
    if (numLineStripsGroups != 0)
        LOG << "there are " << numLineStripsGroups << " LineStripsGroups, but the reader does not supported this type of geometry" << std::endl;
/*
    for (int i=0; i< numLineStripsGroups ; i++)
    {
        CrtPolyGroup *newprimitives = BuildLineStrips(meshElement->getLinestrips_array()[i], newGeo);
        newGeo->Groups.push_back(newprimitives);
    }
*/
}

Box3f Plugin::BuildTriangles(domTriangles *domTriangles, Drawable *&drawable)
{
    LOG << "triangle count: " << domTriangles->getCount() << std::endl;

    daeString str_material = domTriangles->getMaterial();
    MaterialConfig *config;
    if (str_material)
    {
        LOG << "Attaching material " << str_material << " to the drawable" << std::endl;
        config = _mapMaterialConfig[_mapMaterial[str_material]].Clone();
    }
    else
        config = new MaterialConfig();

    Array<DefaultVertexType::Textured>  vertices(domTriangles->getCount() * 3);
    Array<unsigned int>                 indices(domTriangles->getCount() * 3);
    Box3f                               box;

    // prepare data
    domInputLocalOffset_Array &inputs = domTriangles->getInput_array();

    int                 max_offset = 0;
    int                 position_offset = -1;
    int                 normal_offset = -1;
    int                 texture1_offset = -1;
    domListOfFloats     *position_floats = NULL;
    domListOfFloats     *normal_floats = NULL;
    domListOfFloats     *texture1_floats = NULL;

    // inputs with offsets
    for(unsigned int i = 0; i < inputs.getCount(); i++)
    {
        int thisoffset  = (unsigned int) inputs[i]->getOffset();
        if (max_offset < thisoffset)
            max_offset++;

        domSource *source = (domSource*)(domElement*)inputs[i]->getSource().getElement();
        if(strcmp("VERTEX", inputs[i]->getSemantic()) == 0)
        {
            position_offset = thisoffset;
        }
        else if(strcmp("NORMAL", inputs[i]->getSemantic()) == 0)
        {
            normal_offset = thisoffset;
            normal_floats = & source->getFloat_array()->getValue();
        }
        else if(strcmp("TEXCOORD", inputs[i]->getSemantic()) == 0)
        {
            texture1_offset = thisoffset;
            texture1_floats = & source->getFloat_array()->getValue();
        }
    }
    max_offset++;

    // inputs without offsets in vertices
    domMesh *mesh = (domMesh*)inputs[0]->getParentElement()->getParentElement();
    domVertices *v = mesh->getVertices();
    domInputLocal_Array& vertices_inputs = v->getInput_array();
    for(unsigned int i=0; i<vertices_inputs.getCount(); i++)
    {
        domSource * source = (domSource*) (domElement*) vertices_inputs[i]->getSource().getElement();
        if(strcmp("POSITION", vertices_inputs[i]->getSemantic()) == 0)
        {
            position_floats = & source->getFloat_array()->getValue();
        }
        else if(strcmp("NORMAL", vertices_inputs[i]->getSemantic()) == 0)
        {
            normal_floats = & source->getFloat_array()->getValue();
            normal_offset = position_offset;
        }
        else if(strcmp("TEXCOORD", vertices_inputs[i]->getSemantic()) == 0)
        {
            texture1_floats = & source->getFloat_array()->getValue();
            texture1_offset = position_offset;
        }
    }


    // set index, they all have the same index since we process deindexer conditioner
    domListOfUInts P = domTriangles->getP()->getValue();
    for (unsigned int ivertex=0; ivertex< domTriangles->getCount() * 3; ivertex++)
    {
        domUint index;
        index = P[ivertex * max_offset + position_offset];
        //geometry->SkinIndex[ivertex] = (int) index;

        vertices[ivertex].color[0] = 1;
        vertices[ivertex].color[1] = 1;
        vertices[ivertex].color[2] = 1;

        vertices[ivertex].coord[0] = (float)(*position_floats)[(size_t)index*3+0];
        vertices[ivertex].coord[1] = (float)(*position_floats)[(size_t)index*3+1];
        vertices[ivertex].coord[2] = (float)(*position_floats)[(size_t)index*3+2];

        if (normal_offset != -1)
        {
            index = P[ivertex*max_offset + normal_offset];
            vertices[ivertex].normal[0] = (float)(*normal_floats)[(size_t)index*3+0];
            vertices[ivertex].normal[1] = (float)(*normal_floats)[(size_t)index*3+1];
            vertices[ivertex].normal[2] = (float)(*normal_floats)[(size_t)index*3+2];
        }

        if (texture1_offset != -1)
        {
            index = P[ivertex*max_offset + texture1_offset];
            vertices[ivertex].texCoord[0] = (float)(*texture1_floats)[(size_t)index*2+0];
            vertices[ivertex].texCoord[1] = (float)(*texture1_floats)[(size_t)index*2+1];
        }
        indices[ivertex] = ivertex;

        box += Vector3f(vertices[ivertex].coord[0], vertices[ivertex].coord[1], vertices[ivertex].coord[2]);
    }

    drawable = new Drawable(vertices, GL::Enum::StreamDraw, indices, 3, GL::Enum::Triangles, config);
    return box;
}

void Plugin::ReadNodeTranforms(Object *node, domNodeRef domNode, Object *)
{
    // load the node transformations as they are to be able to
    // handle any matrix stack configurations independant of the tools
    for (unsigned int i = 0; i < domNode->getContents().getCount(); i++)
    {
        // get the component type string
        char *typeName = (char*)domNode->getContents()[i]->getTypeName();

        // set the matrix transformation with the good attributes
        if (strcmp(typeName, "rotate") == 0)
        {
            // load rotation
            domRotateRef rotateArray = (domRotate*)(domElement*)domNode->getContents()[i];

            if (rotateArray->getValue().getCount() != 4)
                LOG << "something wrong when we load the rotation matrix transformation" << std::endl;
            else
            {
                node->Matrix.AddRotation(Vector3f((float)rotateArray->getValue()[0], (float)rotateArray->getValue()[1], (float)rotateArray->getValue()[2]),
                                        (float)rotateArray->getValue()[3]);

                //transform->SetRotate( rot );

                // this will be used to bind to an animation later
                //sid = (CrtChar *)rotateArray->getSid();
                //if ( sid )
                //        transform->SetSid( sid );

                //crtNode->Transforms.push_back(transform);
            }
        }
        else if (strcmp(typeName, "translate") == 0)
        {
            // load translation
            domTranslateRef translateArray = (domTranslate*)(domElement*)domNode->getContents()[i];

            if (translateArray->getValue().getCount() != 3)
                LOG << "something wrong when we load the rotation matrix transformation" << std::endl;
            else
            {
                // get the transation data
                node->Matrix.AddTranslation((float)translateArray->getValue()[0], (float)translateArray->getValue()[1], (float)translateArray->getValue()[2]);
                //crtNode->Transforms.push_back(transform);
            }
        }
        else if (strcmp(typeName, "scale") == 0)
        {
            // load scale
            domScaleRef scaleArray = (domScale*)(domElement*)domNode->getContents()[i];

            if (scaleArray->getValue().getCount() != 3)
                LOG << "something wrong when we load the rotation matrix transformation" << std::endl;
            else
            {
                // get the rotation data
                node->Matrix.AddScale((float)scaleArray->getValue()[0], (float)scaleArray->getValue()[1], (float)scaleArray->getValue()[2]);
                //crtNode->Transforms.push_back(transform);
            }
        }
        else if (strcmp(typeName, "matrix") == 0)
        {
            domMatrixRef matrixArray = (domMatrix*)(domElement*)domNode->getContents()[i];

            TMatrix mat;
            for (int j = 0; j < 4; ++j)
                for (int k = 0; k < 4; ++k)
                    mat[j][k] = (float)matrixArray->getValue()[(j*4) + k];
            node->Matrix.AddTransformation(mat);
            //crtNode->Transforms.push_back(transform);
        }
/*
       for (unsigned int i=0; i<Animations.size(); i++)
        {
            CrtAnimation * anim = Animations[i];
            if (anim->HasFoundTarget())
                    continue;
            CrtBool found_target = CrtFalse;
            for (CrtUInt j=0; j<anim->Channels.size(); j++)
            {
                CrtChar * target_id = anim->Channels[j]->GetTargetID();
                CrtChar * target_sid = anim->Channels[j]->GetTargetSID();
                CrtChar * id = crtNode->GetId();
                if (target_id && target_sid && id && sid)
                {
                    if (CrtICmp(target_id, id) &&
                            CrtICmp(target_sid, sid))
                    {
                        anim->SetFoundTarget();
                        transform->SetAnimation(anim);
                        transform->AddChannelId(j);
                        found_target = CrtTrue;

                        crtNode->SetAnimation( anim );
                        // don't want to break, though there maybe be multiple anim targets
                        NumAnimatedNodes++;

                        break;
                    }
                }
            }
            if (found_target)
                    break;
        }
*/
    }
}

