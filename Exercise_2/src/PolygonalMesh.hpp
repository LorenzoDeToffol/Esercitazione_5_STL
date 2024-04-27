#pragma once

#include <iostream>
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;

namespace Polygonal_Library{

struct PolygonalMesh
{
    //Cell0D
    unsigned int n0D = 0;
    vector<unsigned int> Id0D = {};
    vector<Vector2d> Coord0D = {};
    map<unsigned int, list<unsigned int>> Marker0D = {};

    //Cell1D
    unsigned int n1D = 0;
    vector<unsigned int> Id1D = {};
    vector<Vector2i> Vertices1D = {};  //1 lato 2 vertici
    map<unsigned int, list<unsigned int>> Marker1D = {};

    //Cell2D
    unsigned int n2D = 0;
    vector<unsigned int> Id2D = {};

    vector<VectorXi> Vertices2D = {}; //1 poligono X vetici
    vector<VectorXi> Edges2D = {};
};

}
