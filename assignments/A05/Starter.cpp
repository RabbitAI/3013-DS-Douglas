///////////////////////////////////////////////////////////////////////////////
//
// Date:             28 October 2019
// Title:            Program 3 - Graphs
// Semester:         Fall 2019
// Course:           CMPS 3013
// Assignment:       A05
// Author:           Christian Douglas
// Email:            cedoug13@aol.com
// Files:            cities.json, Geo.hpp, Json.hpp, JsonFacade.hpp
// Description:
//       The program takes a json file called cities with 1000 cities and assigns
//       each to a key in a struct called America by using hpp files called
//       Json.hpp and JsonFacade.hpp. JsonFacade uses code that allows Json to
//       read the cities json file and assign the values. In the main function
//       a 2d dynamic array called Array points to America which, using a for
//       loop gets assigned all of cities.json's values through a variable called
//       obj that takes values from JsonFacade's object J and its method getNext.
//       Another for loop with the total numer of cities is then ran taking the
//       top 3 closest cities of the initial city during each loop and puts them
//       in the Heap object H which is initialized to 1000 and false (Heap.hpp
//       was created by Dr. Griffin). For finding the distance, objects Geo1 and
//       Geo2, which are objects of the Coordinate class in the Geo.hpp file are
//       assigned to the initial city in the outer for loop's latitude and
//       longitude and each city that is being compareds latitude and longitude.
//       The HaversineDistance function in Geo.hpp is then called to get each
//       distance, which is then assigned to the priority key in the America
//       struct and inserted into the heap. Temp, which is another dynamic
//       pointer of America. The extracts the top 3 closest values and prints
//       the name of each city and the distance from the initial city.
//       The method EmptyHeap is then called, which deletes Array and resets
//       the pointer along with resetting heap size to 0 and next to 1.
/////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <iomanip>
#include "Heap.hpp"
#include "Geo.hpp"
#include "Json.hpp"
#include "JsonFacade.hpp"
#include <fstream>
#include <string>

using namespace std;
using json = nlohmann::json;

struct America
{
    string city_name;
    double growth;
    double latitude;
    double longitude;
    int population;
    int rank;
    string state_name;
    double Priority;

    America()
    {
        city_name = "";
        growth = 0.0;
        latitude = 0.0;
        longitude = 0.0;
        population = 0;
        rank = 0;
        state_name = "";
        Priority = 0;
    }

    America(json j)
    {
        city_name = j["city"];
        growth = j["growth_from_2000_to_2013"];
        latitude = j["latitude"];
        longitude = j["longitude"];
        population = j["population"];
        rank = j["rank"];
        state_name = j["state"];
    }
};

int main()
{
    America **Array;
    America *temp;
    json obj;
    string filename = "cities.json";
    JsonFacade J(filename);
    ofstream outfile;
    // create a heap that holds cities with priority = distance (smaller distance = higher priority)
    Heap<America> H(1000, false);
    int size = J.getSize();

    outfile.open("outfile.txt");
 
    Array = new America *[size];
    outfile << "Christian Douglas \n\n";
    // loop through cities
    for (int i = 0; i < size; i++)
    {
        obj = J.getNext();
        Array[i] = new America(obj);
    }
    // for each city, find closest cities by calculating distance and pushing onto a heap
    for (int i = 0; i < size; i++)
    {
        // get city1 lat and lon for distance calculation
        Coordinate geo1(Array[i]->latitude, Array[i]->longitude);
        // write out city1's name to output
        if (i < 10 || i > size - 11)
        {
            outfile << setprecision(4) << i + 1 << " : " << Array[i]->city_name << "\n";

            for (int j = 0; j < size; j++)
            {

                // get city2 lat and lon for distance calculation
                Coordinate geo2(Array[j]->latitude, Array[j]->longitude);
                if (Array[i]->city_name != Array[j]->city_name)
                {
                    // calculate distance from city1 to city2 and push that city onto  the heap (with its distance as priority)
                    Array[j]->Priority = HaversineDistance(geo1, geo2);
                    H.Insert(Array[j]);
                }
            }

            // extract x number of cities off the heap because a min-heap will give the x closest cities
            for (int j = 0; j < 3; j++)
            {
                temp = H.Extract();
                // write out each city from heap to output

                outfile << j + 1 << ") " << temp->city_name << " (" << temp->Priority << " km)\n";
            }
        }
        // empty your min heap
        H.emptyHeap();
    }
    outfile.close();
    return 0;
}
