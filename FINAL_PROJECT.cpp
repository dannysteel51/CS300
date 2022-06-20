// Name        : Final_Project_CPP
// Author      : Dan Stull June 13th, 2022


#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <Windows.h>
#include <vector>

using namespace std;

const int GLOBAL_SLEEP_TIME = 4000;//default time for sleep

/*
Struct to hold the course information
*/
struct Course {

    string courseId;
    string courseName;
    vector<string> prereqList;
};

class BinarySearchTree {

private:
    // Struct that hold course
    struct Node {
        Course course;
        Node* right;
        Node* left;

        //constructor
        Node() {
            left = nullptr;
            right = nullptr;
        }

        // initialize course
        Node(Course aCourse) {
            course = aCourse;
            left = nullptr;
            right = nullptr;
        }
    };

    Node* root;
    void inOrder(Node* node);
    int size = 0;

public:
    BinarySearchTree();
    void InOrder();
    void Insert(Course aCourse);
    Course Search(string courseId);
    int Size();
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    this->root = nullptr;
}

/**
 * Move through the Binary tree in order
 */
void BinarySearchTree::InOrder() {
    inOrder(root);
}

void BinarySearchTree::Insert(Course aCourse) {

    Node* currentNode = root;

    if (root == NULL) {

        root = new Node(aCourse);

    }
    else {

        while (currentNode != NULL) {

            if (aCourse.courseId < currentNode->course.courseId) {

                if (currentNode->left == nullptr) {

                    currentNode->left = new Node(aCourse);
                    currentNode = NULL;
                }
                else {

                    currentNode = currentNode->left;
                }
            }
            else {

                if (currentNode->right == nullptr) {

                    currentNode->right = new Node(aCourse);
                    currentNode = NULL;
                }
                else {

                    currentNode = currentNode->right;
                }
            }
        }
    }
    size++;
}


/**
 * Searches for a course
 */
Course BinarySearchTree::Search(string courseId) {

    Course aCourse;

    Node* currentNode = root;

    while (currentNode != NULL) {

        if (currentNode->course.courseId == courseId) {

            return currentNode->course;
        }
        else if (courseId < currentNode->course.courseId) {

            currentNode = currentNode->left;
        }
        else {

            currentNode = currentNode->right;
        }
    }
    return aCourse;
}

void BinarySearchTree::inOrder(Node* node) {

    if (node == NULL) {

        return;
    }
    inOrder(node->left);

    //print the node
    cout << node->course.courseId << ", " << node->course.courseName << endl;

    inOrder(node->right);
}

int BinarySearchTree::Size() {

    return size;
}

/*
Borrowed code from Stackoverflow
*/
vector<string> Split(string lineFeed) {

    char delim = ',';

    lineFeed += delim; 
    vector<string> lineTokens;
    string temp = "";
    for (int i = 0; i < lineFeed.length(); i++)
    {
        if (lineFeed[i] == delim)
        {
            lineTokens.push_back(temp); 
            temp = "";
            i++;
        }
        temp += lineFeed[i];
    }
    return lineTokens;
}

void loadCourses(string csvPath, BinarySearchTree* courseList) {
    // Creates and adds data to the collection of courses

    ifstream inFS; //insteam to read file
    string line; //line feed 
    vector<string> stringTokens;

    inFS.open(csvPath); //open the file

    if (!inFS.is_open()) {//Checks for errors
        cout << "Failed to load. Recheck file source. " << endl;
        return;
    }

    while (!inFS.eof()) {

        Course aCourse;//create a new struct

        getline(inFS, line);
        stringTokens = Split(line); //splits the lines using the deliminater

        if (stringTokens.size() < 2) { //if there aren't 2 tokens the line is misformatted

            cout << "\nSkipped a line." << endl;
        }
        else {
            aCourse.courseId = stringTokens.at(0);
            aCourse.courseName = stringTokens.at(1);

            for (unsigned int i = 2; i < stringTokens.size(); i++) {

                aCourse.prereqList.push_back(stringTokens.at(i));
            }

            // push this course to the end
            courseList->Insert(aCourse);
        }
        
    }

    inFS.close(); //close the file
}

/*
Course passed in
Prints:
courseId, courseName
Prereq list (if none print "No prereq")
*/
void displayCourse(Course aCourse) {

    cout << aCourse.courseId << ", " << aCourse.courseName << endl;
    cout << "Prerequisites: ";

    if (aCourse.prereqList.empty()) {//if the lsit is empty then there are no prereq

        cout << "none" << endl;
    }
    else {

        for (unsigned int i = 0; i < aCourse.prereqList.size(); i++) {

            cout << aCourse.prereqList.at(i);

            if (aCourse.prereqList.size() > 1 && i < aCourse.prereqList.size() - 1) {//put a comma for any elements greater than 1

                cout << ", ";
            }
        }
    }

    cout << endl;
}


// Force input to be uppercase

void convertCase(string& toConvert) {

    for (unsigned int i = 0; i < toConvert.length(); i++) {

        if (isalpha(toConvert[i])) {

            toConvert[i] = toupper(toConvert[i]);
        }
    }
}

int main(int argc, char* argv[]) {

    // Process of the user inputs for the program
    string csvPath, aCourseKey;

    switch (argc) {
    case 2:
        csvPath = argv[1];
        break;
    case 3:
        csvPath = argv[1];
        aCourseKey = argv[2];
        break;
    default:
        csvPath = "C:/Users/danst/source/repos/Final_Project_CPP/Final_Project_CPP/Final_Project_File.csv";
    }

    // Creates a courseList 
    BinarySearchTree* courseList = new BinarySearchTree();

    Course course;
    bool goodInput;
    int choice = 0;

    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Courses" << endl;
        cout << "  2. Display All Courses Alphabetic Order" << endl;
        cout << "  3. Find Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";

        aCourseKey = ""; //clears string        
        string anyKey = " "; //clears string
        choice = 0; //clears choice

        try {
            cin >> choice;

            if ((choice > 0 && choice < 4) || (choice == 9)) {// limit the user menu inputs to good values
                goodInput = true;
            }
            else {//throws an error
                goodInput = false;
                throw 1;
            }

            switch (choice) {
            case 1:

                // loads the courses
                loadCourses(csvPath, courseList);
                cout << courseList->Size() << " courses read" << endl;

                // This global sleep time sets a timer, once it runs out, it pushing back to main menu
                Sleep(GLOBAL_SLEEP_TIME);

                break;

            case 2:
                courseList->InOrder();

                cout << "\nEnter \'y\' to continue..." << endl;

                cin >> anyKey;

                break;

            case 3:

                cout << "\nWhat course do you want to know about? " << endl;
                cin >> aCourseKey;

                convertCase(aCourseKey); //converts to upper case and returns

                course = courseList->Search(aCourseKey);

                if (!course.courseId.empty()) {
                    displayCourse(course);
                }
                else {
                    cout << "\nCourse ID " << aCourseKey << " not found." << endl;
                }

                Sleep(GLOBAL_SLEEP_TIME);

                break;

            case 9:
                exit;
                break;

            default:

                throw 2;
            }
        }

        catch (int err) {

            std::cout << "\n1-3 only, or 9 to exit." << endl;
            Sleep(GLOBAL_SLEEP_TIME);
        }

        cin.clear();
        cin.ignore();

        //Clears any previous consoles, making a cleaner looking screen
        system("cls");
    }

    cout << "Good bye." << endl;

    Sleep(GLOBAL_SLEEP_TIME);

    return 0;
}