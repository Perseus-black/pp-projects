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
    
    return 0;
}