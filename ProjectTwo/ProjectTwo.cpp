#include <iostream>
#include <fstream>
#include <string>

using namespace std;

/**
 * Prompts user for filename and validates file exists
 * Returns the filename if valid, exits program if invalid
 */
string getValidFilename() {
    string filename;

    cout << "Enter filename: ";
    getline(cin, filename);

    // Check if filename is empty
    if (filename.empty()) {
        cout << "Error: Filename cannot be empty" << endl;
        exit(1);
    }

    // Try to open file to validate it exists
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Cannot open file '" << filename << "'" << endl;
        exit(1);
    }

    file.close();
    cout << "File '" << filename << "' found successfully!" << endl;
    return filename;
}

int main() {
    cout << "Welcome to the ABCU Course Management System" << endl;
    cout << "===========================================" << endl;

    string filename = getValidFilename();

    cout << "Ready to process file: " << filename << endl;

    return 0;
}