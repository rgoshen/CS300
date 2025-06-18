#include <iostream>
#include <fstream>
#include <string>

using namespace std;

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