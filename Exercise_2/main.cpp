#include <iostream>
#include "PolygonalMesh.hpp"
#include "Utils.hpp"

    using namespace std;
    using namespace Polygonal_Library;

    int main()
    {

        PolygonalMesh mesh;


        string filepath = "PolygonalMesh";

        double eps_scaling = 12;

        if(!ImportMesh(filepath,mesh,eps_scaling))
        {
            cerr << "An error occured when importing the Mesh" << endl;
            return 1;
        }
        cout << "The Mesh is valid!" << endl;
        return 0;
    }

