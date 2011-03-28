
/*-----------------------------------------------------------------------------

	3dNovac File
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

    File Created At:        05/12/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/


//#include <lib3ds.h>

#include <lib3ds/camera.h>
#include <lib3ds/mesh.h>
#include <lib3ds/node.h>
#include <lib3ds/material.h>
#include <lib3ds/matrix.h>
#include <lib3ds/vector.h>
#include <lib3ds/light.h>

#include "File3ds.h"
#include <Nc/Graphics/3D/Object/Mesh.h>
#include <Nc/Graphics/3D/Material/LightingMaterial.h>

#ifndef	MIN
#define	MIN(a,b) ((a)<(b)?(a):(b))
#define	MAX(a,b) ((a)>(b)?(a):(b))
#endif

#include <Utils/Debug/OverloadAlloc.h>

using namespace std;
using namespace Nc;
using namespace Nc::Graphic;

Utils::FileName             File3ds::_fileName;
GL::LightingVertex          *File3ds::_tabVertices = NULL;
map<string, GL::Texture>    File3ds::_mapTexture;
ListPMesh                   File3ds::_listMesh;
Lib3dsFile                  *File3ds::_file = NULL;
bool                        File3ds::_first;

Object3d* File3ds::Load(const Utils::FileName& aFileName, unsigned int aFlags)
{
/// chargement du fichier .3ds
    if ((_file = lib3ds_file_load(aFileName.Fullname().c_str())) == NULL)
    {
        LOG << "Error: Loading 3DS file : '" << aFileName << "' failed." << std::endl;
        return (NULL);
    }
    _fileName = aFileName;

/// construction des nodes s'il n'en existe pas, en fonction de la liste des meshs
    if (!_file->nodes)
    {
        Lib3dsNode *node;
        for (Lib3dsMesh *mesh = _file->meshes; mesh != NULL; mesh = mesh->next)
        {
            node = lib3ds_node_new_object();
            strcpy(node->name, mesh->name);
            node->parent_id = LIB3DS_NO_PARENT;
            lib3ds_file_insert_node(_file, node);
        }
    }

#ifdef _DEBUG_3DS_FILE
    int nb_meshe = 0;
    for (Lib3dsMesh *m = _file->meshes; m != NULL; m = m->next)
        nb_meshe++;
    uLog::Log() << "nb mesh = " << nb_meshe << "\n";
#endif

/// creation des mesh de l'objet
    _first = true;
    _listMesh.clear();
    CreateMeshs(_file->meshes, aFlags);
    _mapTexture.erase(_mapTexture.begin(), _mapTexture.end());

/// creation de l'objet
    Object3d* NewObject = new Object3d();

    for (ListPMesh::iterator it = _listMesh.begin(); it != _listMesh.end(); ++it)
    {
        (*it)->SetParent(NewObject);
        NewObject->AddChild(*it);
    }

    lib3ds_file_free(_file);
    return NewObject;
}

void    File3ds::CreateMeshs(Lib3dsMesh *mesh, unsigned int flags)
{
    map<string, t_material*> mapMaterial;

    if (mesh == NULL)
        return;
    FillTabVertice(mesh);
    mapMaterial = ParseFaces(mesh);

    for (map<string, t_material*>::iterator it = mapMaterial.begin();
         it != mapMaterial.end(); it++)
    {
        NewMesh(mesh, it->second, flags);
        delete (it->second);
    }
    delete[] (_tabVertices);
    CreateMeshs(mesh->next, flags);
}

void    File3ds::NewMesh(Lib3dsMesh *mesh, t_material *mat, unsigned int flags)
{
// creation du vertex buffer object
    int i = 0;
    GL::LightingVertex *tabVertice = new GL::LightingVertex[mat->listVertex.size()];
    for (list<GL::LightingVertex*>::iterator it = mat->listVertex.begin(); it != mat->listVertex.end(); it++, i++)
    {
        tabVertice[i] = *(*it);
        mat->box += (*it)->coord;
    }
    GL::VertexBuffer<GL::LightingVertex> VBO(GL::LightingVertex::GetDescriptor(), tabVertice, mat->listVertex.size(), flags);
    delete[] tabVertice;

// creation de l'index buffer object
    i = 0;
    unsigned int *tabIndice = new unsigned int[mat->listIndice.size()];
    for (list<unsigned int>::iterator it = mat->listIndice.begin(); it != mat->listIndice.end(); it++, i++)
        tabIndice[i] = *it;
    GL::IndexBuffer IBO(tabIndice, mat->listIndice.size() / 3, 3, mat->listIndice.size(), GL_TRIANGLES);
    delete[] tabIndice;

// creation du mesh
    if (VBO.IsValid() && IBO.IsValid())
    {
        Mesh *g = new Mesh(&VBO, &IBO, mat->box);
        g->SetMaterial(&Utils::Singleton<LightingMaterial>::Instance());
        LightingMaterialConfig *config = static_cast<LightingMaterialConfig*>(g->GetMaterialConfig());
        config->Texture = mat->texture;
        config->NormalMap = mat->normalMap;

        for (int k = 0; k < 4; k++)
            for (int l = 0; l < 4; l++)
                g->Matrix[k][l] = mesh->matrix[k][l];
        _listMesh.push_back(g);
    }
}

map<string, File3ds::t_material*> File3ds::ParseFaces(Lib3dsMesh *mesh)
{
    map<string, t_material*> mapMaterial;
    list<GL::LightingVertex*>::iterator it;
    int j, k, indice[3];
    t_material *mat = 0;
    Lib3dsFace f;

    // parcour toutes les faces pour calculer l'espace local, ainsi que pour construire la liste de vertex et d'indice
    for (unsigned int i = 0; i < mesh->faces; i++)
    {
        f = mesh->faceL[i];
        mat = GetMaterial(mapMaterial, f);

        Graphic::ComputeLocalSpace(_tabVertices[f.points[0]], _tabVertices[f.points[1]], _tabVertices[f.points[2]]);

        indice[0] = indice[1] = indice[2] = -1;
        for (j = 0, it = mat->listVertex.begin(); it != mat->listVertex.end(); it++, j++)
            for (k = 0; k < 3; k++)
                if (*it == _tabVertices + f.points[k])
                    indice[k] = j;
        for (k = 0; k < 3; k++)
        {
            if (indice[k] < 0)
            {
                indice[k] = mat->listVertex.size();
                mat->listVertex.push_back(_tabVertices + f.points[k]);
            }
            mat->listIndice.push_back(indice[k]);
        }
    }
    // normalise les vecteurs de binormal, tangente et normal
    for (unsigned int i = 0; i < mesh->points; i++)
    {
        Math::Normalize(_tabVertices[i].norm);
        Math::Normalize(_tabVertices[i].tan);
        Math::Normalize(_tabVertices[i].bin);
    }
    return mapMaterial;
}

File3ds::t_material *File3ds::GetMaterial(std::map<std::string, t_material*> &mapMaterial, Lib3dsFace &f)
{
    t_material *material;
    Lib3dsMaterial *mat = NULL;
    const char *name;

    if (f.material[0])
    {
        mat = lib3ds_file_material_by_name(_file, f.material);
        name = mat->name;
    }
    else
        name = "NO_MATERIAL";
    // search material name in mapMaterial
    map<string, t_material*>::iterator it = mapMaterial.find(name);
    if (it != mapMaterial.end())
        material = it->second;
    else
    {
#ifdef _DEBUG_3DS_FILE
        uLog::Log() << "new material : ";
        if (mat != NULL)
            uLog::Log() << name;
        uLog::Log() << "\n";
#endif
        material = new t_material();
        material->texture = GetTexture(mat, material->normalMap);
        mapMaterial[name] = material;
    }
    return material;
}

GL::Texture File3ds::GetTexture(Lib3dsMaterial *mat, GL::Texture &normalMap)
{
    GL::Texture texture;

    if (mat != NULL && mat->texture1_map.name != NULL &&
        strlen(mat->texture1_map.name) > 0)
    {
        char texname[1024];
        sprintf(texname, "%s%s", _fileName.Path().c_str(), mat->texture1_map.name);
        map<string, GL::Texture>::iterator it = _mapTexture.find(texname);
        if (it != _mapTexture.end())
            texture = it->second;
        else
        {
            try
            {
                texture.LoadFromFile(texname);
                Utils::FileName f(texname);
                f.SetFilename("normal.png");
                if (f.IsReadable())
                    normalMap.LoadFromFile(f.Fullname());
                _mapTexture[texname] = texture;
            }
            catch (const std::exception &e)
            {
                LOG << "Error: " << e.what() << std::endl;
            }
        }
    }
    return texture;
}

void    File3ds::FillTabVertice(Lib3dsMesh *mesh)
{
    unsigned int i = 0;

    _tabVertices = new GL::LightingVertex[mesh->points];
    while (i < mesh->points)
    {
        // set le vertice
        _tabVertices[i].coord[0] = mesh->pointL[i].pos[0];
        _tabVertices[i].coord[1] = mesh->pointL[i].pos[1];
        _tabVertices[i].coord[2] = mesh->pointL[i].pos[2];

        if (i < mesh->texels)
        {
            _tabVertices[i].texture[0] = mesh->texelL[i][0];
            _tabVertices[i].texture[1] = mesh->texelL[i][1];
        }
        else
        {
            _tabVertices[i].texture[0] = 0;
            _tabVertices[i].texture[1] = 0;
        }

        _tabVertices[i].color[0] = 255;
        _tabVertices[i].color[1] = 255;
        _tabVertices[i].color[2] = 255;

        _tabVertices[i].norm[0] = 0;
        _tabVertices[i].norm[1] = 0;
        _tabVertices[i].norm[2] = 0;
        _tabVertices[i].tan[0] = 0;
        _tabVertices[i].tan[1] = 0;
        _tabVertices[i].tan[2] = 0;
        _tabVertices[i].bin[0] = 0;
        _tabVertices[i].bin[1] = 0;
        _tabVertices[i].bin[2] = 0;
        i++;
    }
}

