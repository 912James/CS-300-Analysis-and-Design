//============================================================================
// Name        : BinarySearchTree.cpp
// Author      : James Soto
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU CODE
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <time.h>
#include <iomanip>
#include <cctype>
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <string>
#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold course information
struct Course {
    string courseNumber;
    string courseName;
    string prerequisites;
       
};

// Internal structure for tree node
struct Node {
    Course course;
    Node *left;
    Node *right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a course
    Node(Course aCourse) :
            Node() {
        course = aCourse;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);
      
public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    Course Search(string courseNumber);
};

//Default constructor
BinarySearchTree::BinarySearchTree() {
    root = nullptr; //root is equal to nullptr
}

//Destructor
BinarySearchTree::~BinarySearchTree() { // https://www.geeksforgeeks.org/inorder-tree-traversal-without-recursion/
    // recurse from root deleting every node
}

//Traverse the tree in order
void BinarySearchTree::InOrder() {
    this->inOrder(root);     // call inOrder fuction and pass root 
}

//Insert a course
void BinarySearchTree::Insert(Course course) {
    if (root == nullptr) {  // if root equarl to null ptr
        root = new Node(course);   // root is equal to new node course
    }
    else {  // else
        this->addNode(root, course);   // add Node root and course
    }
    
}

//Search for a course 
Course BinarySearchTree::Search(string courseNumber) {   
    // Reference points for the search function code in this section
    // https://en.cppreference.com/w/cpp/algorithm/ranges/binary_search   
    // https://www.geeksforgeeks.org/binary-search-tree-set-1-search-and-insertion/
    // https://learn.zybooks.com/zybook/CS-300-T2807-OL-TRAD-UG.22EW2/chapter/6/section/4  
    // https://learn.zybooks.com/zybook/CS-300-T2807-OL-TRAD-UG.22EW2/chapter/6/section/3
  
    Node* cur = root;// set current node equal to root
    
    while (cur != nullptr) {    // keep looping downwards until bottom reached or matching courseNumber is found  
        if (cur->course.courseNumber.compare(courseNumber) == 0) {    // if match found
            return cur->course;    //  return current course
        }
        else if (courseNumber.compare(cur->course.courseNumber) < 0) {    // if course is smaller than current node then traverse left
            cur = cur->left; 
        }
        else {  // else larger so traverse right
            cur = cur->right;
        }
    }             
    Course course;
    return course;
}

 //Add a course to some node (recursive) 
void BinarySearchTree::addNode(Node* node, Course course) {       
    // References for addNode code in this section 
    // https://www.geeksforgeeks.org/binary-search-tree-set-1-search-and-insertion/
    // https://learn.zybooks.com/zybook/CS-300-T2807-OL-TRAD-UG.22EW2/chapter/6/section/5

    if (node != nullptr && node->course.courseNumber.compare(course.courseNumber) > 0) {   // if node is not nullptr and is larger then add to left
        if (node->left == nullptr) {    // if no left node
            node->left = new Node(course); // this node becomes left
            return;
        }
        else {
            this->addNode(node->left, course);     // else recurse down the left node
        }
    }
    else if (node != nullptr && node->course.courseNumber.compare(course.courseNumber) < 0) {   // else if node is not nullptr and and is smaller then add to the right
        if (node->right == nullptr) { // if no right node
            node->right = new Node(course); // this node becomes right
            return;
        }
        else {  //else
            this->addNode(node->right, course); // recurse down the left node
        }
    }
}
void BinarySearchTree::inOrder(Node* node) { // this traversal worked best for this project.
    // Reference for traversal function code in this section
    //https://www.geeksforgeeks.org/binary-search-tree-set-1-search-and-insertion/    
    
    if (node != nullptr) {  //if node is not equal to null ptr
        inOrder(node->left);  //InOrder not left
        cout << setw(12) << node->course.courseNumber << setw(39) << node->course.courseName <<  setw(25) << node->course.prerequisites << endl;    
        //formatted output for course number, courseName, prerequisites. this is called from case 2 which is option 2 for the user
        inOrder(node->right);   //InOder right
    }
}

//============================================================================
// Static methods used for testing
//============================================================================

// Method to display course that is utilized in user option 3
void displayCourse(Course course) {
    cout << endl;
    cout << "Course Number: " << course.courseNumber << endl;
    cout << "Course Name: " << course.courseName << endl;
    cout << "Prerequisites: " << course.prerequisites << endl;
    return;
}

// Method to load the course from the excel file
void loadCourse(string csvPath, BinarySearchTree* bst) {
    cout << "Loading CSV file " << csvPath << endl;
    cout << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    
    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of course
            Course course;
            course.courseNumber = file[i][1]; // uses the data from the excel file to file in the course number
            course.courseName = file[i][0]; // uses the data from the excel file to file in the course name
            course.prerequisites = file[i][2]; // uses the data from the excel file to file in the prerequisites
            
            // push this course to the end
            bst->Insert(course);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

int main(int argc, char* argv[]) { // main method
        
    string csvPath, bidKey; // process command line arguments
    switch (argc) {
    case 2:
        csvPath = argv[1];
        bidKey = argv[1];   // changed this to match the output from the assignment
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[1];
        break;
    default:
        csvPath = "ABCU_Advising.csv"; // path to excel file
        bidKey = " ";
    }

    clock_t timer; // Define a timer variable

    // Define a binary search tree to hold all courses
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Course course;

    int choice = 0;
    while (choice != 9) { // Menu
        cout << endl;
        cout << "Course Planner" << endl;
        cout << "Menu:" << endl;
        cout << "  1. Load Data Structure First" << endl;
        cout << "  2. Print Course List" << endl;
        cout << "  3. Print Course" << endl;
        cout << "  9. Exit" << endl;
        cout << endl;
        cout << "What would you like to do? ";
        cin >> choice;
        cout << endl;

        switch (choice) {

        case 1: // Case 1 loads the csv into the program
                        
            timer = clock(); // Initialize a timer variable before loading courses
                        
            loadCourse(csvPath, bst); // Complete the method call to load the courses

            // Calculate elapsed time and display result
            timer = clock() - timer; // current clock ticks minus starting clock ticks
            cout << "Course Data Structure Loaded" << endl;
            cout << "Time to complete: " << timer << " clock ticks" << endl;
            cout << "Time to complete: " << timer * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            cout << endl;
            break;

        case 2: // Case 2 prints all the courses
            cout << "Course List" << endl;
            cout << endl;
            cout << setw(15) << "Course Number" << setw(30) << "Course Name" << setw(33) << "Prerequisites" << endl; // displays formatted output
            bst->InOrder(); // calls the inorder function which prints the courses from the tree in alphanumerical order
            cout << endl;
            break;

        case 3: // Case 3 for the user to search for a specific course and display course with prerequisites
           
            cout << "What course do you want to know about?" << endl;
            cout << "Enter the course number using uppercase letters and course level" << endl;
            cout << endl;
            cin >> bidKey; // taking user input as the bidkey
            
            course = bst->Search(bidKey); // bidkey is then sent to the search function to compare
         
            if(bidKey.compare(course.courseNumber) == 0){ // if the bidkey is equal to course number
            displayCourse(course); // we display the results
            }
            else {
                cout << "Course Number " << bidKey << " not found." << endl; // otherwise the use gets a message saying course number (  ) not found           
            }

            break;

        default :
            cout << choice << " is not a valid option."; // If user enters an integer that is not an option this message displays as the default
            cout << endl;

        }
    }

    cout << "Good bye, Thank you for using the course planner." << endl;

	return 0;
}
