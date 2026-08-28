#include <iostream>
using namespace std;

int main() {
    float t_marks, ob_marks;

    cout << "Enter the total marks: ";
    cin >> t_marks;

    cout << "Enter the obtained marks: ";
    cin >> ob_marks;

    float percentage = (ob_marks / t_marks) * 100.0;
    float result = (percentage / 100.0) * t_marks;

    cout << "Percentage: " << percentage << "%" << endl;
    cout << "Marks obtained: " << result << endl;

    return 0;
}