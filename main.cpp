#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <climits>

using namespace std;

const int NUM_CITIES = 4;
enum City { RIVERSIDE, MOVAL, PERRIS, HEMET }; // Updated Moreno Valley to Moval to help with my matrix 

string cityToString(City city) {
    switch(city) {
        case RIVERSIDE: return "Riverside";
        case MOVAL: return "Moval"; // 
        case PERRIS: return "Perris";
        case HEMET: return "Hemet";
        default: return "Unknown";
    }
}

class CityConnections {
private:
    int distances[NUM_CITIES][NUM_CITIES];

public:
    CityConnections() {
        for (int i = 0; i < NUM_CITIES; ++i) {
            for (int j = 0; j < NUM_CITIES; ++j) {
                distances[i][j] = (i == j) ? 0 : INT_MAX;
            }
        }
    }

    void addPath(City from, City to, int distance) {
        distances[from][to] = distance;
        distances[to][from] = distance;
    }

    int getDistance(City from, City to) const {
        return distances[from][to];
    }
};

class Journey {
public:
    vector<City> cities;
    int totalDistance;

    Journey() : totalDistance(0) {}

    void addCity(City city, const CityConnections& connections) {
        if (!cities.empty()) {
            totalDistance += connections.getDistance(cities.back(), city);
        }
        cities.push_back(city);
    }

    void displayJourney() const {
        for (City city : cities) {
            cout << cityToString(city) << " -> ";
        }
        cout << "Total Distance: " << totalDistance << " miles" << endl;
    }
};

void generatePermutations(const vector<City>& cities, int start, vector<vector<City>>& allPermutations) {
    if (start >= cities.size()) {
        allPermutations.push_back(cities);
        return;
    }
    for (int i = start; i < cities.size(); ++i) {
        vector<City> new_permutation(cities);
        swap(new_permutation[start], new_permutation[i]);
        generatePermutations(new_permutation, start + 1, allPermutations);
    }
}

void printMatrix(const CityConnections& connections) {// my matrix
    cout << "Distance Matrix:" << endl;
    cout << "                  ";
    for (int i = 0; i < NUM_CITIES; ++i) {
        cout << i + 1 << "." << cityToString(static_cast<City>(i)) << ", ";
    }
    cout << endl;

    for (int i = 0; i < NUM_CITIES; ++i) {
        cout << i + 1 << "." << cityToString(static_cast<City>(i)) << ":   ";
        for (int j = 0; j < NUM_CITIES; ++j) {
            if (connections.getDistance(static_cast<City>(i), static_cast<City>(j)) == INT_MAX) {
                cout << "INF Miles   ";
            } else {
                cout << "(" << i + 1 << ", " << j + 1 << ", " << connections.getDistance(static_cast<City>(i), static_cast<City>(j)) << " Miles)   ";
            }
        }
        cout << endl;
    }
}

int main() {
    CityConnections cityMap;
    // Add distances between cities
    cityMap.addPath(RIVERSIDE, MOVAL, 16); 
    cityMap.addPath(RIVERSIDE, PERRIS, 24);
    cityMap.addPath(RIVERSIDE, HEMET, 33);
    cityMap.addPath(MOVAL, PERRIS, 18); 
    cityMap.addPath(MOVAL, HEMET, 26); 
    cityMap.addPath(PERRIS, HEMET, 30);

    // Print the distance matrix
    printMatrix(cityMap);

    vector<vector<City>> allPermutations;
    generatePermutations({MOVAL, PERRIS, HEMET}, 0, allPermutations);

    vector<Journey> allJourneys;
    for (auto& permutation : allPermutations) {
        Journey journey;
        journey.addCity(RIVERSIDE, cityMap);
        for (City city : permutation) {
            journey.addCity(city, cityMap);
        }
        allJourneys.push_back(journey);
    }
  

    sort(allJourneys.begin(), allJourneys.end(), [](const Journey& a, const Journey& b) {
        return a.totalDistance < b.totalDistance;
    });

    cout << "Enter 1 for the shortest path, 2 for the most low-cost trip, or 3 to see alternate routes: ";
    int choice;
    cin >> choice;
// i tried incorporating user input the ask what they prefer
    if (choice == 1) {
        cout << "The shortest pathh is: ";
        allJourneys.front().displayJourney();
    } else if (choice == 2) {
        cout << "The most low cost trip is: ";
        allJourneys.front().displayJourney();
    } else if (choice == 3) {
        cout << "Alternate routes you can take!:" << endl;
        for (int i = 1; i < allJourneys.size(); ++i) {
            cout << "Route " << i << ": ";
            allJourneys[i].displayJourney();
        }
    } else {
        cout << "Invalid choice. Please enter 1, 2, or 3." << endl;
    }

    return 0;
}
