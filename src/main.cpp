#include <iostream>
#include "CampusCompass.h"

using namespace std;

int main() {
    // Set up the campus compass app
    CampusCompass compass;

    // Load the campus data from CSV files
    compass.ParseCSV("data/edges.csv", "data/classes.csv");

    // Read commands and process them
    int no_of_lines;
    string command;
    cin >> no_of_lines;
    cin.ignore(); // skip the newline after reading the count
    
    for (int i = 0; i < no_of_lines; i++) {
        getline(cin, command);
        compass.ParseCommand(command);
    }
    
    return 0;
}
