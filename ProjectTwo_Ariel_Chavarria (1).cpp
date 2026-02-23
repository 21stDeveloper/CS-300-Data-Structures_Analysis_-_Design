//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Ariel Chavarria
// Version     : 1.0
// Description : ABCU Advising Assistance Program (CS 300 Project Two)
//               Single-file C++ command-line program (no external CSVParser)
//               Uses a tree-based data structure via std::map (balanced BST).
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <cctype>
#include <limits>

using namespace std;

struct Course {
    string courseNumber;
    string title;
    vector<string> prereqs;
};

// ---------- Helpers ----------
static inline string ltrim(const string& s) {
    size_t i = 0;
    while (i < s.size() && isspace(static_cast<unsigned char>(s[i]))) i++;
    return s.substr(i);
}

static inline string rtrim(const string& s) {
    if (s.empty()) return s;
    size_t i = s.size();
    while (i > 0 && isspace(static_cast<unsigned char>(s[i - 1]))) i--;
    return s.substr(0, i);
}

static inline string trim(const string& s) {
    return rtrim(ltrim(s));
}

static inline string toUpper(string s) {
    for (char& c : s) c = static_cast<char>(toupper(static_cast<unsigned char>(c)));
    return s;
}

// Simple CSV split on commas (file is simple: no quoted commas)
static vector<string> splitCSV(const string& line) {
    vector<string> tokens;
    string token;
    stringstream ss(line);
    while (getline(ss, token, ',')) {
        tokens.push_back(trim(token));
    }
    return tokens;
}

static void printMenu() {
    cout << endl;
    cout << "  1. Load Data Structure." << endl;
    cout << "  2. Print Course List." << endl;
    cout << "  3. Print Course." << endl;
    cout << "  9. Exit" << endl;
    cout << endl;
    cout << "What would you like to do? ";
}

// Prints all courses in alphanumeric order (std::map already sorted by key)
static void printCourseList(const map<string, Course>& courses) {
    cout << endl;
    for (const auto& pair : courses) {
        cout << pair.second.courseNumber << ", " << pair.second.title << endl;
    }
    cout << endl;
}

static void printCourse(const map<string, Course>& courses, const string& rawCourseNum) {
    string courseNum = toUpper(trim(rawCourseNum));

    auto it = courses.find(courseNum);
    if (it == courses.end()) {
        cout << endl;
        cout << "Course not found: " << courseNum << endl;
        cout << endl;
        return;
    }

    const Course& c = it->second;

    cout << endl;
    cout << c.courseNumber << ", " << c.title << endl;

    if (c.prereqs.empty()) {
        cout << "Prerequisites: None" << endl;
        cout << endl;
        return;
    }

    // Print prereqs as numbers and (title) when available
    cout << "Prerequisites: ";
    for (size_t i = 0; i < c.prereqs.size(); i++) {
        const string& pre = c.prereqs[i];
        auto pit = courses.find(pre);
        if (pit != courses.end()) {
            cout << pre << " (" << pit->second.title << ")";
        } else {
            cout << pre << " (missing)";
        }
        if (i + 1 < c.prereqs.size()) cout << ", ";
    }
    cout << endl << endl;
}

// Returns true on success; fills courses map
static bool loadCoursesFromFile(const string& fileName, map<string, Course>& courses) {
    courses.clear();

    ifstream file(fileName);
    if (!file.is_open()) {
        cout << endl;
        cout << "Error: Unable to open file: " << fileName << endl;
        cout << endl;
        return false;
    }

    // Pass 1: parse and store; collect course numbers
    vector<Course> temp;
    set<string> courseNumbers;

    string line;
    size_t lineNum = 0;
    while (getline(file, line)) {
        lineNum++;
        line = trim(line);
        if (line.empty()) continue;

        vector<string> tokens = splitCSV(line);
        if (tokens.size() < 2) {
            cout << "Format error on line " << lineNum << ": expected at least 2 values." << endl;
            continue;
        }

        Course c;
        c.courseNumber = toUpper(trim(tokens[0]));
        c.title = trim(tokens[1]);

        if (c.courseNumber.empty() || c.title.empty()) {
            cout << "Format error on line " << lineNum << ": missing course number or title." << endl;
            continue;
        }

        // Remaining tokens are prerequisites (0..N)
        for (size_t i = 2; i < tokens.size(); i++) {
            string pre = toUpper(trim(tokens[i]));
            if (!pre.empty()) c.prereqs.push_back(pre);
        }

        courseNumbers.insert(c.courseNumber);
        temp.push_back(c);
    }

    file.close();

    // Pass 2: prerequisite existence validation (warn only)
    for (const Course& c : temp) {
        for (const string& pre : c.prereqs) {
            if (courseNumbers.find(pre) == courseNumbers.end()) {
                cout << "Prerequisite warning: " << pre
                     << " listed for " << c.courseNumber
                     << " but not found in file." << endl;
            }
        }
    }

    // Insert into tree structure (map)
    for (const Course& c : temp) {
        courses[c.courseNumber] = c;
    }

    cout << endl;
    cout << "Data loaded: " << courses.size() << " courses." << endl;
    cout << endl;

    return true;
}

int main() {
    cout << "Welcome to the course planner." << endl;

    map<string, Course> courses;   // tree-based structure
    bool dataLoaded = false;

    while (true) {
        printMenu();

        int choice;
        if (!(cin >> choice)) {
            // Handle non-integer input
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << endl << "Please enter a number from the menu." << endl;
            continue;
        }

        // Clear leftover newline before getline usage
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 1) {
            cout << "Enter file name: ";
            string fileName;
            getline(cin, fileName);
            fileName = trim(fileName);

            if (fileName.empty()) {
                cout << endl << "Error: file name cannot be empty." << endl;
                continue;
            }

            dataLoaded = loadCoursesFromFile(fileName, courses);

        } else if (choice == 2) {
            if (!dataLoaded) {
                cout << endl << "Load data first (Option 1)." << endl;
                continue;
            }
            printCourseList(courses);

        } else if (choice == 3) {
            if (!dataLoaded) {
                cout << endl << "Load data first (Option 1)." << endl;
                continue;
            }
            cout << "What course do you want to know about? ";
            string courseNum;
            getline(cin, courseNum);
            printCourse(courses, courseNum);

        } else if (choice == 9) {
            cout << endl << "Thank you for using the course planner!" << endl;
            break;

        } else {
            cout << endl << choice << " is not a valid option." << endl;
        }
    }

    return 0;
}
