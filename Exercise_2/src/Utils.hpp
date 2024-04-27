#pragma once

#include <iostream>
#include "PolygonalMesh.hpp"

using namespace std;

namespace Polygonal_Library {

bool ImportMesh(const string &filepath, PolygonalMesh& mesh, double& scaling);

//Nodi
bool ImportCell0Ds(const string &filename, PolygonalMesh& mesh);

//Vertici
bool ImportCell1Ds(const string &filename, PolygonalMesh& mesh);

//Poligoni
bool ImportCell2Ds(const string &filename, PolygonalMesh& mesh);

//Verifico la lunghezza dei dati
bool CheckLength(PolygonalMesh& mesh, double& scaling);

//Verifico la corrispondenza tra lati 1D e 2D
bool CheckVertices2D(PolygonalMesh& mesh);

//Verifico le aree dei poligoni
bool CheckArea(PolygonalMesh& mesh, double& scaling);

}
