#pragma once
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <set>

using namespace std;

struct Edge{
    int to;
    int weight;
    bool closed;
};
struct ClassInformation{
    string name;
    int locationID;
    string start_time;
    string end_time;
};
struct StudentInformation{
    string name;
    string ufid;
    int residenceID;
    set<string>classes;
};

class CampusCompass {
private:
    unordered_map<int, vector<Edge>> graph;
    unordered_map<string, ClassInformation> class_map;
    unordered_map<string, StudentInformation> students;
    
    bool isValidUFID(const string &ufid);
    bool isValidName(const string &name);
    bool isValidClassCode(const string &class_code);
    
    int dijkstra(int start, int target);
    bool dfs(int start, int target, unordered_set<int>& visited);
    bool edgeExists(int start, int target);
    bool isEdgeClosed(int start, int target);
    bool toggleEdgde(int start, int target);
   
public:
    CampusCompass(); 
    bool ParseCSV(const string &edges_filepath, const string &classes_filepath);
    bool ParseCommand(const string &command);
};
