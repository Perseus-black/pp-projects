#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
#include <cctype>

using namespace std;

struct Student
{
    int rollNo;
    string registrationNo;
    string name;
    string fatherName;
    string email;
    string phone;
    string address;
    char section;
    int semester;
    bool active;
};

struct Course
{
    string code;
    string name;
    string professor;
    int creditHours;
};

struct Result
{
    int rollNo;
    string courseCode;
    float marks;
};

struct Attendance
{
    int rollNo;
    string courseCode;
    int totalClasses;
    int attendedClasses;
};

struct Fee
{
    int rollNo;
    double totalFee;
    double paidFee;
};

const int MAX_STUDENTS = 150;
const int MAX_COURSES = 50;
const int MAX_COURSES_PER_STUDENT = 50;

Student students[MAX_STUDENTS];
Course courses[MAX_COURSES];

Result results[MAX_STUDENTS * MAX_COURSES_PER_STUDENT];

Attendance attendanceRecords[
    MAX_STUDENTS * MAX_COURSES_PER_STUDENT
];

Fee fees[MAX_STUDENTS];

int studentCount = 0;
int courseCount = 0;
int attendanceCount = 0;
int resultCount = 0;
int feeCount = 0;

void clearInput()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void ignoreLine()
{
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int findStudent(int rollNo)
{
    for (int i = 0; i < studentCount; i++)
    {
        if (students[i].rollNo == rollNo)
        {
            return i;
        }
    }

    return -1;
}

int findStudentByRegistration(string registrationNo)
{
    for (int i = 0; i < studentCount; i++)
    {
        if (students[i].registrationNo == registrationNo)
        {
            return i;
        }
    }

    return -1;
}

int findCourse(string code)
{
    for (int i = 0; i < courseCount; i++)
    {
        if (courses[i].code == code)
        {
            return i;
        }
    }

    return -1;
}

int findResult(int rollNo, string courseCode)
{
    for (int i = 0; i < resultCount; i++)
    {
        if (results[i].rollNo == rollNo &&
            results[i].courseCode == courseCode)
        {
            return i;
        }
    }

    return -1;
}

int findAttendance(int rollNo, string courseCode)
{
    for (int i = 0; i < attendanceCount; i++)
    {
        if (attendanceRecords[i].rollNo == rollNo &&
            attendanceRecords[i].courseCode == courseCode)
        {
            return i;
        }
    }

    return -1;
}

int findFee(int rollNo)
{
    for (int i = 0; i < feeCount; i++)
    {
        if (fees[i].rollNo == rollNo)
        {
            return i;
        }
    }

    return -1;
}

void addStudent()
{
    cout << "\n";
    cout << "========================================\n";
    cout << "              ADD STUDENT\n";
    cout << "========================================\n";

    if (studentCount >= MAX_STUDENTS)
    {
        cout << "Student storage is full.\n";
        return;
    }

    Student newStudent;

    // ---------------- ROLL NUMBER ----------------

    cout << "Enter Roll Number: ";

    if (!(cin >> newStudent.rollNo))
    {
        cout << "Invalid roll number.\n";
        clearInput();
        return;
    }

    if (newStudent.rollNo <= 0)
    {
        cout << "Roll number must be greater than zero.\n";
        return;
    }

    if (findStudent(newStudent.rollNo) != -1)
    {
        cout << "A student with this roll number already exists.\n";
        return;
    }

    // ---------------- REGISTRATION NUMBER ----------------

    ignoreLine();

    cout << "Enter Registration Number: ";
    getline(cin, newStudent.registrationNo);

    while (newStudent.registrationNo.empty() ||
           findStudentByRegistration(newStudent.registrationNo) != -1)
    {
        if (newStudent.registrationNo.empty())
        {
            cout << "Registration number cannot be empty.\n";
        }
        else
        {
            cout << "Registration number already exists.\n";
        }

        cout << "Enter Registration Number: ";
        getline(cin, newStudent.registrationNo);
    }

    // ---------------- NAME ----------------

    cout << "Enter Student Name: ";
    getline(cin, newStudent.name);

    if (newStudent.name.empty())
    {
        cout << "Student name cannot be empty.\n";
        return;
    }

    // ---------------- FATHER NAME ----------------

    cout << "Enter Father's Name: ";
    getline(cin, newStudent.fatherName);

    // ---------------- EMAIL ----------------

    cout << "Enter Email: ";
    getline(cin, newStudent.email);

    // ---------------- PHONE ----------------

    cout << "Enter Phone: ";
    getline(cin, newStudent.phone);

    // ---------------- ADDRESS ----------------

    cout << "Enter Home Address: ";
    getline(cin, newStudent.address);

    // ---------------- SECTION ----------------

    cout << "Enter Section (A/B/C): ";

    if (!(cin >> newStudent.section))
    {
        cout << "Invalid section.\n";
        clearInput();
        return;
    }

    newStudent.section =
        toupper(static_cast<unsigned char>(newStudent.section));

    if (newStudent.section != 'A' &&
        newStudent.section != 'B' &&
        newStudent.section != 'C')
    {
        cout << "Invalid section. Only A, B, or C is allowed.\n";
        return;
    }

    // ---------------- SEMESTER ----------------

    cout << "Enter Semester: ";

    if (!(cin >> newStudent.semester))
    {
        cout << "Invalid semester.\n";
        clearInput();
        return;
    }

    if (newStudent.semester < 1 ||
        newStudent.semester > 8)
    {
        cout << "Semester must be between 1 and 8.\n";
        return;
    }

    newStudent.active = true;

    students[studentCount] = newStudent;
    studentCount++;

    cout << "\nStudent added successfully.\n";
}


void displayStudent(int index)
{
    cout << "\n----------------------------------------\n";

    cout << "Roll No          : "
         << students[index].rollNo << endl;

    cout << "Registration No  : "
         << students[index].registrationNo << endl;

    cout << "Name             : "
         << students[index].name << endl;

    cout << "Father Name      : "
         << students[index].fatherName << endl;

    cout << "Email            : "
         << students[index].email << endl;

    cout << "Phone            : "
         << students[index].phone << endl;

    cout << "Address          : "
         << students[index].address << endl;

    cout << "Section          : "
         << students[index].section << endl;

    cout << "Semester         : "
         << students[index].semester << endl;

    cout << "Status           : "
         << (students[index].active ? "Active" : "Inactive")
         << endl;

    cout << "----------------------------------------\n";
}


void displayAllStudents()
{
    if (studentCount == 0)
    {
        cout << "\nNo students have been added yet.\n";
        return;
    }

    cout << "\n";
    cout << "========================================\n";
    cout << "              ALL STUDENTS\n";
    cout << "========================================\n";

    for (int i = 0; i < studentCount; i++)
    {
        displayStudent(i);
    }
}

void displayStudentResults(int rollNo)
{
    cout << "\n========== ACADEMIC RECORDS ==========\n";

    bool found = false;

    for (int i = 0; i < resultCount; i++)
    {
        if (results[i].rollNo == rollNo)
        {
            int courseIndex = findCourse(results[i].courseCode);

            cout << "\nCourse Code : "
                 << results[i].courseCode;

            if (courseIndex != -1)
            {
                cout << "\nCourse Name : "
                     << courses[courseIndex].name;
            }

            cout << "\nMarks       : "
                 << results[i].marks;

            found = true;

            cout << "\n----------------------------------------\n";
        }
    }

    if (!found)
    {
        cout << "\nNo academic records found.\n";
    }
}


void displayStudentAttendance(int rollNo)
{
    cout << "\n========== ATTENDANCE RECORD ==========\n";

    bool found = false;

    for (int i = 0; i < attendanceCount; i++)
    {
        if (attendanceRecords[i].rollNo == rollNo)
        {
            float percentage =
                (static_cast<float>(
                    attendanceRecords[i].attendedClasses) /
                 attendanceRecords[i].totalClasses) * 100;

            cout << "\nCourse Code    : "
                 << attendanceRecords[i].courseCode;

            int courseIndex =
                findCourse(attendanceRecords[i].courseCode);

            if (courseIndex != -1)
            {
                cout << "\nCourse Name    : "
                     << courses[courseIndex].name;
            }

            cout << "\nTotal Classes  : "
                 << attendanceRecords[i].totalClasses;

            cout << "\nClasses Attended: "
                 << attendanceRecords[i].attendedClasses;

            cout << "\nPercentage     : "
                 << fixed << setprecision(2)
                 << percentage << "%";

            cout << "\n----------------------------------------\n";

            found = true;
        }
    }

    if (!found)
    {
        cout << "\nNo attendance records found.\n";
    }
}


void displayStudentFee(int rollNo)
{
    cout << "\n========== FEE RECORD ==========\n";

    int index = findFee(rollNo);

    if (index == -1)
    {
        cout << "\nNo fee record found.\n";
        return;
    }

    cout << fixed << setprecision(2);

    cout << "\nTotal Fee     : "
         << fees[index].totalFee << endl;

    cout << "Paid Fee      : "
         << fees[index].paidFee << endl;

    cout << "Remaining Fee : "
         << fees[index].totalFee -
            fees[index].paidFee
         << endl;
}

void studentRecordMenu(int studentIndex)
{
    int choice;

    while (true)
    {
        cout << "\n";
        cout << "========================================\n";
        cout << "           STUDENT RECORD\n";
        cout << "========================================\n";

        cout << "Student : "
             << students[studentIndex].name << endl;

        cout << "Roll No : "
             << students[studentIndex].rollNo << endl;

        cout << "\n";

        cout << "1. Student Information\n";
        cout << "2. Academic Records\n";
        cout << "3. Attendance\n";
        cout << "4. Fee Record\n";
        cout << "5. Back\n";

        cout << "\nEnter Choice: ";

        if (!(cin >> choice))
        {
            cout << "Invalid input.\n";
            clearInput();
            continue;
        }

        if (choice == 1)
        {
            displayStudent(studentIndex);
        }

        else if (choice == 2)
        {
            displayStudentResults(
                students[studentIndex].rollNo);
        }

        else if (choice == 3)
        {
            displayStudentAttendance(
                students[studentIndex].rollNo);
        }

        else if (choice == 4)
        {
            displayStudentFee(
                students[studentIndex].rollNo);
        }

        else if (choice == 5)
        {
            break;
        }

        else
        {
            cout << "Invalid choice.\n";
        }
    }
}

void searchStudent()
{
    int choice;

    while (true)
    {
        cout << "\n";
        cout << "========================================\n";
        cout << "            STUDENT SEARCH\n";
        cout << "========================================\n";

        cout << "1. Search by Roll Number\n";
        cout << "2. Search by Registration Number\n";
        cout << "3. Back\n";

        cout << "\nEnter Choice: ";

        if (!(cin >> choice))
        {
            cout << "Invalid input.\n";
            clearInput();
            continue;
        }

        // --------------------------------------------
        // SEARCH BY ROLL NUMBER
        // --------------------------------------------

        if (choice == 1)
        {
            int rollNo;

            cout << "\nEnter Roll Number: ";

            if (!(cin >> rollNo))
            {
                cout << "Invalid roll number.\n";
                clearInput();
                continue;
            }

            int index = findStudent(rollNo);

            if (index == -1)
            {
                cout << "\nStudent not found.\n";
            }
            else
            {
                cout << "\nStudent found successfully.\n";

                studentRecordMenu(index);
            }
        }

        // --------------------------------------------
        // SEARCH BY REGISTRATION NUMBER
        // --------------------------------------------

        else if (choice == 2)
        {
            string registrationNo;

            ignoreLine();

            cout << "\nEnter Registration Number: ";
            getline(cin, registrationNo);

            if (registrationNo.empty())
            {
                cout << "Registration number cannot be empty.\n";
                continue;
            }

            int index =
                findStudentByRegistration(registrationNo);

            if (index == -1)
            {
                cout << "\nStudent not found.\n";
            }
            else
            {
                cout << "\nStudent found successfully.\n";

                studentRecordMenu(index);
            }
        }

        // --------------------------------------------
        // BACK
        // --------------------------------------------

        else if (choice == 3)
        {
            break;
        }

        else
        {
            cout << "Invalid choice.\n";
        }
    }
}

void updateStudent()
{
    int rollNo;

    cout << "\nEnter Roll Number to update: ";

    if (!(cin >> rollNo))
    {
        cout << "Invalid roll number.\n";
        clearInput();
        return;
    }

    int index = findStudent(rollNo);

    if (index == -1)
    {
        cout << "\nStudent not found.\n";
        return;
    }

    while (true)
    {
        cout << "\n";
        cout << "========================================\n";
        cout << "           UPDATE STUDENT\n";
        cout << "========================================\n";

        cout << "1. Update One Field\n";
        cout << "2. Update Two Fields\n";
        cout << "3. Update Three Fields\n";
        cout << "4. Back\n";

        cout << "\nEnter Choice: ";

        int choice;

        if (!(cin >> choice))
        {
            cout << "Invalid input.\n";
            clearInput();
            continue;
        }

        if (choice == 4)
        {
            break;
        }

        if (choice < 1 || choice > 3)
        {
            cout << "Invalid choice.\n";
            continue;
        }

        int numberOfFields = choice;

        cout << "\nYou can update ";

        if (numberOfFields == 1)
            cout << "1 field.\n";
        else
            cout << numberOfFields << " fields.\n";

        cout << "\n";

        cout << "1. Name\n";
        cout << "2. Father's Name\n";
        cout << "3. Email\n";
        cout << "4. Phone\n";
        cout << "5. Address\n";
        cout << "6. Section\n";
        cout << "7. Semester\n";

        bool selected[8] = {false};

        for (int i = 0; i < numberOfFields; i++)
        {
            int field;

            cout << "\nSelect Field "
                 << i + 1 << ": ";

            if (!(cin >> field))
            {
                cout << "Invalid input.\n";
                clearInput();
                i--;
                continue;
            }

            if (field < 1 || field > 7)
            {
                cout << "Invalid field.\n";
                i--;
                continue;
            }

            if (selected[field])
            {
                cout << "You already selected this field.\n";
                i--;
                continue;
            }

            selected[field] = true;

            // ----------------------------------------
            // NAME
            // ----------------------------------------

            if (field == 1)
            {
                ignoreLine();

                string newName;

                cout << "Enter New Name: ";
                getline(cin, newName);

                if (newName.empty())
                {
                    cout << "Name cannot be empty.\n";
                    selected[field] = false;
                    i--;
                    continue;
                }

                students[index].name = newName;

                cout << "Name updated successfully.\n";
            }

            // ----------------------------------------
            // FATHER NAME
            // ----------------------------------------

            else if (field == 2)
            {
                ignoreLine();

                cout << "Enter New Father's Name: ";

                getline(cin, students[index].fatherName);

                cout << "Father's name updated successfully.\n";
            }

            // ----------------------------------------
            // EMAIL
            // ----------------------------------------

            else if (field == 3)
            {
                ignoreLine();

                cout << "Enter New Email: ";

                getline(cin, students[index].email);

                cout << "Email updated successfully.\n";
            }

            // ----------------------------------------
            // PHONE
            // ----------------------------------------

            else if (field == 4)
            {
                ignoreLine();

                cout << "Enter New Phone: ";

                getline(cin, students[index].phone);

                cout << "Phone updated successfully.\n";
            }

            // ----------------------------------------
            // ADDRESS
            // ----------------------------------------

            else if (field == 5)
            {
                ignoreLine();

                cout << "Enter New Address: ";

                getline(cin, students[index].address);

                cout << "Address updated successfully.\n";
            }

            // ----------------------------------------
            // SECTION
            // ----------------------------------------

            else if (field == 6)
            {
                char newSection;

                cout << "Enter New Section (A/B/C): ";

                if (!(cin >> newSection))
                {
                    cout << "Invalid section.\n";
                    clearInput();
                    selected[field] = false;
                    i--;
                    continue;
                }

                newSection =
                    toupper(
                        static_cast<unsigned char>(
                            newSection));

                if (newSection != 'A' &&
                    newSection != 'B' &&
                    newSection != 'C')
                {
                    cout << "Invalid section.\n";
                    selected[field] = false;
                    i--;
                    continue;
                }

                students[index].section = newSection;

                cout << "Section updated successfully.\n";
            }

            // ----------------------------------------
            // SEMESTER
            // ----------------------------------------

            else if (field == 7)
            {
                int newSemester;

                cout << "Enter New Semester: ";

                if (!(cin >> newSemester))
                {
                    cout << "Invalid semester.\n";
                    clearInput();
                    selected[field] = false;
                    i--;
                    continue;
                }

                if (newSemester < 1 ||
                    newSemester > 8)
                {
                    cout << "Semester must be between 1 and 8.\n";
                    selected[field] = false;
                    i--;
                    continue;
                }

                students[index].semester = newSemester;

                cout << "Semester updated successfully.\n";
            }
        }

        cout << "\nStudent information updated.\n";
    }
}

void deleteStudent()
{
    int rollNo;

    cout << "\nEnter Roll Number to delete: ";

    if (!(cin >> rollNo))
    {
        cout << "Invalid roll number.\n";
        clearInput();
        return;
    }

    int index = findStudent(rollNo);

    if (index == -1)
    {
        cout << "\nStudent not found.\n";
        return;
    }

    // --------------------------------------------
    // REMOVE RESULTS
    // --------------------------------------------

    for (int i = 0; i < resultCount;)
    {
        if (results[i].rollNo == rollNo)
        {
            for (int j = i; j < resultCount - 1; j++)
            {
                results[j] = results[j + 1];
            }

            resultCount--;
        }
        else
        {
            i++;
        }
    }

    // --------------------------------------------
    // REMOVE ATTENDANCE
    // --------------------------------------------

    for (int i = 0; i < attendanceCount;)
    {
        if (attendanceRecords[i].rollNo == rollNo)
        {
            for (int j = i;
                 j < attendanceCount - 1;
                 j++)
            {
                attendanceRecords[j] =
                    attendanceRecords[j + 1];
            }

            attendanceCount--;
        }
        else
        {
            i++;
        }
    }

    // --------------------------------------------
    // REMOVE FEE
    // --------------------------------------------

    for (int i = 0; i < feeCount;)
    {
        if (fees[i].rollNo == rollNo)
        {
            for (int j = i; j < feeCount - 1; j++)
            {
                fees[j] = fees[j + 1];
            }

            feeCount--;
        }
        else
        {
            i++;
        }
    }

    // --------------------------------------------
    // SHIFT STUDENTS
    // --------------------------------------------

    for (int i = index;
         i < studentCount - 1;
         i++)
    {
        students[i] = students[i + 1];
    }

    studentCount--;

    cout << "\nStudent and associated records "
            "deleted successfully.\n";
}

void sortStudentsByRoll()
{
    for (int i = 0;
         i < studentCount - 1;
         i++)
    {
        for (int j = 0;
             j < studentCount - i - 1;
             j++)
        {
            if (students[j].rollNo >
                students[j + 1].rollNo)
            {
                Student temp = students[j];

                students[j] =
                    students[j + 1];

                students[j + 1] =
                    temp;
            }
        }
    }

    cout << "\nStudents sorted by Roll Number.\n";
}

void addCourse()
{
    if (courseCount >= MAX_COURSES)
    {
        cout << "\nCourse storage is full.\n";
        return;
    }

    Course newCourse;

    cout << "\n";
    cout << "========================================\n";
    cout << "               ADD COURSE\n";
    cout << "========================================\n";

    cout << "Enter Course Code: ";
    cin >> newCourse.code;

    if (findCourse(newCourse.code) != -1)
    {
        cout << "Course already exists.\n";
        return;
    }

    ignoreLine();

    cout << "Enter Course Name: ";
    getline(cin, newCourse.name);

    if (newCourse.name.empty())
    {
        cout << "Course name cannot be empty.\n";
        return;
    }

    cout << "Enter Professor Name: ";
    getline(cin, newCourse.professor);

    cout << "Enter Credit Hours: ";

    if (!(cin >> newCourse.creditHours))
    {
        cout << "Invalid credit hours.\n";
        clearInput();
        return;
    }

    if (newCourse.creditHours <= 0)
    {
        cout << "Credit hours must be greater than zero.\n";
        return;
    }

    courses[courseCount] = newCourse;

    courseCount++;

    cout << "\nCourse added successfully.\n";
}


void displayCourses()
{
    if (courseCount == 0)
    {
        cout << "\nNo courses have been added.\n";
        return;
    }

    cout << "\n";
    cout << "==========================================================================\n";

    cout << left
         << setw(12) << "Code"
         << setw(30) << "Course"
         << setw(25) << "Professor"
         << setw(10) << "Credits"
         << endl;

    cout << "===========================================================================\n";

    for (int i = 0; i < courseCount; i++)
    {
        cout << left
             << setw(12) << courses[i].code
             << setw(30) << courses[i].name
             << setw(25) << courses[i].professor
             << setw(10) << courses[i].creditHours
             << endl;
    }
}

void addResult()
{
    if (resultCount >=
        MAX_STUDENTS * MAX_COURSES_PER_STUDENT)
    {
        cout << "Result storage is full.\n";
        return;
    }

    int rollNo;
    string courseCode;
    float marks;

    cout << "\nEnter Student Roll Number: ";

    if (!(cin >> rollNo))
    {
        cout << "Invalid roll number.\n";
        clearInput();
        return;
    }

    int studentIndex = findStudent(rollNo);

    if (studentIndex == -1)
    {
        cout << "Student does not exist.\n";
        return;
    }

    if (!students[studentIndex].active)
    {
        cout << "This student is inactive.\n";
        return;
    }

    cout << "Enter Course Code: ";
    cin >> courseCode;

    if (findCourse(courseCode) == -1)
    {
        cout << "Course does not exist.\n";
        return;
    }

    if (findResult(rollNo, courseCode) != -1)
    {
        cout << "Result for this student and course "
                "already exists.\n";
        return;
    }

    cout << "Enter Marks: ";

    if (!(cin >> marks))
    {
        cout << "Invalid marks.\n";
        clearInput();
        return;
    }

    if (marks < 0 || marks > 100)
    {
        cout << "Marks must be between 0 and 100.\n";
        return;
    }

    Result newResult;

    newResult.rollNo = rollNo;
    newResult.courseCode = courseCode;
    newResult.marks = marks;

    results[resultCount] = newResult;

    resultCount++;

    cout << "\nResult added successfully.\n";
}


void displayResults()
{
    int rollNo;

    cout << "\nEnter Student Roll Number: ";

    if (!(cin >> rollNo))
    {
        cout << "Invalid roll number.\n";
        clearInput();
        return;
    }

    if (findStudent(rollNo) == -1)
    {
        cout << "Student does not exist.\n";
        return;
    }

    displayStudentResults(rollNo);
}

void addAttendance()
{
    if (attendanceCount >=
        MAX_STUDENTS * MAX_COURSES_PER_STUDENT)
    {
        cout << "Attendance storage is full.\n";
        return;
    }

    Attendance record;

    cout << "\nEnter Student Roll Number: ";

    if (!(cin >> record.rollNo))
    {
        cout << "Invalid roll number.\n";
        clearInput();
        return;
    }

    int studentIndex =
        findStudent(record.rollNo);

    if (studentIndex == -1)
    {
        cout << "Student does not exist.\n";
        return;
    }

    if (!students[studentIndex].active)
    {
        cout << "This student is inactive.\n";
        return;
    }

    cout << "Enter Course Code: ";
    cin >> record.courseCode;

    if (findCourse(record.courseCode) == -1)
    {
        cout << "Course does not exist.\n";
        return;
    }

    if (findAttendance(
            record.rollNo,
            record.courseCode) != -1)
    {
        cout << "Attendance record for this student "
                "and course already exists.\n";
        return;
    }

    cout << "Enter Total Classes: ";

    if (!(cin >> record.totalClasses))
    {
        cout << "Invalid total classes.\n";
        clearInput();
        return;
    }

    cout << "Enter Classes Attended: ";

    if (!(cin >> record.attendedClasses))
    {
        cout << "Invalid attended classes.\n";
        clearInput();
        return;
    }

    if (record.totalClasses <= 0)
    {
        cout << "Total classes must be greater than zero.\n";
        return;
    }

    if (record.attendedClasses < 0 ||
        record.attendedClasses >
            record.totalClasses)
    {
        cout << "Invalid attendance values.\n";
        return;
    }

    attendanceRecords[attendanceCount] = record;

    attendanceCount++;

    float percentage =
        (static_cast<float>(
            record.attendedClasses) /
         record.totalClasses) * 100;

    cout << "\nAttendance recorded.\n";

    cout << fixed << setprecision(2);

    cout << "Attendance Percentage: "
         << percentage
         << "%\n";
}


void displayAttendance()
{
    int rollNo;

    cout << "\nEnter Student Roll Number: ";

    if (!(cin >> rollNo))
    {
        cout << "Invalid roll number.\n";
        clearInput();
        return;
    }

    if (findStudent(rollNo) == -1)
    {
        cout << "Student does not exist.\n";
        return;
    }

    displayStudentAttendance(rollNo);
}

void addFee()
{
    if (feeCount >= MAX_STUDENTS)
    {
        cout << "Fee storage is full.\n";
        return;
    }

    int rollNo;

    cout << "\nEnter Student Roll Number: ";

    if (!(cin >> rollNo))
    {
        cout << "Invalid roll number.\n";
        clearInput();
        return;
    }

    int studentIndex =
        findStudent(rollNo);

    if (studentIndex == -1)
    {
        cout << "Student does not exist.\n";
        return;
    }

    if (!students[studentIndex].active)
    {
        cout << "This student is inactive.\n";
        return;
    }

    if (findFee(rollNo) != -1)
    {
        cout << "Fee record already exists for this student.\n";
        return;
    }

    Fee newFee;

    newFee.rollNo = rollNo;

    cout << "Enter Total Fee: ";

    if (!(cin >> newFee.totalFee))
    {
        cout << "Invalid fee amount.\n";
        clearInput();
        return;
    }

    cout << "Enter Amount Paid: ";

    if (!(cin >> newFee.paidFee))
    {
        cout << "Invalid paid amount.\n";
        clearInput();
        return;
    }

    if (newFee.totalFee < 0)
    {
        cout << "Total fee cannot be negative.\n";
        return;
    }

    if (newFee.paidFee < 0)
    {
        cout << "Paid amount cannot be negative.\n";
        return;
    }

    if (newFee.paidFee > newFee.totalFee)
    {
        cout << "Paid amount cannot exceed total fee.\n";
        return;
    }

    fees[feeCount] = newFee;

    feeCount++;

    cout << fixed << setprecision(2);

    cout << "\nFee record added.\n";

    cout << "Remaining Fee: "
         << newFee.totalFee -
            newFee.paidFee
         << endl;
}


void displayFee()
{
    int rollNo;

    cout << "\nEnter Student Roll Number: ";

    if (!(cin >> rollNo))
    {
        cout << "Invalid roll number.\n";
        clearInput();
        return;
    }

    if (findStudent(rollNo) == -1)
    {
        cout << "Student does not exist.\n";
        return;
    }

    displayStudentFee(rollNo);
}

void studentMenu()
{
    int choice;

    while (true)
    {
        cout << "\n";
        cout << "========================================\n";
        cout << "          STUDENT MANAGEMENT\n";
        cout << "========================================\n";

        cout << "1. Add Student\n";
        cout << "2. Display All Students\n";
        cout << "3. Search Student\n";
        cout << "4. Update Student\n";
        cout << "5. Delete Student\n";
        cout << "6. Sort by Roll Number\n";
        cout << "7. Back\n";

        cout << "\nEnter Choice: ";

        if (!(cin >> choice))
        {
            cout << "\nInvalid input.\n";
            clearInput();
            continue;
        }

        if (choice == 1)
        {
            addStudent();
        }

        else if (choice == 2)
        {
            displayAllStudents();
        }

        else if (choice == 3)
        {
            searchStudent();
        }

        else if (choice == 4)
        {
            updateStudent();
        }

        else if (choice == 5)
        {
            deleteStudent();
        }

        else if (choice == 6)
        {
            sortStudentsByRoll();
        }

        else if (choice == 7)
        {
            break;
        }

        else
        {
            cout << "\nInvalid choice.\n";
        }
    }
}

void courseMenu()
{
    int choice;

    while (true)
    {
        cout << "\n";
        cout << "========================================\n";
        cout << "           COURSE MANAGEMENT\n";
        cout << "========================================\n";

        cout << "1. Add Course\n";
        cout << "2. Display Courses\n";
        cout << "3. Back\n";

        cout << "\nEnter Choice: ";

        if (!(cin >> choice))
        {
            cout << "\nInvalid input.\n";
            clearInput();
            continue;
        }

        if (choice == 1)
        {
            addCourse();
        }

        else if (choice == 2)
        {
            displayCourses();
        }

        else if (choice == 3)
        {
            break;
        }

        else
        {
            cout << "\nInvalid choice.\n";
        }
    }
}

void academicMenu()
{
    int choice;

    while (true)
    {
        cout << "\n";
        cout << "========================================\n";
        cout << "          ACADEMIC MANAGEMENT\n";
        cout << "========================================\n";

        cout << "1. Add Result\n";
        cout << "2. View Student Results\n";
        cout << "3. Add Attendance\n";
        cout << "4. View Student Attendance\n";
        cout << "5. Back\n";

        cout << "\nEnter Choice: ";

        if (!(cin >> choice))
        {
            cout << "\nInvalid input.\n";
            clearInput();
            continue;
        }

        if (choice == 1)
        {
            addResult();
        }

        else if (choice == 2)
        {
            displayResults();
        }

        else if (choice == 3)
        {
            addAttendance();
        }

        else if (choice == 4)
        {
            displayAttendance();
        }

        else if (choice == 5)
        {
            break;
        }

        else
        {
            cout << "\nInvalid choice.\n";
        }
    }
}

void feeMenu()
{
    int choice;

    while (true)
    {
        cout << "\n";
        cout << "========================================\n";
        cout << "             FEE MANAGEMENT\n";
        cout << "========================================\n";

        cout << "1. Add Fee Record\n";
        cout << "2. View Fee Record\n";
        cout << "3. Back\n";

        cout << "\nEnter Choice: ";

        if (!(cin >> choice))
        {
            cout << "\nInvalid input.\n";
            clearInput();
            continue;
        }

        if (choice == 1)
        {
            addFee();
        }

        else if (choice == 2)
        {
            displayFee();
        }

        else if (choice == 3)
        {
            break;
        }

        else
        {
            cout << "\nInvalid choice.\n";
        }
    }
}

int main()
{
    int choice;

    while (true)
    {
        cout << "\n";

        cout << "================================================\n";
        cout << "       HONS BATCH 2025 MANAGEMENT SYSTEM\n";
        cout << "================================================\n";

        cout << "1. Student Management\n";
        cout << "2. Course Management\n";
        cout << "3. Academic Management\n";
        cout << "4. Fee Management\n";
        cout << "5. Display All Students\n";
        cout << "6. Exit\n";

        cout << "\nEnter Choice: ";

        if (!(cin >> choice))
        {
            cout << "\nInvalid input. Please enter a number.\n";
            clearInput();
            continue;
        }

        if (choice == 1)
        {
            studentMenu();
        }

        else if (choice == 2)
        {
            courseMenu();
        }

        else if (choice == 3)
        {
            academicMenu();
        }

        else if (choice == 4)
        {
            feeMenu();
        }

        else if (choice == 5)
        {
            displayAllStudents();
        }

        else if (choice == 6)
        {
            cout << "\nExiting program...\n";
            break;
        }

        else
        {
            cout << "\nInvalid choice. Try again.\n";
        }
    }

    return 0;
}