#include<iostream>
#include<cmath>

int main() {
    int subj;

    std::cout << "Enter total number of subjects: ";
    std::cin >> subj;
    
    float marks[subj];
    double total {0};
    float average;
    char grade;
    float gpa;
    
    for (int i = 0; i < subj; i++) {
        std::cout << "Enter marks for subject " << i + 1 << " = ";
        std::cin >> marks[i];

        total += marks[i];
    }
    
    average = total / double(subj);
    
    if (average >= 90) {
        grade = 'A+';
    }
    else if (average >= 80) {
        grade = 'A';
    }
    else if (average >= 70) {
        grade = 'B';
    }
    else if (average >= 60) {
        grade = 'C';
    }
    else if (average >= 50) {
        grade = 'D';
    }
    else {
        grade = 'F';
    }

    std::cout << "\nTotal Marks: " << total << "\nAverage Marks: " << average << "\nGrade: " << grade << std::endl;
    
    // Calculate GPA (assuming a 4.0 scale)
    if (average >= 90) {
        gpa = 4.0;
    }
    else if (average >= 85) {
        gpa = 3.5;
    }
    else if (average >= 80) {
        gpa = 3.0;
    }
    else if (average >= 75) {
        gpa = 2.5;
    }
    else if (average >= 70) {
        gpa = 2.0;
    }
    else if (average >= 65) {
        gpa = 1.5;
    }
    else if (average >= 60) {
        gpa = 1.0;
    }
    else {
        gpa = 0.0;
    }

    std::cout << "\nGPA: " << gpa << std::endl;
    return 0;
}