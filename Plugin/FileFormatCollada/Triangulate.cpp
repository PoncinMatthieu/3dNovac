
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

    File Created At:        10/04/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include <Nc/Core/Utils/Utils.h>
#include "Triangulate.h"

using namespace Nc;

Triangulate::Triangulate(DAE *dae)
    : _dae(dae)
{
}

void Triangulate::Process()
{
    // How many geometry elements are there?
    int geometryElementCount = (int)(_dae->getDatabase()->getElementCount(NULL, "geometry"));

    LOG << "There are " << geometryElementCount << " geometry elements in this file" << std::endl;

    for(int currentGeometry = 0; currentGeometry < geometryElementCount; currentGeometry++)
    {
        // Find the next geometry element
        domGeometry *thisGeometry;
        //error = _dae->getDatabase()->getElement((daeElement**)&thisGeometry,currentGeometry, NULL, "geometry");
        daeElement * element = 0;
        _dae->getDatabase()->getElement(&element, currentGeometry, NULL, "geometry");
        thisGeometry = (domGeometry *) element;

        // Get the mesh out of the geometry
        domMesh *thisMesh = thisGeometry->getMesh();

        if (thisMesh == NULL)
            continue;

        // Loop over all the polygon elements
        for(int currentPolygons = 0; currentPolygons < (int)(thisMesh->getPolygons_array().getCount()); currentPolygons++)
        {
            // Get the polygons out of the mesh
            // Always get index 0 because every pass through this loop deletes the <polygons> element as it finishes with it
            domPolygons *thisPolygons = thisMesh->getPolygons_array()[currentPolygons];
            CreateTrianglesFromPolygons( thisMesh, thisPolygons );
            // Remove the polygons from the mesh
            //thisMesh->removeChildElement(thisPolygons);
        }
        while (thisMesh->getPolygons_array().getCount() > 0)
        {
            domPolygons *thisPolygons = thisMesh->getPolygons_array().get(0);
            thisMesh->removeChildElement(thisPolygons); // Remove the polygons from the mesh
        }
        int polylistElementCount = (int)(thisMesh->getPolylist_array().getCount());

        LOG << "There are " << polylistElementCount << " polylist elements in this file" << std::endl;

        for(int currentPolylist = 0; currentPolylist < polylistElementCount; currentPolylist++)
        {
            // Get the polylist out of the mesh
            // Always get index 0 because every pass through this loop deletes the <polygons> element as it finishes with it
            domPolylist *thisPolylist = thisMesh->getPolylist_array()[currentPolylist];
            CreateTrianglesFromPolylist(thisMesh, thisPolylist);
            // Remove the polylist from the mesh
            //thisMesh->removeChildElement(thisPolylist);
        }
        while (thisMesh->getPolylist_array().getCount() > 0)
        {
            domPolylist *thisPolylist = thisMesh->getPolylist_array().get(0);
            thisMesh->removeChildElement(thisPolylist); // Remove the polylist from the mesh
        }
    }
}

void Triangulate::CreateTrianglesFromPolygons(domMesh *thisMesh, domPolygons *thisPolygons)
{
    // Create a new <triangles> inside the mesh that has the same material as the <polygons>
    domTriangles *thisTriangles = (domTriangles *)thisMesh->createAndPlace("triangles");
    thisTriangles->setCount( 0 );
    thisTriangles->setMaterial(thisPolygons->getMaterial());
    domP* p_triangles = (domP*)thisTriangles->createAndPlace("p");

    // Give the new <triangles> the same <_dae> and <parameters> as the old <polygons>
    for(int i = 0; i < (int)(thisPolygons->getInput_array().getCount()); i++)
        thisTriangles->placeElement(thisPolygons->getInput_array()[i]->clone());

    // Get the number of inputs and primitives for the polygons array
    int numberOfInputs = (int)getMaxOffset(thisPolygons->getInput_array()) + 1;
    int numberOfPrimitives = (int)(thisPolygons->getP_array().getCount());

    // Triangulate all the primitives, this generates all the triangles in a single <p> element
    for(int j = 0; j < numberOfPrimitives; j++)
    {
        // Check the polygons for consistancy (some exported files have had the wrong number of indices)
        domP * thisPrimitive = thisPolygons->getP_array()[j];
        int elementCount = (int)(thisPrimitive->getValue().getCount());
        if((elementCount%numberOfInputs) != 0)
        {
            //cerr<<"Primitive "<<j<<" has an element count "<<elementCount<<" not divisible by the number of inputs "<<numberOfInputs<<"\n";
        }
        else
        {
            int triangleCount = (elementCount/numberOfInputs)-2;
            // Write out the primitives as triangles, just fan using the first element as the base
            int idx = numberOfInputs;
            for(int k = 0; k < triangleCount; k++)
            {
                // First vertex
                for(int l = 0; l < numberOfInputs; l++)
                {
                    p_triangles->getValue().append(thisPrimitive->getValue()[l]);
                }
                // Second vertex
                for(int l = 0; l < numberOfInputs; l++)
                {
                    p_triangles->getValue().append(thisPrimitive->getValue()[idx + l]);
                }
                // Third vertex
                idx += numberOfInputs;
                for(int l = 0; l < numberOfInputs; l++)
                {
                    p_triangles->getValue().append(thisPrimitive->getValue()[idx + l]);
                }
                thisTriangles->setCount(thisTriangles->getCount()+1);
            }
        }
    }
}

void    Triangulate::CreateTrianglesFromPolylist(domMesh *thisMesh, domPolylist *thisPolylist)
{
    // Create a new <triangles> inside the mesh that has the same material as the <polylist>
    domTriangles *thisTriangles = (domTriangles *)thisMesh->createAndPlace("triangles");
    //thisTriangles->setCount( 0 );
    unsigned int triangles = 0;
    thisTriangles->setMaterial(thisPolylist->getMaterial());
    domP* p_triangles = (domP*)thisTriangles->createAndPlace("p");

    // Give the new <triangles> the same <_dae> and <parameters> as the old <polylist>
    for(int i=0; i<(int)(thisPolylist->getInput_array().getCount()); i++)
        thisTriangles->placeElement( thisPolylist->getInput_array()[i]->clone() );

    // Get the number of inputs and primitives for the polygons array
    int numberOfInputs = (int)getMaxOffset(thisPolylist->getInput_array()) + 1;
    int numberOfPrimitives = (int)(thisPolylist->getVcount()->getValue().getCount());

    unsigned int offset = 0;

    // Triangulate all the primitives, this generates all the triangles in a single <p> element
    for(int j = 0; j < numberOfPrimitives; j++)
    {
        int triangleCount = (int)thisPolylist->getVcount()->getValue()[j] -2;
        // Write out the primitives as triangles, just fan using the first element as the base
        int idx = numberOfInputs;
        for(int k = 0; k < triangleCount; k++)
        {
            // First vertex
            for(int l = 0; l < numberOfInputs; l++)
            {
                p_triangles->getValue().append(thisPolylist->getP()->getValue()[offset + l]);
            }
            // Second vertex
            for(int l = 0; l < numberOfInputs; l++)
            {
                p_triangles->getValue().append(thisPolylist->getP()->getValue()[offset + idx + l]);
            }
            // Third vertex
            idx += numberOfInputs;
            for(int l = 0; l < numberOfInputs; l++)
            {
                p_triangles->getValue().append(thisPolylist->getP()->getValue()[offset + idx + l]);
            }
            //thisTriangles->setCount(thisTriangles->getCount()+1);
            triangles++;
        }
        offset += (unsigned int)thisPolylist->getVcount()->getValue()[j] * numberOfInputs;
    }
    thisTriangles->setCount(triangles);
}

unsigned int Triangulate::getMaxOffset(domInputLocalOffset_Array &input_array)
{
 	unsigned int maxOffset = 0;
    for (unsigned int i = 0; i < input_array.getCount(); i++)
    {
        if (input_array[i]->getOffset() > maxOffset)
            maxOffset = (unsigned int)input_array[i]->getOffset();
    }
	return maxOffset;
}
