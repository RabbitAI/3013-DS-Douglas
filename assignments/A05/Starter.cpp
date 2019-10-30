#include <iostream>
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
    double distance;

    America()
    {
        city_name = "";
        growth = 0.0;
        latitude = 0.0;
        longitude = 0.0;
        population = 0;
        rank = 0;
        state_name = "";
        distance = 0;
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

// create a heap that holds cities with priority = distance (smaller distance = higher priority)
int main()
{
    America **Array;
    America *temp;
    json obj;
    string filename = "cities.json";
    JsonFacade J(filename);
    ofstream outfile;
    Heap<America> H(1000, false);
    int size = J.getSize();

    outfile.open("outfile.txt");

    temp = H.Extract();  
    // loop through cities
     for (int i = 0; i < size; i++) {
        obj = J.getNext();
        Array[i] = new America(obj);
    }
    // for each city, find closest cities by calculating distance and pushing onto a heap
    for (int i = 0; i < size; i++)
    {
        // get city1 lat and lon for distance calculation
        Coordinate geo1(Array[i]->latitude, Array[i]->longitude);
        // write out city1's name to output
        outfile <<"Initial City: " << Array[i]->city_name << "\n";

        for (int j = 1; j < size; j++)
        {
            
            // get city2 lat and lon for distance calculation
            Coordinate geo2(Array[j]->latitude, Array[j]->longitude);
            // calculate distance from city1 to city2 and push that city onto  the heap (with its distance as priority)
            Array[j]->distance = HaversineDistance(geo1, geo2);
            H.Insert(Array[j]);
        }

        // extract x number of cities off the heap because a min-heap will give the x closest cities
        
        // write out each city from heap to output

        // empty your min heap
    }
    outfile.close();
    return 0;
}