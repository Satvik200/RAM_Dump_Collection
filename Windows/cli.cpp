#include <iostream>
#include <cstdlib>
#include <string>

using namespace std;

void clearScreen()
{
    cout <<  "DumpWizard is a Windows command-line tool that creates a memory dump of the current process by iterating through all memory regions and writing \n";
    cout <<  "their contents to a file. \n";
    cout <<  "It uses the Windows APIs to obtain system information, query memory regions, and read their contents. \n";
    cout <<  "The memory dump is saved to a binary file named memory.dmp and a human-readable version is saved to a text file named memory.txt. \n";
    cout <<  "The program returns 0 if successful, or 1 if any error occurs. \n";
    cout << "\n";
    cout <<  "DumpAnalyzer is a Windows command-line tool that reads a memory dump file created by DumpWizard and performs a basic analysis of its contents.\n";
    cout <<  "It reads the binary file and writes its analysis to a text file named analysis.txt. The program returns 0 if successful, or 1 if any error occurs.\n\n\n";
}

void printMenu()
{
    cout << "------------------------------------------------------\n";
    cout << "|                 Team Grit Brokers                   |\n";
    cout << "------------------------------------------------------\n";
    cout << "|  Choose an option:                                  |\n";
    cout << "|                                                     |\n";
    cout << "|  1. Run DumpWizard                                  |\n";
    cout << "|  2. Run DumpAnalyzer                                |\n";
    cout << "|  3. Exit                                            |\n";
    cout << "|                                                     |\n";
    cout << "------------------------------------------------------\n";
    cout << "Enter your choice: ";
}

void printEnding()
{
    cout << "------------------------------------------------------\n";
    cout << "|                 Team Details                        |\n";
    cout << "------------------------------------------------------\n";
    cout << "|  Shreesh Jha (Team Leader)                          |\n";
    cout << "|  Parth Pasricha                                     |\n";
    cout << "|  Satvik Sharma                                      |\n";
    cout << "|  Samarth Bhatia                                     |\n";
    cout << "|  Pratham Dahara                                     |\n";
    cout << "|  Dr. Shikha Gupta (Mentor)                          |\n";
    cout << "------------------------------------------------------\n";
}
int main()
{
    string line;
    int choice = 0;

    while (choice != 3) {
        clearScreen();
        printEnding();
        printMenu();
        
        cin >> choice;

        switch (choice) {
            case 1:
                system("start DumpWizard.exe");
                cout << "Opening DumpWizard... \n";
                system("start notepad.exe memory.txt");
                break;
            case 2:
                system("start DumpAnalyzer.exe");
                cout << "Opening DumpAnalyzer... \n";
                system("start notepad.exe analysis.txt");
                break;
            case 3:
                cout << "Exiting CLI...\n";
                break;
            default:
                cout << "Invalid choice.\n";
        }
        cout << "\nPress Enter key to continue...\n";
        cin.ignore();
        cin.get();
    }
    return 0;
}
