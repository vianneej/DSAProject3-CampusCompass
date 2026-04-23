#include "CampusCompass.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <queue>
#include <limits>
#include <climits>
#include <unordered_set>
#include <map>

using namespace std;

// Constructor - initialize all data structures
CampusCompass::CampusCompass() {
    graph.clear();
    class_map.clear();
    students.clear();
}

// Load campus data from CSV files (edges and classes)
bool CampusCompass::ParseCSV(const string &edges_filepath, const string &classes_filepath) {
    ifstream edgesFile(edges_filepath);
    ifstream classesFile(classes_filepath);

    if (!edgesFile.is_open() || !classesFile.is_open()) {
        return false;
    }
    
    string line;

    // Read edges: location pairs with travel times
    getline(edgesFile, line); // skip header
    while (getline(edgesFile, line)) {
        stringstream ss(line);
        string id1, id2, name1, name2, time;

        getline(ss, id1, ',');
        getline(ss, id2, ',');
        getline(ss, name1, ',');
        getline(ss, name2, ',');
        getline(ss, time, ',');

        int u = stoi(id1);
        int v = stoi(id2);
        int w = stoi(time);

        // Add bidirectional edges
        graph[u].push_back({v, w, false});
        graph[v].push_back({u, w, false});
    }

    // Read classes: class codes mapped to locations and times
    getline(classesFile, line); // skip header
    while (getline(classesFile, line)) {
        stringstream ss(line);
        string code, loc, start, end;

        getline(ss, code, ',');
        getline(ss, loc, ',');
        getline(ss, start, ',');
        getline(ss, end, ',');

        class_map[code] = {code, stoi(loc), start, end};
    }

    return true;
}

// Validate UFID: must be exactly 8 digits
bool CampusCompass::isValidUFID(const string &ufid) {
    if (ufid.size() != 8) return false;
    for (char c : ufid) if (!isdigit(c)) return false;
    return true;
}

// Validate name: only letters and spaces allowed
bool CampusCompass::isValidName(const string &name) {
    for (char c : name) {
        if (!(isalpha(c) || c == ' ')) return false;
    }
    return true;
}

// Validate class code: 3 uppercase letters followed by 4 digits
bool CampusCompass::isValidClassCode(const string &class_code) {
    if (class_code.size() != 7) return false;
    for (int i = 0; i < 3; i++) if (!isupper(class_code[i])) return false;
    for (int i = 3; i < 7; i++) if (!isdigit(class_code[i])) return false;
    return true;
}

// DFS to check if there's a path between two nodes (ignoring closed edges)
bool CampusCompass::dfs(int start, int target, unordered_set<int>& visited) {
    if (start == target) return true;
    visited.insert(start);

    for (auto &e : graph[start]) {
        if (!e.closed && !visited.count(e.to)) {
            if (dfs(e.to, target, visited)) return true;
        }
    }
    return false;
}

// Dijkstra's algorithm: find shortest path from start to target
int CampusCompass::dijkstra(int start, int target) {
    unordered_map<int, int> dist;
    for (auto &p : graph) dist[p.first] = INT_MAX;

    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;
    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (u == target) return d;

        for (auto &e : graph[u]) {
            if (e.closed) continue; // skip closed edges
            if (d + e.weight < dist[e.to]) {
                dist[e.to] = d + e.weight;
                pq.push({dist[e.to], e.to});
            }
        }
    }
    return -1; // unreachable
}

// Check if an edge exists between two nodes
bool CampusCompass::edgeExists(int start, int target) {
    for (auto &e : graph[start]) {
        if (e.to == target) return true;
    }
    return false;
}

// Check if an edge is currently closed
bool CampusCompass::isEdgeClosed(int start, int target) {
    for (auto &e : graph[start]) {
        if (e.to == target) return e.closed;
    }
    return false;
}

// Toggle an edge's closure status (open/closed)
bool CampusCompass::toggleEdgde(int start, int target) {
    for (auto &e : graph[start]) {
        if (e.to == target) e.closed = !e.closed;
    }
    for (auto &e : graph[target]) {
        if (e.to == start) e.closed = !e.closed;
    }
    return true;
}

// Calculate the minimum cost to connect a student's residence to all their class locations
// Uses Dijkstra to find shortest paths, then builds an MST of those paths
int CampusCompass::studentZoneCost(int residence, set<int>& locations) {
    // Step 1: Find all vertices that appear in shortest paths to class locations
    unordered_set<int> allVertices;
    allVertices.insert(residence);
    
    // Run Dijkstra with parent tracking for path reconstruction
    unordered_map<int, int> parent;
    unordered_map<int, int> dist;
    
    for (auto &p : graph) dist[p.first] = INT_MAX;
    dist[residence] = 0;
    
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;
    pq.push({0, residence});
    
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        
        if (d > dist[u]) continue;
        
        for (auto &e : graph[u]) {
            if (e.closed) continue;
            if (d + e.weight < dist[e.to]) {
                dist[e.to] = d + e.weight;
                parent[e.to] = u;
                pq.push({dist[e.to], e.to});
            }
        }
    }
    
    // Backtrack through shortest paths to collect all intermediate vertices
    for (int loc : locations) {
        if (dist[loc] == INT_MAX) return -1; // Can't reach this class
        
        int current = loc;
        while (current != residence) {
            allVertices.insert(current);
            current = parent[current];
        }
    }
    
    // Step 2: Build MST using only vertices and edges from the shortest paths
    // This eliminates redundant edges and gives us the minimum spanning tree
    set<int> inMST;
    int totalCost = 0;
    inMST.insert(residence);
    
    while (inMST.size() < allVertices.size()) {
        int minEdgeWeight = INT_MAX;
        int nextNode = -1;
        
        // Find the cheapest edge from MST to a non-MST vertex
        for (int u : inMST) {
            for (auto &e : graph[u]) {
                if (!e.closed && allVertices.count(e.to) && !inMST.count(e.to)) {
                    if (e.weight < minEdgeWeight) {
                        minEdgeWeight = e.weight;
                        nextNode = e.to;
                    }
                }
            }
        }
        
        if (nextNode == -1) return -1; // Shouldn't happen if shortest paths exist
        
        inMST.insert(nextNode);
        totalCost += minEdgeWeight;
    }
    
    return totalCost;
}

// Main command parser - handles all student and campus management commands
bool CampusCompass::ParseCommand(const string &command) {
    stringstream ss(command);
    string cmd;
    ss >> cmd;

    // add a new student
    if (cmd == "insert") {
        // Extract name from quotes
        size_t q1 = command.find('"');
        size_t q2 = command.find('"', q1 + 1);

        if (q1 == string::npos || q2 == string::npos) {
            cout << "unsuccessful\n";
            return false;
        }

        string name = command.substr(q1 + 1, q2 - q1 - 1);
        string rest = command.substr(q2 + 1);
        stringstream rs(rest);

        string ufid;
        int residence, n;
        rs >> ufid >> residence >> n;

        vector<string> codes;
        string c;
        while (rs >> c) codes.push_back(c);

        // Validate all inputs
        if (!isValidUFID(ufid) || !isValidName(name) || codes.size() != n || n < 1 || n > 6) {
            cout << "unsuccessful\n";
            return false;
        }

        if (students.count(ufid)) { // UFID must be unique
            cout << "unsuccessful\n";
            return false;
        }

        // Validate all class codes
        for (auto &code : codes) {
            if (!isValidClassCode(code) || !class_map.count(code)) {
                cout << "unsuccessful\n";
                return false;
            }
        }

        // Add the student
        StudentInformation s;
        s.name = name;
        s.ufid = ufid;
        s.residenceID = residence;
        s.classes.insert(codes.begin(), codes.end());

        students[ufid] = s;

        cout << "successful\n";
        return true;
    }

    // delete a student
    else if (cmd == "remove") {
        string ufid;
        ss >> ufid;

        if (!students.count(ufid)) {
            cout << "unsuccessful\n";
            return false;
        }

        students.erase(ufid);
        cout << "successful\n";
        return true;
    }

    // remove a specific class from a student
    else if (cmd == "dropClass") {
        string ufid, code;
        ss >> ufid >> code;

        if (!students.count(ufid) || !students[ufid].classes.count(code)) {
            cout << "unsuccessful\n";
            return false;
        }

        students[ufid].classes.erase(code);

        // If student has no classes left, remove them entirely
        if (students[ufid].classes.empty()) {
            students.erase(ufid);
        }

        cout << "successful\n";
        return true;
    }

    // swap one class for another
    else if (cmd == "replaceClass") {
        string ufid, oldC, newC;
        ss >> ufid >> oldC >> newC;

        if (!students.count(ufid) || !class_map.count(newC)) {
            cout << "unsuccessful\n";
            return false;
        }

        auto &s = students[ufid];

        if (!s.classes.count(oldC) || s.classes.count(newC)) {
            cout << "unsuccessful\n";
            return false;
        }

        s.classes.erase(oldC);
        s.classes.insert(newC);

        cout << "successful\n";
        return true;
    }

    // remove a class from all students
    else if (cmd == "removeClass") {
        string code;
        ss >> code;

        if (!class_map.count(code)) {
            cout << "unsuccessful\n";
            return false;
        }

        int count = 0;
        vector<string> removeList;

        // Remove the class from all students
        for (auto &p : students) {
            if (p.second.classes.count(code)) {
                p.second.classes.erase(code);
                count++;

                // Remove student if they have no classes left
                if (p.second.classes.empty()) {
                    removeList.push_back(p.first);
                }
            }
        }

        for (auto &id : removeList) students.erase(id);

        if (count == 0) {
            cout << "unsuccessful\n";
            return false;
        }

        cout << count << "\n"; // Print number of students affected
        return true;
    }

    // check if a path is open/closed/dne
    else if (cmd == "checkEdgeStatus") {
        int u, v;
        ss >> u >> v;

        if (!edgeExists(u, v)) cout << "DNE\n";
        else if (isEdgeClosed(u, v)) cout << "closed\n";
        else cout << "open\n";

        return true;
    }

    // open/close paths
    else if (cmd == "toggleEdgesClosure") {
        int n;
        ss >> n;

        for (int i = 0; i < n; i++) {
            int u, v;
            ss >> u >> v;
            toggleEdgde(u, v);
        }

        cout << "successful\n";
        return true;
    }

    // check if two locations can be reached
    else if (cmd == "isConnected") {
        int u, v;
        ss >> u >> v;

        unordered_set<int> visited;
        if (dfs(u, v, visited)) cout << "successful\n";
        else cout << "unsuccessful\n";

        return true;
    }

    // show travel time to each student's class
    else if (cmd == "printShortestEdges") {
        string ufid;
        ss >> ufid;

        if (!students.count(ufid)) {
            cout << "unsuccessful\n";
            return false;
        }

        auto &s = students[ufid];
        cout << "Time For Shortest Edges: " << s.name << "\n";

        for (auto &code : s.classes) {
            int dist = dijkstra(s.residenceID, class_map[code].locationID);
            cout << code << ": " << dist << "\n";
        }

        return true;
    }

    // calculate min spanning tree cost for student's zone
    else if (cmd == "printStudentZone") {
        string ufid;
        ss >> ufid;

        if (!students.count(ufid)) {
            cout << "unsuccessful\n";
            return false;
        }

        auto &s = students[ufid];
        set<int> classLocations;
        
        for (auto &code : s.classes) {
            classLocations.insert(class_map[code].locationID);
        }
        
        int cost = studentZoneCost(s.residenceID, classLocations);
        
        if (cost == -1) {
            cout << "unsuccessful\n";
            return false;
        }

        cout << "Student Zone Cost For " << s.name << ": " << cost << "\n";
        return true;
    }

    // Unknown command
    cout << "unsuccessful\n";
    return false;
}