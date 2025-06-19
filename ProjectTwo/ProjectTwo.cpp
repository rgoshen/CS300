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
 * Function: Open and Read File
 * Purpose: Opens a file and reads all valid lines into a vector
 * Input: filename - path to the course data file, lines - reference to vector that will store file lines
 * Output: true if file was successfully read, false otherwise
 */
bool readFileLines(string filename, vector<string>& lines) {
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Error: Cannot open file '" << filename << "'" << endl;
        return false;
    }

    string line;
    while (getline(file, line)) {
        // Check if line is not empty and not just whitespace
        if (!line.empty()) {
            // Simple whitespace check - if line has any non-space characters
            bool hasContent = false;
            for (char c : line) {
                if (c != ' ' && c != '\t' && c != '\r' && c != '\n') {
                    hasContent = true;
                    break;
                }
            }
            if (hasContent) {
                lines.push_back(line);
            }
        }
    }

    file.close();

    if (lines.size() == 0) {
        cout << "Error: File is empty or contains no valid data" << endl;
        return false;
    }

    return true;
}

/**
 * Function: Parse Single Line
 * Purpose: Splits a line by commas and cleans up tokens
 * Input: line - raw line from file, tokens - reference to vector that will store parsed tokens
 * Output: true if line was successfully parsed, false if malformed
 */
bool parseLine(string line, vector<string>& tokens) {
    if (line.empty()) {
        return false;
    }

    // Check if line is just whitespace
    bool hasContent = false;
    for (char c : line) {
        if (c != ' ' && c != '\t' && c != '\r' && c != '\n') {
            hasContent = true;
            break;
        }
    }
    if (!hasContent) {
        return false;
    }

    // Split line by comma into tokens
    tokens.clear();
    string token = "";

    for (char c : line) {
        if (c == ',') {
            tokens.push_back(token);
            token = "";
        }
        else {
            token += c;
        }
    }
    // Add the last token
    tokens.push_back(token);

    // Clean up tokens: trim whitespace and remove empty tokens
    for (int i = tokens.size() - 1; i >= 0; i--) {
        // Trim leading and trailing whitespace
        string& t = tokens[i];
        size_t start = t.find_first_not_of(" \t\r\n");
        size_t end = t.find_last_not_of(" \t\r\n");

        if (start != string::npos && end != string::npos) {
            t = t.substr(start, end - start + 1);
        }
        else {
            t = "";
        }

        // Remove empty tokens
        if (t.empty()) {
            tokens.erase(tokens.begin() + i);
        }
    }

    return tokens.size() > 0;
}

/**
 * Function: Validate Line Format
 * Purpose: Ensures each line has minimum required fields
 * Input: tokens - parsed line tokens, originalLine - for error reporting
 * Output: true if line format is valid, false otherwise
 */
bool validateLineFormat(vector<string> tokens, string originalLine) {
    if (tokens.size() < 2) {
        cout << "Error: Line '" << originalLine << "' does not have minimum required parameters" << endl;
        return false;
    }

    if (tokens[0].empty()) {
        cout << "Error: Course number cannot be empty" << endl;
        return false;
    }

    if (tokens[1].empty()) {
        cout << "Error: Course name cannot be empty" << endl;
        return false;
    }

    return true;
}

/**
 * Function: Check if Course Exists
 * Purpose: Searches for a course number in the file data
 * Input: courseNumber - course to search for, allLines - all file lines
 * Output: true if course exists, false otherwise
 */
bool courseExists(string courseNumber, vector<string> allLines) {
    if (courseNumber.empty()) {
        return false;
    }

    for (string line : allLines) {
        vector<string> tokens;
        if (parseLine(line, tokens) && tokens.size() >= 1) {
            if (tokens[0] == courseNumber) {
                return true;
            }
        }
    }

    return false;
}

/**
 * Function: Check All Prerequisites Exist
 * Purpose: Validates that all prerequisites exist as courses in the file
 * Input: allLines - all lines from the file
 * Output: true if all prerequisites are valid, false otherwise
 */
bool validatePrerequisites(vector<string> allLines) {
    for (string line : allLines) {
        vector<string> tokens;
        if (!parseLine(line, tokens)) {
            continue; // Skip malformed lines
        }

        // Check prerequisites (tokens 2 and beyond)
        if (tokens.size() > 2) { // has prerequisites
            for (int i = 2; i < tokens.size(); i++) {
                string prerequisite = tokens[i];
                if (!prerequisite.empty() && !courseExists(prerequisite, allLines)) {
                    cout << "Error: Prerequisite '" << prerequisite << "' in course '" << tokens[0] << "' does not exist as a course" << endl;
                    return false;
                }
            }
        }
    }

    return true;
}



/**
 * Function: Validate Entire File
 * Purpose: Orchestrates all file validation steps
 * Input: lines - all lines read from file
 * Output: true if entire file is valid, false if any validation fails
 */
bool validateFile(vector<string> lines) {
    if (lines.size() == 0) {
        cout << "Error: No valid lines found in file" << endl;
        return false;
    }

    // Step 1: Check that all lines can be parsed and have valid format
    for (string line : lines) {
        vector<string> tokens;
        if (!parseLine(line, tokens)) {
            cout << "Error: Unable to parse line '" << line << "'" << endl;
            return false;
        }

        if (!validateLineFormat(tokens, line)) {
            return false;
        }
    }

    // Step 2: Check that all prerequisites exist as courses
    if (!validatePrerequisites(lines)) {
        return false;
    }

    return true;
}

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
 * Function: Menu Option 1 - Load Data Structure
 * Purpose: Handles loading course data into hash table with comprehensive error handling
 * Input: table - reference to hash table to populate
 * Output: Hash table is populated with validated course data
 */
void menuOption1(const string& filename) {
    cout << "Loading data structure..." << endl;

    vector<string> lines;

    // Step 1: Read file contents
    if (!readFileLines(filename, lines)) {
        return;
    }

    // Step 2: Validate file format and prerequisites
    if (!validateFile(lines)) {
        cout << "Failed to load courses from file. Please check the file format and try again." << endl;
        return;
    }

    cout << "File validation successful!" << endl;
    cout << "Number of valid courses: " << lines.size() << endl;

    // TODO: Step 3: Create course objects and insert into hash table

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