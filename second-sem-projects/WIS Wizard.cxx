#include <iostream>
#include <string>
#include <thread>
#include <chrono>
using namespace std;
class OS {
    private:
    string os_model;
    public:
    string os_pro;
    string os_fwin;
    int ram;
    int hdd;

    OS(string osm) : os_model(osm) {
        cout << "CPU Model: " << os_model << endl;
    }

    void OS_data() {
        cout << "\nCPU Processor: ";
        getline(cin >> ws, os_pro);

        if (os_pro == "Core 2 Duo") {
            cout << "Last Installed Windows: ";
            getline(cin >> ws, os_fwin);
        }

        cout << "CPU RAM (GB): ";
        cin >> ram;

        cout << "CPU Hard Disk (GB): ";
        cin >> hdd;
    }
};


class Windows {
    private:
    int win_key;
    public:
    string win_name;
    int win_rdate;

    Windows(int k) : win_key(k) {
        cout << "\nEnter Windows Product Key: ";
        cin >> win_key;
    }

    void win_data() {
        win_name = "Windows 8.1";
        win_rdate = 2013;

        cout << "\nSelected Windows: " << win_name;
        cout << "\nRelease Date: " << win_rdate << endl;
    }
};


class Int_Wiz : public OS, public Windows {
    public:
    Int_Wiz(string model, int key)
        : OS(model), Windows(key) {
    }
    
    void waitNxt() {
        string next;

        while (true) {

            cout << "\nPress Enter for Next: ";
            cin >> next;

            if (next == "Enter")
                break;

            cout << "Invalid option.\n";
        }
    }

    void installWindows() {
        int option;

        cout << "\n================================";
        cout << "\n  Windows Installation Wizard";
        cout << "\n================================\n";

        while (true) {

            cout << "\n1. Install Windows";
            cout << "\n0. Exit";
            cout << "\nSelect: ";

            cin >> option;


            if (option == 1)
                break;

            if (option == 0)
                exit(0);

            cout << "Invalid choice.\n";
        }

        cout << "\nLicense Terms";
        cout << "\nAccept license agreement";

        waitNxt();

        cout << "\nSelect Installation Type";
        cout << "\n1. Upgrade";
        cout << "\n2. Custom Install";

        int type;

        while (true) {

            cout << "\nSelect: ";
            cin >> type;

            if (type == 2)
                break;

            cout << "Please choose Custom Install.\n";
        }

        cout << "\nAvailable Drives";
        cout << "\n1. Drive C: 100GB";
        cout << "\n2. Drive D: 100GB";
        cout << "\n3. Drive E: 38GB";

        int drive;

        while (true) {

            cout << "\nSelect Drive: ";
            cin >> drive;

            if (drive == 1)
                break;

            cout << "Choose primary partition.\n";
        }

        cout << "\nFormatting C Drive...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(1200));

        cout << "Format Complete.\n";

        waitNxt();

        cout << "\nChoose Windows Edition";
        cout << "\n1. Windows 8.1 Home";
        cout << "\n2. Windows 8.1 Pro";
        cout << "\n3. Windows 8.1 Enterprise";

        int edition;
        cout << "\nSelect Edition: ";
        cin >> edition;

        win_data();

        waitNxt();

        installationStages();

        restart();

        finalSetup();
    }

    void progress(string text) {

        for (int i = 0; i <= 100; i += 10) {

            cout << "\r" << text << " " << i << "%";
            std::this_thread::sleep_for(std::chrono::milliseconds(360));
        }

        cout << endl;
    }


    void installationStages() {

        cout << "\nInstalling Windows...\n";

        progress("Copying Windows files...");
        progress("Getting files ready...");
        progress("Installing features...");
        progress("Installing updates...");
        progress("Finishing up...");


        cout << "\nInstallation completed.\n";
    }

    void restart() {

        string r;
        cout << "\nWindows will now restart...";
        
        while (true) {
            cout << "\nPress any key to continue... ";
            cin >> r;

            if (r == "Enter")
                break;
        }

        cout << "\nRestarting Computer...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(3600));

        cout << "\nStarting Windows...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2100));

        progress("Getting devices ready...");

        cout << "\nGetting ready...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2100));

        cout << "\nPreparing your PC...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(2100));
    }

    void finalSetup() {

        cout << "\nFinalizing your settings...\n";

        std::this_thread::sleep_for(std::chrono::milliseconds(3600));

        cout << "\n=========";
        cout << "\n Welcome ";
        cout << "\n=========\n";
    }
};

int main() {
    Int_Wiz w1("E8500", 0);
    w1.OS_data();
    w1.installWindows();

    return 0;
}