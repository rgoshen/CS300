#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

/**
 * Course Structure
 * Contains course number, name, and list of prerequisites
 */
struct Course {
    string courseNumber;          // Unique identifier (e.g., "CSCI100")
    string name;                  // Full course name
    vector<string> prerequisites; // List of prerequisite course numbers
};

/**
 * Hash table node structure for chaining collision resolution
 */
struct HashNode {
    Course course;             // Course object containing course data
    HashNode* next;            // Pointer to next node in chain (for collisions)
};

/**
 * Hash table structure with dynamic resizing capability
 */
struct HashTable {
    vector<HashNode*> buckets; // Array of pointers to hash nodes
    int size;                  // Current number of courses stored
    int capacity;              // Current number of buckets
    double maxLoadFactor;      // Maximum load factor before resize (0.7)
};

/**
 * Trims leading/trailing whitespace and quotes from filename
 */
string trimFilename(const string& filename) {
    size_t start = filename.find_first_not_of(" \t\"'");
    size_t end = filename.find_last_not_of(" \t\"'");

    if (start != string::npos && end != string::npos) {
        return filename.substr(start, end - start + 1);
    }
    return filename;
}

/**
 * Validates if filename has .csv extension (case insensitive)
 */
bool isValidCsvExtension(const string& filename) {
    if (filename.length() < 4) {
        return false;
    }

    string extension = filename.substr(filename.length() - 4);
    return (extension == ".csv" || extension == ".CSV");
}

/**
 * Checks if file exists and can be opened
 */
bool fileExists(const string& filename) {
    ifstream file(filename);
    bool exists = file.is_open();
    if (exists) {
        file.close();
    }
    return exists;
}

/**
 * Gets raw filename input from user
 */
string getUserInput() {
    string filename;
    cout << "Enter filename (must be .csv format): ";
    getline(cin, filename);
    return filename;
}

/**
 * Prompts user for filename and validates file exists and is CSV format
 * Keeps asking until a valid CSV file is provided
 * Returns the filename when valid CSV file is found
 */
string getValidFilename() {
    while (true) {
        string filename = getUserInput();

        // Check if filename is empty
        if (filename.empty()) {
            cout << "Error: Filename cannot be empty" << endl;
            continue;
        }

        // Clean the filename
        filename = trimFilename(filename);

        // Validate CSV extension
        if (!isValidCsvExtension(filename)) {
            cout << "Error: File must have .csv extension" << endl;
            continue;
        }

        // Check if file exists
        if (!fileExists(filename)) {
            cout << "Error: Cannot open file '" << filename << "'" << endl;
            continue;
        }

        cout << "CSV file '" << filename << "' found successfully!" << endl;
        return filename;
    }
}

/**
 * Displays the main menu options
 */
void displayMenu() {
    cout << "1. Load Data Structure." << endl;
    cout << "2. Print Course List." << endl;
    cout << "3. Print Course." << endl;
    cout << "9. Exit" << endl;
    cout << "What would you like to do? ";
}

/**
 * Validates menu choice input
 */
bool isValidMenuChoice(const string& choice) {
    return (choice == "1" || choice == "2" || choice == "3" || choice == "9");
}

/**
 * Gets and validates menu choice from user
 */
string getMenuChoice() {
    string choice;
    getline(cin, choice);
    return choice;
}

/**
 * Handles menu option 1 - Load Data Structure
 */
void menuOption1(const string& filename) {
    cout << "Loading data structure..." << endl;
    cout << "Data structure loaded with file: " << filename << endl;
}

/**
 * Handles menu option 2 - Print Course List
 */
void menuOption2() {
    cout << "Here is a sample schedule:" << endl;
    cout << "CSCI100, Introduction to Computer Science" << endl;
    cout << "CSCI101, Introduction to Programming in C++" << endl;
    cout << "CSCI200, Data Structures" << endl;
    cout << "CSCI301, Advanced Programming in C++" << endl;
    cout << "CSCI300, Introduction to Algorithms" << endl;
    cout << "CSCI350, Operating Systems" << endl;
    cout << "CSCI400, Large Software Development" << endl;
    cout << "MATH201, Discrete Mathematics" << endl;
}

/**
 * Handles menu option 3 - Print Course
 */
void menuOption3() {
    string courseNumber;
    cout << "What course do you want to know about? ";
    getline(cin, courseNumber);

    cout << "CSCI400, Large Software Development" << endl;
    cout << "Prerequisites: CSCI301, CSCI350" << endl;
}



int main() {
    cout << "Welcome to the ABCU Course Management System" << endl;
    cout << "===========================================" << endl;

    string filename = getValidFilename();

    cout << "Ready to process file: " << filename << endl;

    // Transition to menu system
    cout << "\nWelcome to the course planner." << endl;

    bool running = true;

    while (running) {
        displayMenu();
        string choice = getMenuChoice();

        if (!isValidMenuChoice(choice)) {
            cout << choice << " is not a valid option." << endl;
            continue;
        }

        if (choice == "1") {
            menuOption1(filename);
        }
        else if (choice == "2") {
            menuOption2();
        }
        else if (choice == "3") {
            menuOption3();
        }
        else if (choice == "9") {
            cout << "Thank you for using the course planner!" << endl;
            cout << "Press Enter to exit...";
            cin.get(); // Wait for user to press Enter before exiting
            running = false;
        }
    }
    

    return 0;
}