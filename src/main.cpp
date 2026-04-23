#include <iostream>

#include "CampusCompass.h"

using namespace std;

int main() {
    CampusCompass compass;

    //CSV data
    compass.ParseCSV("data/edges.csv", "data/classes.csv");

    int no_of_lines;
    string command;
    cin >> no_of_lines;
    cin.ignore(); 
    for (int i = 0; i < no_of_lines; i++) {
        getline(cin, command);

        compass.ParseCommand(command);
    }
    return 0;
}
