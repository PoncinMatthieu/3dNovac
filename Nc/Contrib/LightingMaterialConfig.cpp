/*
	3dNovac Graphics
	Copyright (C) 2010, Poncin Matthieu
	All rights reserved.

	This work is licensed under the Creative Commons
	Attribution-Share Alike 2.0 France License.
	To view a copy of this license, visit :
	http://creativecommons.org/licenses/by-sa/2.0/fr/
*/

#include "LightingMaterialConfig.h"

using namespace Nc::Graphic;
using namespace Nc::GL;

const VertexDescriptor   &LightingVertex::GetDescriptor()
{
    static VertexDescriptor desc(6);
// fonctionne pas sur le compilo de visual, la loose....
	desc.Data[0] = {VertexData, 3, GL_FLOAT, 0, 0, false};
    desc.Data[1] = {TexCoordData, 2, GL_FLOAT, 3 * sizeof(float), 0, false};
    desc.Data[2] = {ColorData, 3, GL_UNSIGNED_BYTE, 5 * sizeof(float), 0, false};
    desc.Data[3] = {AttribData, 3, GL_FLOAT, (5 * sizeof(float)) + (3 * sizeof(unsigned char)), 0, false};
    desc.Data[4] = {AttribData, 3, GL_FLOAT, (8 * sizeof(float)) + (3 * sizeof(unsigned char)), 0, false};
    desc.Data[5] = {AttribData, 3, GL_FLOAT, (11 * sizeof(float)) + (3 * sizeof(unsigned char)), 0, false};
/*
	desc.Data[0].DataType = VertexData;
	desc.Data[0].Size = 3;
	desc.Data[0].Type = GL_FLOAT;
	desc.Data[0].PointerOffset = 0;
	desc.Data[0].IndexAttrib = 0;
	desc.Data[0].Normalized = false;

	desc.Data[0].DataType = TexCoordData;
	desc.Data[0].Size = 2;
	desc.Data[0].Type = GL_FLOAT;
	desc.Data[0].PointerOffset = 3 * sizeof(float);
	desc.Data[0].IndexAttrib = 0;
	desc.Data[0].Normalized = false;

	desc.Data[0].DataType = ColorData;
	desc.Data[0].Size = 3;
	desc.Data[0].Type = GL_UNSIGNED_BYTE;
	desc.Data[0].PointerOffset = 5 * sizeof(float);
	desc.Data[0].IndexAttrib = 0;
	desc.Data[0].Normalized = false;

	desc.Data[0].DataType = AttribData;
	desc.Data[0].Size = 3;
	desc.Data[0].Type = GL_FLOAT;
	desc.Data[0].PointerOffset = (5 * sizeof(float)) + (3 * sizeof(unsigned char));
	desc.Data[0].IndexAttrib = 0;
	desc.Data[0].Normalized = false;

	desc.Data[0].DataType = AttribData;
	desc.Data[0].Size = 3;
	desc.Data[0].Type = GL_FLOAT;
	desc.Data[0].PointerOffset = (8 * sizeof(float)) + (3 * sizeof(unsigned char));
	desc.Data[0].IndexAttrib = 0;
	desc.Data[0].Normalized = false;

	desc.Data[0].DataType = AttribData;
	desc.Data[0].Size = 3;
	desc.Data[0].Type = GL_FLOAT;
	desc.Data[0].PointerOffset = (11 * sizeof(float)) + (3 * sizeof(unsigned char));
	desc.Data[0].IndexAttrib = 0;
	desc.Data[0].Normalized = false;
*/
	return desc;
}

namespace Nc
{
namespace Graphic
{
void ComputeLocalSpace(GL::LightingVertex &v1, GL::LightingVertex &v2, GL::LightingVertex &v3)
{
	// on calcule 2 vecteurs formant les bords du triangle
	Vector3f side0 = Vector3f(v2.coord) - Vector3f(v1.coord);	// vecteur allant du vertex 1 au vertex 2
	Vector3f side1 = Vector3f(v3.coord) - Vector3f(v2.coord);	// vecteur allant du vertex 1 au vertex 3

	// calcule la normale à la face
	Vector3f normal = side0.VectorProduct(side1);
	normal.Normalize();
/*
	// maintenant on calcule les coefficients des tangentes
	// ces coefficients sont calcules en fonction des coordonnées de textures du polygone
	float deltaT0 = v2.texture[0] - v1.texture[0];
	float deltaT1 = v3.texture[0] - v1.texture[0];
	float deltaB0 = v2.texture[1] - v1.texture[1];	// on effectue la meme chose pour la binormale
	float deltaB1 = v3.texture[1] - v1.texture[1];
	// le facteur permettant de rendre les tangentes et binormales proportionnelles aux coordonnees de textures.
	// on aura donc des vecteurs dont la norme depend des coordonnees de textures.
    float scale = 1.0 / ((deltaB0 * deltaT1) - (deltaB1 * deltaT0));
	mVector3D tangente = ((side0*deltaT1) - (side1*deltaT0))*scale;	// on calcule la tangente temporaire
	tangente.normalize();
	mVector3D binormal = ((side0*(-deltaB1)) + (side1*deltaB0))*scale;	// on calcule la binormale temporaire
	binormal.normalize();
*/

    Vector3f tangente = Vector3f(v1.coord[0], v1.coord[1], v1.coord[2]);
    tangente.Normalize();

    Vector3f binormal = tangente.VectorProduct(normal);

	// nous avons donc maintenant les tangentes, binormales et normales de la face.
	// on les additionne a celles deja presentes dans les tableaux
    AddVector3D(v1.tan, tangente);
    AddVector3D(v2.tan, tangente);
    AddVector3D(v3.tan, tangente);

    AddVector3D(v1.bin, binormal);
    AddVector3D(v2.bin, binormal);
    AddVector3D(v3.bin, binormal);

    AddVector3D(v1.norm, normal);
    AddVector3D(v2.norm, normal);
    AddVector3D(v3.norm, normal);
}
}
}
