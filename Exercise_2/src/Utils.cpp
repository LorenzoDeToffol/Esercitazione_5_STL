 #include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <limits>
#include <cmath>
#include<vector>

using namespace std;

namespace Polygonal_Library {



bool ImportMesh(const string& filepath, PolygonalMesh& mesh, double &scaling)
{   //Controllo sulle celle 0D
    if(!ImportCell0Ds(filepath + "/Cell0Ds.csv",mesh))
    {
        cerr << "Failed to import 0D cells!" << endl;
        return false;
    }
    else
    {
        //Test sui marker
        cout << "Test 1: "<< endl;
        cout << "Cell0D Markers:" << endl;
        for(auto i = mesh.Marker0D.begin(); i != mesh.Marker0D.end(); i++)
        {
            cout << "Key: " << i -> first << " Values:";
            for(const unsigned int id : i -> second)
            {
                cout << " " << id;
            }
            cout << endl;
        }
        cout << endl;
    }
    //Controllo sulle celle 1D
    if(!ImportCell1Ds(filepath + "/Cell1Ds.csv",mesh))
    {
        cerr << "Failed to import 1D cells!" << endl;
        return false;
    }
    else
    {
        cout << "Test 2: "<< endl;
        cout << "Cell1D Markers:" << endl;
        for(auto i = mesh.Marker1D.begin(); i != mesh.Marker1D.end(); i++)
        {
            cout << "Key: " << i -> first << " Values:";
            for(const unsigned int id : i -> second)
            {
                cout << " " << id;
            }
            cout << endl;
        }
        cout << endl;
    }

    //Test sulla lunghezza dei lati
    cout << "Test 3: ";
    if(!CheckLength(mesh, scaling))
    {
        return false;
    }
    cout << "No edge has length 0!" << endl;

    //Controllo sulle celle 2D
    if(!ImportCell2Ds(filepath + "/Cell2Ds.csv",mesh))
    {
        cerr << "Failed to import 2D cells!" << endl;
        return false;
    }

    //Test di corrispondenza tra lati
    cout << "Test 4: ";
    if(!CheckVertices2D(mesh))
    {
        return false;
    }
    cout << "The correspondance between 1D and 2D edges has been verified!" << endl;

    //Test sulle aree
    cout << "Test 5: ";
    if(!CheckArea(mesh, scaling))
    {
        return false;
    }
    cout << "The area of the polygons is non-zero!" << endl;

    return true;
}


bool ImportCell0Ds(const string &filename,PolygonalMesh& mesh)
{

    ifstream file;
    file.open(filename);
    if(file.fail())
    {
        cout << "Failed to open the file." << endl;
        return false;
    }

    string line;
    list<string> listLines;
       while (getline(file, line))
    {
        listLines.push_back(line);
    }

    //Tolgo la prima riga
    listLines.pop_front();

    //Salvo il numero di punti.
    mesh.n0D = listLines.size();

    //Verifico che ci fosse almeno un punto.
    if (mesh.n0D == 0)
    {
        cerr << "No 0D cell found." << endl;
        return false;
    }

    //Salvo della memoria
    mesh.Id0D.reserve(mesh.n0D);
    mesh.Coord0D.reserve(mesh.n0D);

    //Prendo i dati da ogni riga
    for (const string& line : listLines)
    {
        istringstream ss1(line);
        char temp1;
        unsigned int id;
        unsigned int marker;
        Vector2d coord;
        ss1 >> id >> temp1 >> marker >> temp1 >> coord(0) >> temp1 >> coord(1);

        mesh.Id0D.push_back(id);
        mesh.Coord0D.push_back(coord);

        //Controllo di avere un punto interno
        if(marker != 0)
        {
            auto ret = mesh.Marker0D.insert({marker, {id}});

            // Se la chiave esiste già, aggiungo il valore di ID.
            if(!ret.second)
            {
                (ret.first)->second.push_back(id);
            }
        }
    }
    file.close();

    return true;
}


bool ImportCell1Ds(const string &filename,PolygonalMesh& mesh)
{

    ifstream file;
    file.open(filename);
    if(file.fail())
    {
        cout << "Failed to open the file." << endl;
        return false;
    }


    list<string> listLines;
    string line;
    while (getline(file, line))
    {
        listLines.push_back(line);
    }

    listLines.pop_front();

    //Salvo il numero di lati.
    mesh.n1D = listLines.size();

    // Verifico che ci sia almeno un lato.
    if (mesh.n1D == 0)
    {
        cerr << "No 0D cell found." << endl;
        return false;
    }

    mesh.Id1D.reserve(mesh.n1D);
    mesh.Vertices1D.reserve(mesh.n1D);


    for (const string& line : listLines)
    {
        istringstream ss2(line);
        char temp2;
        unsigned int id;
        unsigned int marker;
        Vector2i vertices;
        ss2 >> id >> temp2 >> marker >> temp2 >> vertices(0) >> temp2 >> vertices(1);


        mesh.Id1D.push_back(id);
        mesh.Vertices1D.push_back(vertices);

        if(marker != 0)
        {
            auto ret = mesh.Marker1D.insert({marker, {id}});

            // Se la chiave esiste già, aggiungo il valore di ID.
            if(!ret.second)
            {
                (ret.first)->second.push_back(id);
            }
        }
    }

    file.close();

    return true;
}


bool CheckLength(PolygonalMesh& mesh, double &scaling)
{
    //Tolleranza epsilon
    double eps = numeric_limits<double>::epsilon()*scaling;

    // Leggo il vettore che contiene il numero dei vertici associati a ciascun lato.
    for(const auto& points : mesh.Vertices1D)
    {
        // Ricavo le coordinate di ciascun vertice e controllo la loro distanza.
        Vector2d coord_1 = mesh.Coord0D[points(0)];
        Vector2d coord_2 = mesh.Coord0D[points(1)];
        if(abs(coord_1(0)-coord_2(0)) < eps && abs(coord_1(1)-coord_2(1)) < eps)
        {
            cerr << "Edges of length 0 found!" << endl;
            return false;
        }
    }
    return true;
}


bool ImportCell2Ds(const string &filename,PolygonalMesh& mesh)
{

    ifstream file;
    file.open(filename);
    if(file.fail())
    {
        cout << "Failed to open the file." << endl;
        return false;
    }


    list<string> listLines;
    string line;
    while (getline(file, line))
    {
        listLines.push_back(line);
    }

    listLines.pop_front();

    //Salvo il numero di poligoni.
    mesh.n2D = listLines.size();


    if (mesh.n2D == 0)
    {
        cerr << "No 2D cells found!" << endl;
        return false;
    }

    mesh.Id2D.reserve(mesh.n2D);
    mesh.Vertices2D.reserve(mesh.n2D);
    mesh.Edges2D.reserve(mesh.n2D);


    for (const string& line : listLines)
    {
        istringstream ss3(line);
        string stringa;
        unsigned int id;
        unsigned int sizeVer;
        unsigned int sizeEdg;
        VectorXi vertices;
        VectorXi edges;

        getline(ss3,stringa,';');
        istringstream(stringa)>>id;

        getline(ss3,stringa,';'); //Qui i marker sono tutti uguali a zero, evito di memorizzarli

        //Ridimensiono il vettore dei vertici dopo aver letto quanti sono
        getline(ss3,stringa,';');
        istringstream(stringa)>>sizeVer;
        vertices.resize(sizeVer);

        //Salvo i vertici di ogni poligono
        for(unsigned int i=0; i<sizeVer; i++)
        {
            getline(ss3,stringa,';');
            istringstream(stringa)>>vertices(i);
        }

        //Ridimensiono il vettore dei lati
        getline(ss3,stringa,';');
        istringstream(stringa)>>sizeEdg;
        edges.resize(sizeEdg);

        // Memorizzo i lati di ogni poligono.
        for(unsigned int i=0; i<sizeEdg; i++)
        {
            getline(ss3,stringa,';');
            istringstream(stringa)>>edges(i);
        }

        mesh.Id2D.push_back(id);
        mesh.Vertices2D.push_back(vertices);
        mesh.Edges2D.push_back(edges);

    }

    file.close();
    return true;
}

bool CheckVertices2D(PolygonalMesh& mesh)
{
    for(const auto& vertices : mesh.Vertices2D)
    {
        //Flag
        bool found = false;

        //Leggo i vertici di ogni poligono
        for(unsigned int i = 0; i < vertices.size(); i++)
        {
            if(i != vertices.size() -1)
            {
                Vector2i to_find(vertices(i),vertices(i+1));
                for(const auto& vertices_couple : mesh.Vertices1D)
                {
                    Vector2i swapped(vertices_couple[1],vertices_couple[0]);
                    if(to_find == vertices_couple || to_find == swapped)
                    {
                        found = true;
                        break;
                    }
                }
                if(!found)
                {
                    cerr << "Missed correspondance between 2D and 1D!" << endl;
                    return false;
                }
            }

            else
            {
                Vector2i to_find(vertices(i),vertices(0));
                for(const auto& vertices_couple : mesh.Vertices1D)
                {
                    Vector2i swapped(vertices_couple[1],vertices_couple[0]);
                    if(to_find == vertices_couple || to_find == swapped)
                    {
                        found = true;
                        break;
                    }
                }
                if(!found)
                {
                    cerr << "Missed correspondance between 2D and 1D!" << endl;
                    return false;
                }
            }
        }
    }
    return true;
}

bool CheckArea(PolygonalMesh& mesh, double &scaling)
{

    double eps = numeric_limits<double>::epsilon()*scaling;

    for(const auto& edges : mesh.Edges2D)
    {
        //prendo i primi due punti del poligono e calcolo l'equazione della retta passante per i due punti
         Vector2i lato1 = mesh.Vertices1D[edges(0)];
         Vector2d P1 = mesh.Coord0D[lato1(0)];
         Vector2d P2 = mesh.Coord0D[lato1(1)];
         double m = (P2[1] - P1[1]) / (P2[0] - P1[0]);
         double q = P2[0] - m*P1[0];

         bool allineati = true; //vedo se i restanti punti sono allineati ai primi 2
         for (size_t i = 2; i < edges.size(); ++i) {
             Vector2i latoi = mesh.Vertices1D[edges(i)];
             Vector2d Pi = mesh.Coord0D[latoi(0)];

             double yRetta = m*Pi[0]+q;


             if (Pi[1] != yRetta) {
                 allineati = false;
                 break;
             }
         }

         if (allineati)
         {
             cout << "Area = 0 found!" << endl;
         }

    }
    return true;
}

}
