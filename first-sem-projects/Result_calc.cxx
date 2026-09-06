#include<iostream>
using namespace std;
int main() {
    int subj;
    cout << "Enter total number of subjects: ";
    cin >> subj;
    
    int marks[subj];
    int total = 0;
    float average;
    char grade;
    float gpa;
    
    for (int i = 0; i < subj; i++) {
        cout << "Enter marks for subject " << i +1 << " = ";
        cin >> marks[i];
        total += marks[i];
    }
    
    average = total / double(subj);
    
    if (average >= 90)
    grade = 'A';
    else if (average >= 80)
    grade = 'B';
    else if (average >= 70)
    grade = 'C';
    else if (average >= 60)
    grade = 'D';
    else
    grade = 'F';
    
    cout << "\nTotal Marks: " << total;
    cout << "\nAverage Marks: " << average;
    cout << "\nGrade: " << grade;
    
    // Calculate GPA (assuming a 4.0 scale)
    if (average >= 90)
        gpa = 4.0;
    else if (average >= 85)
        gpa = 3.5;
    else if (average >= 80)
        gpa = 3.0;
    else if (average >= 75)
        gpa = 2.5;
    else if (average >= 70)
        gpa = 2.0;
    else if (average >= 65)
        gpa = 1.5;
    else if (average >= 60)
        gpa = 1.0;
    else
        gpa = 0.0;
    
    cout << "\nGPA: " << gpa;
    return 0;
}