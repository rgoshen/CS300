# CS 300 Project Two - ABCU Course Management System

## Overview

A comprehensive C++ command-line application implementing a hash table-based course management system for the ABCU Computer Science Department. This system enables academic advisors to efficiently load, search, and display course information with prerequisite tracking.

## Features

### Core Functionality

- **Data Loading**: Robust CSV file parsing with comprehensive validation
- **Course Display**: Alphanumerically sorted course listings with pagination
- **Course Search**: Efficient hash table-based lookup with prerequisite information
- **Interactive Menu**: User-friendly command-line interface

### Advanced Features

- **Hash Table Implementation**: Dynamic resizing with collision resolution via chaining
- **Performance Analytics**: Detailed hash table statistics and collision analysis
- **Memory Management**: Proper resource cleanup and memory deallocation
- **Pagination System**: 20-courses-per-page display with user navigation
- **Error Handling**: Comprehensive input validation and user feedback

## Technical Architecture

### Data Structures

- **Hash Table**: Primary storage with chaining collision resolution
- **Dynamic Resizing**: Automatic capacity expansion when load factor exceeds 0.7
- **Course Objects**: Structured data with course number, name, and prerequisites

### Algorithms

Hash Function: Polynomial rolling hash for optimal distribution
Sorting: O(n log n) merge sort for alphanumeric ordering
Search: O(1) average time complexity hash table lookup

### Performance Characteristics

- **Time Complexity**: O(1) average search, O(n log n) sorting
- **Space Complexity**: O(n) storage with minimal overhead
- **Load Factor**: Maintained below 0.7 for optimal performance

## Installation

### Prerequisites

- C++ compiler with C++11 support or higher
- Windows/macOS/Linux operating system
- Command-line interface

### Build Instructions

#### Visual Studio (Recommended)

```bash
# Open the solution file
ProjectTwo.sln

# Build using Visual Studio IDE:

# - Select Debug or Release configuration

# - Choose x64 or ARM64 platform

# - Build > Build Solution (Ctrl+Shift+B)
```

#### Command Line

```bash
# Using Visual Studio Developer Command Prompt
cd ProjectTwo
cl ProjectTwo.cpp /EHsc

# Using g++ (if available)

g++ -o ProjectTwo ProjectTwo/ProjectTwo.cpp -std=c++11
```

## Usage

### Running the Application

#### From Visual Studio

- Press F5 (Debug) or Ctrl+F5 (Release) to run directly from IDE

#### From Command Line

```bash
# Navigate to build output directory
cd Debug # For Debug build

# OR

cd Release # For Release build

# Run the executable

./ProjectTwo.exe # Windows
./ProjectTwo # macOS/Linux
```

### Sample Workflow

1. File Input: Provide CSV filename when prompted
2. Menu Navigation: Select from available options (1, 2, 3, 9)
3. Data Loading: Option 1 - Load course data with validation
4. Course Listing: Option 2 - View all courses in sorted order
5. Course Search: Option 3 - Search for specific course information
6. Exit: Option 9 - Clean shutdown with memory cleanup

### Input File Format

CSV files must follow this structure:

```csv
CourseNumber,CourseName,Prerequisite1,Prerequisite2,...
CSCI100,Introduction to Computer Science
CSCI101,Introduction to Programming in C++,CSCI100
CSCI200,Data Structures,CSCI101
```

## File Structure

```bash
PROJECTTWO/
├── .vs/ProjectTwo/ # Visual Studio cache and settings
├── ARM64/ # ARM64 build outputs
│ ├── Debug/
│ └── Release/
├── Debug/ # x64 Debug build outputs
│ ├── ProjectTwo.exe
│ └── ProjectTwo.pdb
├── ProjectTwo/ # Main project folder
│ ├── ARM64/ # ARM64 project outputs
│ ├── Debug/ # Debug project outputs
│ ├── Release/ # Release project outputs
│ ├── CS 300 ABCU_Advising_Program_Input.csv # Sample data
│ ├── ProjectTwo.cpp # Complete implementation
│ ├── ProjectTwo.vcxproj # Project configuration
│ └── ProjectTwo.vcxproj.filters # Project filters
├── Release/ # x64 Release build outputs
│ ├── ProjectTwo.exe
│ └── ProjectTwo.pdb
├── .gitignore # Git ignore rules
├── CS 300 Project 1-Analysis and Recommendation_final... # Project 1 document
├── ProjectTwo.sln # Visual Studio solution
└── README.md # This file
```

## Key Functions

### Core Components

- `initializeHashTable()`: Creates hash table with specified capacity
- `loadCoursesFromFileHashTable()`: Comprehensive file loading and validation
- `searchCourse()`: Efficient course lookup with collision handling
- `printAllCoursesSorted()`: Sorted display with pagination

### Utility Functions

- `hashFunction()`: Polynomial rolling hash implementation
- `resizeHashTable()`: Dynamic capacity expansion
- `displayHashTableStats()`: Performance monitoring and analysis
- `cleanupHashTable()`: Memory deallocation and cleanup

### Performance Metrics

#### Hash Table Statistics

- Load factor monitoring and optimization
- Collision detection and chain length analysis
- Empty bucket distribution tracking
- Average search operation calculation

#### Example Output

```bash
=== Hash Table Performance Statistics ===
Total courses: 8
Table capacity: 16
Load factor: 0.500
Empty buckets: 10 (62.5%)
Maximum chain length: 2
Average search operations: 1.13
```

### Error Handling

#### File Validation

- CSV extension verification
- File existence and accessibility checks
- Malformed data detection
- Prerequisite cross-reference validation

#### User Input Validation

- Menu choice verification
- Empty input handling
- Case-insensitive course search
- Comprehensive error messaging

## Development Standards

### Code Quality

- Industry standard commenting and documentation
- Consistent naming conventions
- Modular design with single responsibility principle
- Comprehensive error handling throughout

### Memory Management

- Dynamic allocation with proper deallocation
- Memory leak prevention
- Clean shutdown procedures
- Resource cleanup validation

## Testing

### Validation Scenarios

- Valid and invalid CSV files
- Edge cases (empty files, malformed data)
- Large datasets for performance testing
- Memory cleanup verification
- Menu navigation and error conditions

## Requirements Met

### Academic Standards

- Single file implementation (ProjectTwo.cpp)
- No external libraries or dependencies
- Command-line interface as specified
- Exact output format matching sample requirements

### Performance Requirements

- Efficient data structure implementation
- Optimal algorithm selection
- Professional code organization
- Industry best practices

Author
Rick Goshen
CS 300 - Analysis and Design
Southern New Hampshire University
June 2025
