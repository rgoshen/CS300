#include <iostream>
#include <fstream>
#include <string>

using namespace std;

/**
 * Prompts user for filename and validates file exists and is CSV format
 * Keeps asking until a valid CSV file is provided
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

        // Trim leading/trailing whitespace and quotes
        size_t start = filename.find_first_not_of(" \t\"'");
        size_t end = filename.find_last_not_of(" \t\"'");
        if (start != string::npos && end != string::npos) {
            filename = filename.substr(start, end - start + 1);
        }

        // Check if file has .csv extension (case insensitive)
        if (filename.length() < 4 ||
            (filename.substr(filename.length() - 4) != ".csv" &&
                filename.substr(filename.length() - 4) != ".CSV")) {
            cout << "Error: File must have .csv extension" << endl;
            cout << "Note: Do not include quotes around the filename" << endl;
            continue; // Ask again
        }

        // Try to open file to validate it exists
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Cannot open file '" << filename << "'" << endl;
            cout << "Debug info:" << endl;
            cout << "  - Filename length: " << filename.length() << endl;
            cout << "  - First char ASCII: " << (int)filename[0] << endl;
            cout << "  - Last char ASCII: " << (int)filename[filename.length() - 1] << endl;

            // Additional debugging - try alternative ifstream approach
            cout << "  - Attempting alternative ifstream approach..." << endl;
            ifstream testFile;
            testFile.open(filename, ios::in);
            if (testFile.is_open()) {
                testFile.close();
                cout << "  - Alternative ifstream: File IS accessible!" << endl;
                cout << "  - This suggests an ifstream initialization issue" << endl;
            }
            else {
                cout << "  - Alternative ifstream: File NOT accessible either" << endl;
                cout << "  - Checking if this is a working directory issue..." << endl;

                // Print current working directory info
                cout << "  - Try using relative path or move file to current directory" << endl;
            }

            cout << "Please check:" << endl;
            cout << "  - File exists at the specified path" << endl;
            cout << "  - You have permission to read the file" << endl;
            cout << "  - Path is correct (use tab completion if available)" << endl;
            cout << "  - Try putting file in same directory as executable" << endl;
            continue; // Ask again
        }

        file.close();
        cout << "CSV file '" << filename << "' found successfully!" << endl;
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