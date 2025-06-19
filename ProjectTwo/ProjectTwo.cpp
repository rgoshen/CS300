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
    Course course;                 // Course object containing course data
    HashNode* next = nullptr;      // Pointer to next node in chain (for collisions)
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
 * Function: Create Single Course Object
 * Purpose: Creates a Course object from a validated line of data
 * Input: line - validated line from file, course - reference to Course object to populate
 * Output: true if course was created successfully, false otherwise
 */
bool createCourseObject(string line, Course& course) {
    vector<string> tokens;
    if (!parseLine(line, tokens) || tokens.size() < 2) {
        return false; // Invalid line format
    }

    // Set required fields
    course.courseNumber = tokens[0];
    course.name = tokens[1];
    course.prerequisites.clear();

    // Add prerequisites (tokens 2 and beyond)
    for (int i = 2; i < tokens.size(); i++) {
        if (!tokens[i].empty()) {
            course.prerequisites.push_back(tokens[i]);
        }
    }

    return true;
}

/**
 * Function: Initialize Hash Table
 * Purpose: Creates and initializes a new hash table with specified capacity
 * Input: initialCapacity - starting number of buckets
 * Output: Initialized HashTable structure
 */
HashTable initializeHashTable(int initialCapacity = 16) {
    HashTable table;
    table.capacity = initialCapacity;
    table.size = 0;
    table.maxLoadFactor = 0.7;

    // Initialize all buckets to null
    table.buckets.resize(initialCapacity);
    for (int i = 0; i < initialCapacity; i++) {
        table.buckets[i] = nullptr;
    }

    return table;
}

/**
 * Function: Hash Function
 * Purpose: Converts course number string to hash index using polynomial rolling hash
 * Input: courseNumber - string to hash, capacity - table size for modulo
 * Output: Hash index (0 to capacity-1)
 */
int hashFunction(string courseNumber, int capacity) {
    if (courseNumber.empty()) {
        return 0;
    }

    int hash = 0;
    int prime = 31; // Common prime for polynomial hash

    for (int i = 0; i < courseNumber.length(); i++) {
        char c = courseNumber[i];
        hash = hash * prime + (int)c;
    }

    // Ensure positive result and fit within table capacity
    return abs(hash) % capacity;
}

/**
 * Function: Calculate Load Factor
 * Purpose: Determines current load factor for resize decisions
 * Input: table - hash table to analyze
 * Output: Current load factor (size/capacity ratio)
 */
double getLoadFactor(HashTable table) {
    if (table.capacity == 0) {
        return 0.0;
    }
    return (double)table.size / (double)table.capacity;
}

/**
 * Function: Resize Hash Table
 * Purpose: Doubles table capacity and rehashes all existing courses
 * Input: table - reference to hash table to resize
 * Output: Updates table with new capacity and redistributed courses
 */
void resizeHashTable(HashTable& table) {
    // Store old buckets for rehashing
    vector<HashNode*> oldBuckets = table.buckets;
    int oldCapacity = table.capacity;

    // Double the capacity
    table.capacity = table.capacity * 2;
    table.size = 0; // Will be recounted during rehashing

    // Create new empty bucket array
    table.buckets.clear();
    table.buckets.resize(table.capacity);
    for (int i = 0; i < table.capacity; i++) {
        table.buckets[i] = nullptr;
    }

    // Rehash all existing courses into new table
    for (int i = 0; i < oldCapacity; i++) {
        HashNode* current = oldBuckets[i];
        while (current != nullptr) {
            HashNode* next = current->next; // Save next before reinsertion

            // Calculate new hash index for the resized table
            int newIndex = hashFunction(current->course.courseNumber, table.capacity);

            // Insert directly into new table (no resize check needed)
            current->next = table.buckets[newIndex];
            table.buckets[newIndex] = current;
            table.size++;

            current = next;
        }
    }

    cout << "Hash table resized from " << oldCapacity << " to " << table.capacity << " buckets" << endl;
}

/**
 * Function: Check and Resize if Needed
 * Purpose: Monitors load factor and triggers resize when necessary
 * Input: table - reference to hash table to check
 * Output: Resizes table if load factor > 0.7
 */
void checkAndResize(HashTable& table) {
    if (getLoadFactor(table) > table.maxLoadFactor) {
		resizeHashTable(table);
    }
}

/**
 * Function: Insert Course into Hash Table
 * Purpose: Adds a course to the hash table using chaining for collision resolution
 * Input: table - reference to hash table, course - Course object to insert
 * Output: Updates table with new course, handles collisions via chaining
 */
void insertCourseIntoTable(HashTable& table, Course course) {
    // Check if resize needed before insertion
    checkAndResize(table);

    // Calculate hash index for course
    int index = hashFunction(course.courseNumber, table.capacity);

    // Create new node for the course
    HashNode* newNode = new HashNode();
    newNode->course = course;
    newNode->next = nullptr;

    // Handle collision using chaining
    if (table.buckets[index] == nullptr) {
        // No collision - first course in this bucket
        table.buckets[index] = newNode;
    }
    else {
        // Collision detected - check for duplicate course numbers first
        HashNode* current = table.buckets[index];
        while (current != nullptr) {
            if (current->course.courseNumber == course.courseNumber) {
                // Update existing course instead of creating duplicate
                current->course = course;
                delete newNode; // Clean up unused node
                return;
            }
            current = current->next;
        }

        // No duplicate found - add to front of chain
        newNode->next = table.buckets[index];
        table.buckets[index] = newNode;
    }

    table.size = table.size + 1;
}

/**
 * Collects all courses from the hash table into a vector for sorting
 * Traverses all buckets and collision chains to gather every course
 * @param table The hash table containing course data
 * @return Vector containing all courses from the hash table
 */
vector<Course> collectAllCourses(const HashTable& table) {
    vector<Course> allCourses;

    // Traverse each bucket in the hash table
    for (int i = 0; i < table.capacity; i++) {
        HashNode* current = table.buckets[i];

        // Traverse the collision chain at this bucket
        while (current != nullptr) {
            allCourses.push_back(current->course);
            current = current->next;
        }
    }

    return allCourses;
}

/**
 * Merges two sorted subarrays into one sorted array
 * @param courses Vector containing the subarrays
 * @param left Start of first subarray
 * @param middle End of first subarray (start of second is middle+1)
 * @param right End of second subarray
 */
void merge(vector<Course>& courses, int left, int middle, int right) {
    // Create temporary arrays for the two subarrays
    vector<Course> leftArray;
    vector<Course> rightArray;

    // Copy data to temporary arrays
    for (int i = left; i <= middle; i++) {
        leftArray.push_back(courses[i]);
    }

    for (int j = middle + 1; j <= right; j++) {
        rightArray.push_back(courses[j]);
    }

    // Merge the temporary arrays back into courses[left..right]
    int i = 0;    // Initial index of first subarray
    int j = 0;    // Initial index of second subarray
    int k = left; // Initial index of merged subarray

    while (i < leftArray.size() && j < rightArray.size()) {
        // Compare course numbers for alphabetical ordering
        if (leftArray[i].courseNumber <= rightArray[j].courseNumber) {
            courses[k] = leftArray[i];
            i++;
        }
        else {
            courses[k] = rightArray[j];
            j++;
        }
        k++;
    }

    // Copy remaining elements of leftArray, if any
    while (i < leftArray.size()) {
        courses[k] = leftArray[i];
        i++;
        k++;
    }

    // Copy remaining elements of rightArray, if any
    while (j < rightArray.size()) {
        courses[k] = rightArray[j];
        j++;
        k++;
    }
}

/**
 * Recursive merge sort implementation
 * Divides the array into halves and recursively sorts each half
 * @param courses Vector to sort
 * @param left Starting index of the subarray
 * @param right Ending index of the subarray
 */
void mergeSort(vector<Course>& courses, int left, int right) {
    if (left < right) {
        int middle = left + (right - left) / 2; // Avoid potential overflow

        // Recursively sort first and second halves
        mergeSort(courses, left, middle);
        mergeSort(courses, middle + 1, right);

        // Merge the sorted halves
        merge(courses, left, middle, right);
    }
}

/**
 * Sorts a vector of courses alphanumerically by course number
 * Uses merge sort algorithm for O(n log n) performance
 * @param courses Vector of Course objects to sort (modified in place)
 */
void sortCoursesAlphanumerically(vector<Course>& courses) {
    if (courses.size() <= 1) {
        return; // Already sorted or empty
    }

    mergeSort(courses, 0, courses.size() - 1);
}

/**
 * Function: Print Single Course Info
 * Purpose: Displays basic course information in required format
 * Input: course - Course object to display
 * Output: Prints "courseNumber, courseName" to console
 */
void printCourseInfo(const Course& course) {
    cout << course.courseNumber << ", " << course.name << endl;
}

/**
 * Function: Print All Courses Sorted with Pagination
 * Purpose: Displays all courses from hash table in alphanumeric order with pagination
 * Input: table - hash table containing courses
 * Output: Displays all courses sorted by course number, 20 per page
 */
void printAllCoursesSorted(const HashTable& table) {
    if (table.size == 0) {
        cout << "No courses loaded. Please load data first using option 1." << endl;
        return;
    }

    // Collect all courses from hash table
    vector<Course> allCourses = collectAllCourses(table);

    // Sort the courses alphanumerically
    sortCoursesAlphanumerically(allCourses);

    // Display the sorted course list with pagination
    cout << "Here is a sample schedule:\n" << endl;

    int pageSize = 10;
    int currentPage = 1;
    int totalPages = (allCourses.size() + pageSize - 1) / pageSize; // Ceiling division

    for (size_t i = 0; i < allCourses.size(); i++) {
        // Print page header at the start of each page
        if (i % pageSize == 0) {
            if (i > 0) {
                // Add blank line before pagination prompt
                cout << endl;
                cout << "Press Enter to continue to page " << (currentPage + 1)
                    << " of " << totalPages << "..." << endl;
                cin.get(); // Wait for Enter key
                cout << endl; // Add blank line after user input
                currentPage++; // Move to next page after user input
            }

            if (totalPages > 1) {
                cout << "--- Page " << currentPage << " of " << totalPages << " ---" << endl;
                cout << endl; // Add blank line after page header
            }
        }

        printCourseInfo(allCourses[i]);
    }

    cout << "\nTotal courses displayed: " << allCourses.size() << endl;
    cout << endl; // Add blank line for separation before menu redisplays
}

/**
 * Function: Search Course in Hash Table
 * Purpose: Efficiently finds a course using hash table lookup
 * Input: table - hash table to search, courseNumber - course to find,
 *        foundCourse - reference to Course object to populate if found
 * Output: true if course found, false otherwise
 * Complexity: Average O(1), worst case O(n) if many collisions
 */
bool searchCourse(const HashTable& table, const string& courseNumber, Course& foundCourse) {
    if (courseNumber.empty()) {
        return false;
    }

    // Calculate hash index
    int index = hashFunction(courseNumber, table.capacity);

    // Search through chain at this index
    HashNode* current = table.buckets[index];

    while (current != nullptr) {
        if (current->course.courseNumber == courseNumber) {
            foundCourse = current->course;
            return true;
        }
        current = current->next;
    }

    return false; // Course not found
}

/**
 * Function: Print Prerequisites
 * Purpose: Displays all prerequisites for a course using hash table lookups
 * Input: course - Course object whose prerequisites to display,
 *        table - hash table for prerequisite lookups
 * Output: Prints prerequisite course information or "No prerequisites required"
 */
void printPrerequisites(const Course& course, const HashTable& table) {
    if (course.prerequisites.size() == 0) {
        cout << "No prerequisites required" << endl;
        return;
    }

    cout << "Prerequisites: ";

    for (size_t i = 0; i < course.prerequisites.size(); i++) {
        Course prereqCourse;
        if (searchCourse(table, course.prerequisites[i], prereqCourse)) {
            cout << prereqCourse.courseNumber << ", " << prereqCourse.name;
        }
        else {
            cout << "Warning: Prerequisite " << course.prerequisites[i] << " not found";
        }

        // Add comma separator between prerequisites (except for last one)
        if (i < course.prerequisites.size() - 1) {
            cout << "; ";
        }
    }

    cout << endl;
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
    cout << endl;
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
void menuOption1(const string& filename, HashTable& table) {
    cout << "\nLoading data structure..." << endl;

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

    // Step 3: Create course objects and insert into hash table
    for (string line : lines) {
        Course newCourse;
        if (createCourseObject(line, newCourse)) {
            insertCourseIntoTable(table, newCourse);
        }
        else {
            cout << "Warning: Skipping invalid line during course creation" << endl;
        }
    }

    cout << "Courses loaded successfully into hash table!" << endl;
    cout << "Number of courses loaded: " << table.size << endl;
    cout << "Hash table capacity: " << table.capacity << endl;
    cout << "Current load factor: " << getLoadFactor(table) << endl;
}

/**
 * Function: Menu Option 2 - Print Course List
 * Purpose: Prints all courses in alphanumeric order with error handling
 * Input: table - hash table containing courses
 * Output: All courses displayed in alphanumeric order by course number
 */
void menuOption2(const HashTable& table) {
    printAllCoursesSorted(table);
}

/**
 * Function: Menu Option 3 - Print Course
 * Purpose: Searches for and displays specific course information with prerequisites
 * Input: table - hash table containing courses
 * Output: Displays course information and prerequisites, or error message
 */
void menuOption3(const HashTable& table) {
    if (table.size == 0) {
        cout << "No courses loaded. Please load data first using option 1." << endl;
        return;
    }

    string courseNumber;
    cout << "What course do you want to know about? ";
    getline(cin, courseNumber);

    if (courseNumber.empty()) {
        cout << "Error: Course number cannot be empty" << endl;
        return;
    }

    // Convert to uppercase for case-insensitive search
    for (char& c : courseNumber) {
        c = toupper(c);
    }

    Course foundCourse;
    if (searchCourse(table, courseNumber, foundCourse)) {
        cout << endl << "Course Information:" << endl;
        cout << "===================" << endl;
        printCourseInfo(foundCourse);
        cout << endl;
        printPrerequisites(foundCourse, table);
    }
    else {
        cout << "Course '" << courseNumber << "' not found." << endl;
        cout << "Please check the course number and try again." << endl;
    }
}




int main() {

    // Near the top of main(), after getting the filename:
    HashTable courseTable = initializeHashTable(16);

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
            menuOption1(filename, courseTable);
        }
        else if (choice == "2") {
            menuOption2(courseTable);
        }
        else if (choice == "3") {
            menuOption3(courseTable);
        }
        else if (choice == "9") {
            cout << "Thank you for using the course planner!\n" << endl;
            cout << "Press Enter to exit...";
            cin.get(); // Wait for user to press Enter before exiting
            running = false;
        }
    }
    

    return 0;
}