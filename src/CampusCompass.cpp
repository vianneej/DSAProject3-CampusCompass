#include "CampusCompass.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

using namespace std;

CampusCompass::CampusCompass() {
    // initialize your object
}

bool CampusCompass::ParseCSV(const string &edges_filepath, const string &classes_filepath) {
    ifstream edgesFile(edges_filepath);
    ifstream classesFile(classes_filepath);

    if (!edgesFile.is_open() || !classesFile.is_open()) {
        return false;
    }
    string line;

    // return boolean based on whether parsing was successful or not
    return true;
}

bool CampusCompass::ParseCommand(const string &command) {
    // do whatever regex you need to parse validity
    // hint: return a boolean for validation when testing. For example:
    bool is_valid = true; // replace with your actual validity checking

    return is_valid;
}
