#include <iostream>
#include <fstream>
#include <string>

using namespace std;

/**
 * Prompts user for filename and validates file exists and is csv format
 * Keeps asking until a valid file is provided
 * Returns the filename when valid CSV file is found
 */
string getValidFilename() {
    string filename;

    while (true) {
        cout << "Enter filename (must be .csv format): ";
        getline(cin, filename);

        // Check if filename is empty
        if (filename.empty()) {
            cout << "Error: Filename cannot be empty" << endl;
            continue; // Ask again
        }

        // Check if file has .csv extension (case insensitive)
        if (filename.length() < 4 ||
            (filename.substr(filename.length() - 4) != ".csv" &&
                filename.substr(filename.length() - 4) != ".CSV")) {
            cout << "Error: File must have .csv extension" << endl;
            continue; // Ask again
        }

        // Try to open file to validate it exists
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Cannot open file '" << filename << "'" << endl;
            continue; // Ask again
        }

        file.close();
        cout << "File '" << filename << "' found successfully!" << endl;
        return filename; // Only return when valid CSV file found
    }
}

int main() {
    cout << "Welcome to the ABCU Course Management System" << endl;
    cout << "===========================================" << endl;

    string filename = getValidFilename();

    cout << "Ready to process file: " << filename << endl;
    cout << "\nFile input successful! Press Enter to continue...";
    cin.get(); // Wait for user to press Enter

    cout << "Program will continue with file processing in next iteration." << endl;

    return 0;
}