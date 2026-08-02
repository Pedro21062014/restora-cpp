#include "console.h"
#include <iostream>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

namespace restora {
namespace console {

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void print_header() {
    clear_screen();
    std::cout << "\n";
    std::cout << "══════════════════════════════════════════════════════╗\n";
    std::cout << "║                                                      ║\n";
    std::cout << "║         RESTORA - File Recovery Tool v1.0.1          ║\n";
    std::cout << "║                                                      \n";
    std::cout << "║    Native C++ • Ultra Light • Works on Old PCs   ║\n";
    std::cout << "║                                                      ║\n";
    std::cout << "╚══════════════════════════════════════════════════════╝\n";
    std::cout << "\n";
}

void print_menu() {
    std::cout << "\n";
    std::cout << "══════════════════════════════════════════════════════╗\n";
    std::cout << "║                    MAIN MENU                         ║\n";
    std::cout << "╠══════════════════════════════════════════════════════╣\n";
    std::cout << "║  1. Scan Drive                                       ║\n";
    std::cout << "║  2. List Drives                                      ║\n";
    std::cout << "║  3. Settings                                         ║\n";
    std::cout << "║  4. About                                            ║\n";
    std::cout << "║  0. Exit                                             ║\n";
    std::cout << "╚══════════════════════════════════════════════════════╝\n";
    std::cout << "\n";
}

int get_user_choice() {
    int choice = -1;
    std::cout << "Enter choice: ";
    while (!(std::cin >> choice) || choice < 0 || choice > 4) {
        std::cout << "Invalid choice. Try again: ";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
    std::cin.ignore(10000, '\n');
    return choice;
}

std::string get_user_input(const std::string& prompt) {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return input;
}

void print_drives(const std::vector<DriveInfo>& drives) {
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════════╗\n";
    std::cout << "║                  AVAILABLE DRIVES                    ║\n";
    std::cout << "╠══════════════════════════════════════════════════════╣\n";
    
    for (const auto& drive : drives) {
        std::cout << "║  " << drive.letter << ": " << std::left << std::setw(30) << drive.label;
        std::cout << std::right << std::setw(15) << drive.file_system << "  ║\n";
        
        std::cout << "║     Size: " << std::left << std::setw(25) << format_bytes(drive.total_size);
        std::cout << std::right << std::setw(15) << format_bytes(drive.free_space) << " free  ║\n";
        
        std::cout << "║     Type: " << (drive.is_removable ? "Removable" : "Fixed") << std::endl;
        std::cout << "║                                                      ║\n";
    }
    
    std::cout << "╚══════════════════════════════════════════════════════╝\n";
}

void print_results(const std::vector<RecoveredFile>& files) {
    if (files.empty()) {
        std::cout << "\nNo files found.\n";
        return;
    }
    
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════════╗\n";
    std::cout << "║               RECOVERED FILES (" 
              << std::setw(5) << files.size() << ")                      ║\n";
    std::cout << "╠══════════════════════════════════════════════════════╣\n";
    
    for (size_t i = 0; i < files.size(); i++) {
        const auto& f = files[i];
        std::cout << "║  " << (i + 1) << ". " << std::left << std::setw(30) << f.original_name;
        std::cout << std::right << std::setw(10) << format_bytes(f.size) << "  ║\n";
        std::cout << "║     Type: " << std::left << std::setw(20) << f.file_type;
        std::cout << std::right << std::setw(20) << f.category << "  \n";
        std::cout << "║     Status: " << std::left << std::setw(35) << f.status << "  ║\n";
        std::cout << "║                                                      ║\n";
    }
    
    std::cout << "╚══════════════════════════════════════════════════════╝\n";
}

void print_progress(int percent, const std::string& status) {
    std::cout << "\r[";
    int pos = percent / 2;
    for (int i = 0; i < 50; i++) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << std::setw(3) << percent << "% " << status;
    std::cout.flush();
}

void print_success(const std::string& msg) {
    std::cout << "\n[+] " << msg << "\n";
}

void print_error(const std::string& msg) {
    std::cout << "\n[-] ERROR: " << msg << "\n";
}

void print_warning(const std::string& msg) {
    std::cout << "\n[!] WARNING: " << msg << "\n";
}

void wait_key() {
    std::cout << "\nPress any key to continue...";
#ifdef _WIN32
    _getch();
#else
    char c;
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    c = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif
    std::cout << "\n";
}

} // namespace console
} // namespace restora
