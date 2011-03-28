
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

#include <fstream>

#include <Nc/Graphics/3D/Object/Mesh.h>
#include "FileObj.h"
#include <Utils/Debug/OverloadAlloc.h>

using namespace std;
using namespace Nc;
using namespace Nc::Graphic;
using namespace Nc::GL;

Vector3f*        FileObj::_tabCoordVertices = NULL;
Vector3f*        FileObj::_tabCoordNorms = NULL;
Vector2f*        FileObj::_tabCoordTexs = NULL;
unsigned int**   FileObj::_tabIndices = NULL;
unsigned int     FileObj::_nbCoordVertices = 0;
unsigned int     FileObj::_nbCoordTextures = 0;
unsigned int     FileObj::_nbCoordNormals = 0;
unsigned int     FileObj::_size = 0;
unsigned int     FileObj::_stride = 0;
Color      FileObj::_color;

Object3d* FileObj::Load(const Utils::FileName& aFileName, Color color, unsigned int aFlags)
{
// Création des variables permettant la création de l'objet 3D
    GL::LightingVertex* TabVertices=NULL;
    unsigned int* TabIndices=NULL;
    unsigned int NbVertices=0;
    Texture texture, normalMap;

    // reset les attributs
    _size = _stride = 0;
    _tabCoordVertices = _tabCoordNorms = NULL;
    _tabCoordTexs = NULL;
    _tabIndices = NULL;
    _color = color;


// lecture du fichier
    if(!Read(aFileName, &TabVertices, &TabIndices, &NbVertices, texture, normalMap))
        return NULL;

// calcul de l'espace local
    for (unsigned int i = 0; i < _size; i++) // effectue le calcul sur l'ensemble des faces
        Graphic::ComputeLocalSpace(TabVertices[TabIndices[(i*_stride)]],
                                 TabVertices[TabIndices[(i*_stride)+1]],
                                 TabVertices[TabIndices[(i*_stride)+2]]);
    for (unsigned int i = 0; i < NbVertices; i++) // uniformise les vecteur, car un vertex peut etre double a cause des textures
        for (unsigned int j = 0; j < NbVertices; j++)
            if (i != j && ( TabVertices[i].coord[0] == TabVertices[j].coord[0] &&
                            TabVertices[i].coord[1] == TabVertices[j].coord[1] &&
                            TabVertices[i].coord[2] == TabVertices[j].coord[2]))
            {
                Math::AddVector3D(TabVertices[i].tan, TabVertices[j].tan);
                Math::AddVector3D(TabVertices[i].bin, TabVertices[j].bin);
                Math::AddVector3D(TabVertices[i].norm, TabVertices[j].norm);
            }
    Box3f box;
    for (unsigned int i = 0; i < NbVertices; i++)
    {
        Math::Normalize(TabVertices[i].tan);
        Math::Normalize(TabVertices[i].bin);
        Math::Normalize(TabVertices[i].norm);
        box += TabVertices[i].coord;
    }

    #ifdef _DEBUG_OBJ
        LOG<< "VERTICES :" << "\n";
        for(unsigned int i=0; i<NbVertices; i++)
            LOG<< TabVertices[i].X << " " << TabVertices[i].Y  << " " << TabVertices[i].Z  << "\t"
                        << TabVertices[i].Tx<< " " << TabVertices[i].Ty << "\t"
                        << TabVertices[i].Nx<< " " << TabVertices[i].Ny << " " << TabVertices[i].Nz <<"\n";
        LOG<< "INDICES :" << "\n";
        for(unsigned int i=0; i<_size; i++)
        {
            for(unsigned int j=0; j<_stride; j++)
                LOG<< TabIndices[(i*_stride)+j] << " ";
            LOG<< "\n";
        }
    #endif

    unsigned int DisplayType = 0;
    if (_stride == 2)
        DisplayType=GL_LINES;
    else if (_stride == 3)
        DisplayType=GL_TRIANGLES;
    else if (_stride == 4)
        DisplayType=GL_QUADS;
    else
        LOG<<"Impossible de definir un type d'affichage des polygones avec un nb de vertex de " << _stride << " par face, pour le chargement de " << aFileName << "\n";

// creation du mesh
    GL::VertexBuffer<GL::LightingVertex> VBO(GL::LightingVertex::GetDescriptor(), TabVertices, NbVertices, aFlags);
    IndexBuffer IBO(TabIndices, _size, _stride, _size*_stride, DisplayType);
    Mesh *mesh = new Mesh(&VBO, &IBO, box);
    mesh->SetMaterial(&Utils::Singleton<LightingMaterial>::Instance());
    LightingMaterialConfig *config = static_cast<LightingMaterialConfig*>(mesh->GetMaterialConfig());
    config->Texture = texture;
    config->NormalMap = normalMap;

    delete[] TabVertices;
    delete[] TabIndices;
    return mesh;
}

bool FileObj::Read(const Utils::FileName& aFileName, GL::LightingVertex** aTabVertices, unsigned int** aTabIndices, unsigned int* aNbVertices, Texture &tex, Texture &normalMap)
{
// ouverture du fichier en lecture seule
    ifstream Fichier(aFileName.Fullname().c_str(),ios::in);
    if(!Fichier)
    {
        LOG << "Impossible d'ouvrir le fichier " << aFileName << std::endl;
        return false;
    }

// lecture des nombres de sommets, de coord de texture, de normale et de faces
    Utils::FileName Mtlib(aFileName.Path());
    GetNbsAndMtlib(Fichier, Mtlib);
    _stride = 3; // TODO: il y avais un bug sur le stride, on le force a 3 point final :) Serieux il va faloir recoder tout ca proprement

// lecture du fichier .mtl et chargement de la texture associé
    if (!Mtlib.Filename().empty())
    {
        Utils::FileName FilenameTexture = ReadMtl(Mtlib);
        if(!FilenameTexture.Fullname().empty())
        {
            tex.LoadFromFile(FilenameTexture);
            if (tex.IsValid())
            {
                FilenameTexture.SetFilename("normal.png");
                if (FilenameTexture.IsReadable())
                    normalMap.LoadFromFile(FilenameTexture);
            }
        }
    }

// allocations dynamique des tableaux intermediaire qui contiendront les données du fichier obj
    if(!AllocDynamiqueTabsInterm())
        return false;
// referance chacune des coordonnées des sommets, des sommets de textures, et des sommets de normales, et remplit le tableau d'indice
    GetTabCoords3D(Fichier, _tabCoordVertices, _nbCoordVertices, "v");
    GetTabCoords3D(Fichier, _tabCoordNorms, _nbCoordNormals, "vn");
    GetTabCoords2D(Fichier, _tabCoordTexs, _nbCoordTextures, "vt");
    GetTabIndices(Fichier, _tabIndices, _size, _stride);
    Fichier.close();// on a toutes les infos, on ferme le fichier

// création d'une liste de vertice, pour pouvoir creer les vertices sans doublon au bon format
    (*aTabIndices) = new unsigned int[_size * _stride];      // allocation du tableau d'indices final
    list<GL::LightingVertex> ListVertex = CreateListVertex(aTabIndices);

// aprés avoir construit tout les vertices, on peut aloué le tableau de vertices et le recopier
    unsigned int i=0;
    (*aNbVertices) = ListVertex.size();
    (*aTabVertices) = new GL::LightingVertex[*aNbVertices];
    for(list<GL::LightingVertex>::iterator It = ListVertex.begin(); It != ListVertex.end(); It++, i++)
        (*aTabVertices)[i] = (*It);

// destruction des tableaux
    delete[] _tabCoordVertices;
    if(_nbCoordTextures>0)
        delete[] _tabCoordTexs;
    if(_nbCoordNormals>0)
        delete[] _tabCoordNorms;
    for(i=0; i<_size; i++)
        delete[] _tabIndices[i];
    delete[] _tabIndices;
    return true;
}

list<GL::LightingVertex>   FileObj::CreateListVertex(unsigned int** aTabIndices)
{
    list<GL::LightingVertex> ListVertex;
    unsigned int indice = 0;
    bool existe;
    for(unsigned int i=0; i<_size; i++) // parcour le tableau d'indice, pour pouvoir créer les vertex
        for(unsigned int j=0; j<_stride; j++)
        {
        // création du vertex intermediaire
            GL::LightingVertex VertexIntermediaire = CreateVertexIntermediaire(i, j);

        // recherche dans la liste de vertex si il existe deja, en recuperant l'indice
            list<GL::LightingVertex>::iterator It = ListVertex.begin();
            existe = false;
            for (unsigned int k = 0; It != ListVertex.end(); k++)
            {
                if ( VertexIntermediaire.coord[0] == (*It).coord[0] && VertexIntermediaire.coord[1] == (*It).coord[1] && VertexIntermediaire.coord[2] == (*It).coord[2] &&
                     VertexIntermediaire.texture[0] == (*It).texture[0] && VertexIntermediaire.texture[1] == (*It).texture[1])
                {
                    existe = true;
                    indice = k;
                    It = ListVertex.end();
                }
                else
                    It++;
            }
            if (!existe)// si le vertex n'exite pas, on l'ajoute a la liste
            {
                indice = ListVertex.size();
                ListVertex.push_back(VertexIntermediaire);
            }
            (*aTabIndices)[(i*_stride)+j] = indice;// ajout de l'indice dans le tableau d'indice
        }
    return ListVertex;
}

GL::LightingVertex FileObj::CreateVertexIntermediaire(unsigned int i, unsigned int j)
{
    GL::LightingVertex Vi;

    Vi.color[0] = _color.R;
    Vi.color[1] = _color.G;
    Vi.color[2] = _color.B;
    Vi.coord[0] = _tabCoordVertices[_tabIndices[i][(j*3)]][0];
    Vi.coord[1] = _tabCoordVertices[_tabIndices[i][(j*3)]][1];
    Vi.coord[2] = _tabCoordVertices[_tabIndices[i][(j*3)]][2];
    Vi.texture[0] = (_nbCoordTextures>0) ? _tabCoordTexs[_tabIndices[i][(j*3)+1]].Data[0] : 0;
    Vi.texture[1] = (_nbCoordTextures>0) ? _tabCoordTexs[_tabIndices[i][(j*3)+1]].Data[1] : 0;
    Vi.norm[0] = 0;
    Vi.norm[1] = 0;
    Vi.norm[2] = 0;
    Vi.tan[0] = 0;
    Vi.tan[1] = 0;
    Vi.tan[2] = 0;
    Vi.bin[0] = 0;
    Vi.bin[1] = 0;
    Vi.bin[2] = 0;
    return (Vi);
}

void FileObj::GetNbsAndMtlib(  ifstream& aFile, Utils::FileName& aMtlib)
{
    _nbCoordVertices = 0;
    _nbCoordTextures = 0;
    _nbCoordNormals = 0;
    _size = 0;
    _stride = 1;

    string C, C1;
    bool NbIndiceCompute=false;
    while (!aFile.eof()) // tant qu'on est pas a la fin du fichier
    {
    // on lit une ligne et on test si on a un v, vt, vn, ou f au debut. jusqu'a ce que l'on ait attein la fin du fichier
        getline(aFile,C);
        istringstream caract(C);
        getline(caract,C1,' '); // recupere la premiere chaine

    // tests
        if(C1 == "v")
            _nbCoordVertices++;
        else if(C1 == "vt")
            _nbCoordTextures++;
        else if(C1 == "vn")
            _nbCoordNormals++;
        else if(C1 == "f")
        {
            // recupere le Nb d'indices par face TODO : si il n'y a pas exactement le meme nb d'indice pour chaque face, cale ne fonctionnera pas, peut etre faudra il triangulariser le model ?
            if(!NbIndiceCompute)
            {
                NbIndiceCompute=true;
                istringstream caract1(C);
                while (getline(caract1,C,' ')) // compte le nombre d'espace
                    _stride++;
                _stride -= 2;     //  on compte 2 espace en trop
                _size++;
                if (!aFile.eof())
                    getline(aFile, C);
            }
            _size++;
        }
        else if(C1 == "mtllib")
        {
            getline(caract,C1,' ');  // recupere la deuxieme chaine
            string::size_type Pos = C1.find_first_of('\r');
            if(Pos != string::npos)
                C1.erase(Pos);
            aMtlib.Fullname(aMtlib.Fullname()+C1);
        }
    }
}

Utils::FileName FileObj::ReadMtl(Utils::FileName& aFile)
{
    Utils::FileName FilenameTex;
    string Filename;

// ouverture du fichier avec le chemin complet
    ifstream FileMtl(aFile.Fullname().c_str());
    if(!FileMtl)// gere les erreurs
    {
        LOG_ERROR << "impossible d'ouvrir le fichier mtl : " << aFile << std::endl;
        return FilenameTex;
    }

// lecture du fichier jusqu'au premier objet
    string C, C1;

    while (!FileMtl.eof())
    {
        getline(FileMtl,C);
        istringstream caract1(C);
        getline(caract1,C,' ');

        if (C == "newmtl")
        {
            bool x=false;
            while (!x && !FileMtl.eof())// tant que l'on et pas arrivé au nom du fichier ou a la fin
            {
                getline(FileMtl,C1);
                istringstream caract2(C1);
                getline(caract2,C1,'\t');
                getline(caract2, C1, ' ');
                if (C1.empty())
                    getline(caract2, C1, ' ');// on le fait 2 fois car il y a un espace en trop
                if(C1 == "map_Kd")
                {
                    x=true;
                    getline( caract2, C1, ' ' );// une 3eme fois pour avoir le nom du fichier de texture
                    std::string::size_type Pos = C1.find_first_of('\r');
                    if(Pos != string::npos)
                        C1.erase(Pos);
                    Filename = C1;
                }
            }
        }
    }

    FileMtl.close();
    if (!Filename.empty())
        FilenameTex.Fullname(aFile.Path() + Filename);
    return FilenameTex;
}

bool FileObj::AllocDynamiqueTabsInterm()
{
// tab_coord_vertice
    try
    {
       _tabCoordVertices = new Vector3f[_nbCoordVertices];
    }
    catch(const bad_alloc &)
    {
        LOG<< "erreur lors de l'allocation dynamique du tableau de coordonnées de vertices" << endl;
        delete (_tabCoordVertices);
        return false;
    }

// tab_coord_text
    if(_nbCoordTextures>0)
        try
        {
            _tabCoordTexs = new Vector2f[_nbCoordTextures];
        }
        catch(const bad_alloc &)
        {
            LOG_ERROR << "erreur lors de l'allocation dynamique du tableau de coordonnées de textur" << endl;
            delete (_tabCoordTexs);
            return false;
        }

// tab_coord_norm
    if(_nbCoordNormals>0)
        try
        {
           _tabCoordNorms = new Vector3f[_nbCoordNormals];
        }
        catch(const bad_alloc &)
        {
            LOG_ERROR << "erreur lors de l'allocation dynamique du tableau de coordonnées de normale" << endl;
            delete (_tabCoordNorms);
            return false;
        }

// tab_face
    try
    {
        _tabIndices = new unsigned int*[_size];
        for(unsigned int i=0; i<_size; i++)
        {
            _tabIndices[i] = new unsigned int[_stride*3];
        }
    }
    catch(const bad_alloc &)
    {
        LOG_ERROR << "erreur lors de l'allocation dynamique du tableau d'indice\nNbFaces = " << _size << "\tStride = " << _stride << endl;
        delete (_tabIndices);
        return false;
    }

    return true;
}

void FileObj::GetTabCoords3D(ifstream& aFile, Vector3f* TabCoord, unsigned int NbCoord, const string& ChaineDepart)
{
    aFile.clear();
    aFile.seekg(0,ios::beg);  // remise au debut du fichier

    float coord;
    string C;
    unsigned int i=0;
    while(i < NbCoord)
    {
        aFile >> C;

        // si on a un v, on enregistre les coordonnées et incremente i
        if (C == ChaineDepart)
        {
            aFile >> coord;
            TabCoord[i].Data[0] = coord;

            aFile >> coord;
            TabCoord[i].Data[1] = coord;

            aFile >> coord;
            TabCoord[i].Data[2] = coord;

            i++;
        }
        // si on a un quelque chose d'autre, on saute une ligne
        else
            getline(aFile, C);
    }
}

void FileObj::GetTabCoords2D(ifstream& aFile, Vector2f* TabCoord, unsigned int NbCoord, const string& ChaineDepart)
{
    aFile.clear();
    aFile.seekg(0,ios::beg);  // remise au debut du fichier

    float coord;
    string C;
    unsigned int i=0;
    while(i < NbCoord)
    {
        aFile >> C;

        // si on a un v, on enregistre les coordonnées et incremente i
        if (C == ChaineDepart)
        {
            aFile >> coord;
            TabCoord[i].Data[0] = coord;

            aFile >> coord;
            TabCoord[i].Data[1] = coord;

            i++;
        }
        // si on a un quelque chose d'autre, on saute une ligne
        else
            getline(aFile, C);
    }
}

void FileObj::GetTabIndices(ifstream& aFile, unsigned int** TabIndices, unsigned int NbFaces, unsigned int Stride)
{
        aFile.clear();
        aFile.seekg(0,ios::beg); // remise a zero du fichier

        string C, V, Vt;
        unsigned int IV, IVt, IVn;
        unsigned int i=0, j=0;
        while(i < NbFaces)
        {
            aFile >> C;

            if (C == "f")
            {
                for(j=0; j<Stride; j++)
                {
                    aFile >> C;

                    V = C.substr(0, C.find_first_of('/'));
                    if(!V.empty())
                    {
                        istringstream v(V);
                        v>>IV;
                    }
                    else
                        IV=1;

                    C = C.substr(C.find_first_of('/')+1);

                    Vt = C.substr(0,C.find_first_of('/'));
                    if(!Vt.empty())
                    {
                        istringstream vt(Vt);
                        vt>>IVt;
                    }
                    else
                        IVt=1;

                    C = C.substr(C.find_first_of('/')+1);
                    if(!C.empty())
                    {
                        istringstream vn(C);
                        vn>>IVn;
                    }
                    else
                        IVn=1;

                    TabIndices[i][(j*3)] = IV-1;
                    TabIndices[i][(j*3)+1] = IVt-1;
                    TabIndices[i][(j*3)+2] = IVn-1;

                }
                i++;
            }
            else // si on a un quelque chose d'autre, on saute une ligne
                getline(aFile, C);
        }
}
