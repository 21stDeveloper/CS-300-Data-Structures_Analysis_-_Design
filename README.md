# 📚 CS 300 – Project Two  
## ABCU Advising Assistance Program

### 👤 Author  
Ariel Chavarria  

---

## 📌 Project Overview

This project implements a command-line advising assistance program for ABC University (ABCU). The application allows academic advisors to load course data, view a sorted list of courses, and retrieve detailed course information including prerequisites.

The program was developed in C++ using a tree-based data structure (`std::map`) to efficiently store and retrieve course information in alphanumeric order.

---

## 🎯 Program Features

The application provides the following menu options:

1. **Load Data Structure**
   - Prompts the user for a CSV file name
   - Reads course data from the file
   - Stores courses in a tree-based structure
   - Validates formatting and prerequisite existence

2. **Print Course List**
   - Displays all courses sorted alphanumerically

3. **Print Course**
   - Prompts for a course number
   - Displays the course title and prerequisites

4. **Exit**
   - Closes the program safely

---

## 🗂 Data Structure Used

This implementation uses:

`std::map<string, Course>`

Why this structure?

- Automatically maintains sorted order
- Efficient search: O(log n)
- Efficient insertion: O(log n)
- Eliminates need for manual sorting

---

## 📄 Input File Format

The program expects a CSV file formatted as:

CourseNumber,CourseTitle,Prereq1,Prereq2,...

Example:

CSCI400,Advanced Programming in C++,CSCI300

- Each line must contain at least a course number and title
- Prerequisites are optional
- Prerequisites must exist as valid course numbers in the file

---

## ▶️ How to Run

### Compile

g++ ProjectTwo.cpp -o ProjectTwo

### Run

./ProjectTwo

When prompted, enter:

CS 300 ABCU_Advising_Program_Input.csv

Make sure the CSV file is located in the same directory as the executable.

---

## 💡 Learning Outcomes

This project demonstrates:

- Implementation of tree-based data structures
- File parsing and validation
- Runtime efficiency analysis
- Clean, modular C++ program design
