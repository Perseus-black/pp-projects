#include <iostream>
#include <vector>
using namespace std;

class LogicDevice {
public:
    virtual void compute() = 0;
    virtual void display() const = 0;
    virtual ~LogicDevice() {}
};

// 3x8 Decoder
class Decoder3x8 : public LogicDevice {
    int input;
    vector<int> output;

public:
    Decoder3x8() : input(0), output(8, 0) {}

    void setInput(int in) {
        input = in;
    }

    void compute() override {
        fill(output.begin(), output.end(), 0);
        if (input >= 0 && input < 8)
            output[input] = 1;
    }

    void display() const override {
        cout << "Output (3x8 Decoder): ";
        for (int val : output) cout << val << " ";
        cout << endl;
    }
};

// 2x4 Decoder
class Decoder2x4 : public LogicDevice {
    int input;
    vector<int> output;

public:
    Decoder2x4() : input(0), output(4, 0) {}

    void setInput(int in) {
        input = in;
    }

    void compute() override {
        fill(output.begin(), output.end(), 0);
        if (input >= 0 && input < 4)
            output[input] = 1;
    }

    void display() const override {
        cout << "Output (2x4 Decoder): ";
        for (int val : output) cout << val << " ";
        cout << endl;
    }
};

// 4x2 Encoder
class Encoder4x2 : public LogicDevice {
    vector<int> input;
    int output;

public:
    Encoder4x2() : input(4, 0), output(0) {}

    void setInput(const vector<int>& in) {
        input = in;
    }

    void compute() override {
        for (int i = 0; i < 4; i++) {
            if (input[i] == 1) {
                output = i;
                return;
            }
        }
        output = -1; // invalid case
    }

    void display() const override {
        if (output == -1)
            cout << "Invalid input (not one-hot)" << endl;
        else
            cout << "Encoded Output: " << output << endl;
    }
};

class Encoder8x3 : public LogicDevice {
    vector<int> input;  // 8 inputs
    int output;         // 0–7

public:
    Encoder8x3() : input(8, 0), output(-1) {}

    void setInput(const vector<int>& in) {
        input = in;
    }

    void compute() override {
        output = -1;
        // priority: highest index wins
        for (int i = 7; i >= 0; i--) {
            if (input[i] == 1) {
                output = i;
                return;
            }
        }
    }

    void display() const override {
        if (output == -1) {
            cout << "No active input\n";
            return;
        }

        // convert to 3-bit binary
        cout << "Encoded Output (8x3): ";
        cout << ((output >> 2) & 1) << " "
             << ((output >> 1) & 1) << " "
             << (output & 1) << endl;
    }
};

int main() {
    int choice;

    while (true) {
        cout << "\n--- DLD Simulator ---\n";
        cout << "1. 3x8 Decoder\n";
        cout << "2. 2x4 Decoder\n";
        cout << "3. 4x2 Encoder\n";
        cout << "4. 8x3 Encoder (Priority)\n";
        cout << "0. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 0) break;

        if (choice == 1) {
            int in;
            cout << "Enter 3-bit input (0-7): ";
            cin >> in;

            Decoder3x8 d;
            d.setInput(in);
            d.compute();
            d.display();
        }

        else if (choice == 2) {
            int in;
            cout << "Enter 2-bit input (0-3): ";
            cin >> in;

            Decoder2x4 d;
            d.setInput(in);
            d.compute();
            d.display();
        }

        else if (choice == 3) {
            vector<int> in(4);
            cout << "Enter 4-bit one-hot input (e.g. 0 1 0 0): ";
            for (int i = 0; i < 4; i++) cin >> in[i];

            Encoder4x2 e;
            e.setInput(in);
            e.compute();
            e.display();
        }
        
        else if (choice == 4) {
    vector<int> in(8);
    cout << "Enter 8-bit input (e.g. 0 0 1 0 0 0 0 0): ";
    for (int i = 0; i < 8; i++) cin >> in[i];

    Encoder8x3 e;
    e.setInput(in);
    e.compute();
    e.display();
}

        else {
            cout << "Invalid choice\n";
        }
    }

    return 0;
}