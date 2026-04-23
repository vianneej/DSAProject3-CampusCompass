#pragma once
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <set>

using namespace std;

// Represents a connection between two campus locations
struct Edge {
    int to;              // destination location ID
    int weight;          // travel time/cost
    bool closed;         // whether this path is currently accessible
};

// Stores class info (name, location, timing)
struct ClassInformation {
    string name;
    int locationID;
    string start_time;
    string end_time;
};

// Stores student info (name, ID, residence, enrolled classes)
struct StudentInformation {
    string name;
    string ufid;
    int residenceID;
    set<string> classes;
};

// Main campus compass class that manages the graph, students, and commands
class CampusCompass {
private:
    unordered_map<int, vector<Edge>> graph;           // campus map as adjacency list
    unordered_map<string, ClassInformation> class_map; // class code to class info mapping
    unordered_map<string, StudentInformation> students; // student ID to student info mapping
    
    // Validation helpers
    bool isValidUFID(const string &ufid);
    bool isValidName(const string &name);
    bool isValidClassCode(const string &class_code);
    
    // Graph algorithms
    int dijkstra(int start, int target);
    bool dfs(int start, int target, unordered_set<int>& visited);
    
    // Edge management
    bool edgeExists(int start, int target);
    bool isEdgeClosed(int start, int target);
    bool toggleEdgde(int start, int target);
    
    // Student zone calculation
    int studentZoneCost(int residence, set<int>& classLocations);
   
public:
    CampusCompass(); 
    bool ParseCSV(const string &edges_filepath, const string &classes_filepath);
    bool ParseCommand(const string &command);
};
