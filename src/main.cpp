#include "types.h"
#include "console.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace restora;

void show_about() {
    console::print_header();
    std::cout << "══════════════════════════════════════════════════════╗\n";
    std::cout << "║                      ABOUT                           ║\n";
    std::cout << "╠══════════════════════════════════════════════════════╣\n";
    std::cout << "║                                                      \n";
    std::cout << "║  Restora - File Recovery Tool v1.1.4                 ║\n";
    std::cout << "║                                                      \n";
    std::cout << "║  A lightweight, native C++ file recovery tool.       ║\n";
    std::cout << "║  Works on any Windows PC with just 2GB RAM.          ║\n";
    std::cout << "║                                                      \n";
    std::cout << "║  Features:                                           ║\n";
    std::cout << "║  • Fast and deep scanning                            ║\n";
    std::cout << "║  • Recover images, videos, audio, documents          ║\n";
    std::cout << "║  • Repair damaged files automatically                ║\n";
    std::cout << "║  • Filter thumbnails and duplicates                  ║\n";
    std::cout << "║  • 32-bit support for old PCs                        ║\n";
    std::cout << "║                                                      \n";
    std::cout << "║  Built with pure C++ and Windows API                 ║\n";
    std::cout << "║  No frameworks, no Electron, no bloat                ║\n";
    std::cout << "║                                                      \n";
    std::cout << "║  © 2026 Restora                                      ║\n";
    std::cout << "╚══════════════════════════════════════════════════════╝\n";
    console::wait_key();
}

void show_settings(ScanConfig& config) {
    console::print_header();
    std::cout << "══════════════════════════════════════════════════════╗\n";
    std::cout << "║                    SETTINGS                          ║\n";
    std::cout << "╠══════════════════════════════════════════════════════╣\n";
    std::cout << "║                                                      \n";
    std::cout << "║  Current Configuration:                              \n";
    std::cout << "║  ────────────────────────────────────────────      \n";
    std::cout << "║  Scan Type:        " << std::left << std::setw(20) << config.scan_type << " ║\n";
    std::cout << "║  Destination:      " << std::left << std::setw(20) << config.destination << " ║\n";
    std::cout << "║  Filter Thumbnails:" << std::left << std::setw(19) << (config.filter_thumbnails ? "Yes" : "No") << " ║\n";
    std::cout << "║  Repair Damaged:   " << std::left << std::setw(19) << (config.repair_damaged ? "Yes" : "No") << " ║\n";
    std::cout << "║  Skip Duplicates:  " << std::left << std::setw(19) << (config.skip_duplicates ? "Yes" : "No") << " ║\n";
    std::cout << "║  Min File Size:    " << std::left << std::setw(14) << format_bytes(config.min_file_size) << " ║\n";
    std::cout << "║  Max File Size:    " << std::left << std::setw(14) << format_bytes(config.max_file_size) << " ║\n";
    std::cout << "║                                                      \n";
    std::cout << "╚══════════════════════════════════════════════════════╝\n";
    console::wait_key();
}

void scan_menu(ScanConfig& config) {
    console::print_header();
    
    auto drives = get_drives();
    
    if (drives.empty()) {
        console::print_error("No drives found!");
        console::wait_key();
        return;
    }
    
    console::print_drives(drives);
    
    std::cout << "\nSelect drive letter: ";
    char drive_letter;
    std::cin >> drive_letter;
    std::cin.ignore(10000, '\n');
    
    drive_letter = toupper(drive_letter);
    
    std::string drive_path = std::string(1, drive_letter) + ":\\";
    
    // Check if drive exists
    bool valid_drive = false;
    for (const auto& drive : drives) {
        if (drive.letter == drive_letter) {
            valid_drive = true;
            break;
        }
    }
    
    if (!valid_drive) {
        console::print_error("Invalid drive letter!");
        console::wait_key();
        return;
    }
    
    // Select scan type
    console::print_header();
    std::cout << "══════════════════════════════════════════════════════╗\n";
    std::cout << "║                 SELECT SCAN TYPE                     ║\n";
    std::cout << "╠══════════════════════════════════════════════════════╣\n";
    std::cout << "║  1. Fast Scan (~2-5 minutes)                         ║\n";
    std::cout << "║  2. Deep Scan (~15-60 minutes)                       ║\n";
    std::cout << "══════════════════════════════════════════════════════╝\n";
    std::cout << "\nEnter choice: ";
    
    int scan_choice;
    std::cin >> scan_choice;
    std::cin.ignore(10000, '\n');
    
    if (scan_choice == 1) {
        config.scan_type = "fast";
    } else if (scan_choice == 2) {
        config.scan_type = "deep";
    } else {
        console::print_error("Invalid choice!");
        console::wait_key();
        return;
    }
    
    // Select categories
    console::print_header();
    std::cout << "══════════════════════════════════════════════════════╗\n";
    std::cout << "║                SELECT FILE TYPES                     ║\n";
    std::cout << "╠══════════════════════════════════════════════════════╣\n";
    std::cout << "║  1. All Files                                        ║\n";
    std::cout << "║  2. Images Only                                      ║\n";
    std::cout << "║  3. Videos Only                                      ║\n";
    std::cout << "║  4. Audio Only                                       ║\n";
    std::cout << "║  5. Documents Only                                   ║\n";
    std::cout << "║  6. Archives Only                                    ║\n";
    std::cout << "╚══════════════════════════════════════════════════════╝\n";
    std::cout << "\nEnter choice: ";
    
    int cat_choice;
    std::cin >> cat_choice;
    std::cin.ignore(10000, '\n');
    
    config.categories.clear();
    if (cat_choice == 1) {
        // All files - empty categories means all
    } else if (cat_choice == 2) {
        config.categories.push_back("images");
    } else if (cat_choice == 3) {
        config.categories.push_back("videos");
    } else if (cat_choice == 4) {
        config.categories.push_back("audio");
    } else if (cat_choice == 5) {
        config.categories.push_back("documents");
    } else if (cat_choice == 6) {
        config.categories.push_back("archives");
    } else {
        console::print_error("Invalid choice!");
        console::wait_key();
        return;
    }
    
    // Select destination
    console::print_header();
    std::cout << "══════════════════════════════════════════════════════╗\n";
    std::cout << "║              SELECT DESTINATION FOLDER               ║\n";
    std::cout << "╠══════════════════════════════════════════════════════╣\n";
    std::cout << "║                                                      \n";
    std::cout << "║  Current: " << std::left << std::setw(40) << config.destination << " ║\n";
    std::cout << "║                                                      \n";
    std::cout << "║  Enter new path or press Enter to keep current:      ║\n";
    std::cout << "║                                                      \n";
    std::cout << "╚══════════════════════════════════════════════════════╝\n";
    std::cout << "\nPath: ";
    
    std::string new_dest;
    std::getline(std::cin, new_dest);
    
    if (!new_dest.empty()) {
        config.destination = new_dest;
    }
    
    // Start scan
    config.drive_path = drive_path;
    
    console::print_header();
    std::cout << "══════════════════════════════════════════════════════╗\n";
    std::cout << "║                  STARTING SCAN                       ║\n";
    std::cout << "╠══════════════════════════════════════════════════════╣\n";
    std::cout << "║                                                      \n";
    std::cout << "║  Drive:          " << std::left << std::setw(30) << drive_path << " ║\n";
    std::cout << "║  Scan Type:      " << std::left << std::setw(30) << config.scan_type << " ║\n";
    std::cout << "║  Destination:    " << std::left << std::setw(30) << config.destination << " ║\n";
    std::cout << "║                                                      \n";
    std::cout << "║  Press any key to start...                           ║\n";
    std::cout << "║                                                      \n";
    std::cout << "══════════════════════════════════════════════════════╝\n";
    console::wait_key();
    
    ScanResult result;
    if (config.scan_type == "fast") {
        result = fast_scan(config);
    } else {
        result = deep_scan(config);
    }
    
    console::print_header();
    console::print_results(result.files);
    
    std::cout << "\n══════════════════════════════════════════════════════╗\n";
    std::cout << "║                                                      \n";
    std::cout << "║  1. Recover All Files                                ║\n";
    std::cout << "║  2. Select Files to Recover                          ║\n";
    std::cout << "║  3. Back to Menu                                     ║\n";
    std::cout << "║                                                      \n";
    std::cout << "╚══════════════════════════════════════════════════════╝\n";
    std::cout << "\nEnter choice: ";
    
    int action;
    std::cin >> action;
    std::cin.ignore(10000, '\n');
    
    if (action == 1) {
        console::print_header();
        console::print_success("Recovering all files...");
        auto recovered = recover_files(result.files, config);
        console::print_success("Recovery complete!");
        console::wait_key();
    } else if (action == 2) {
        // Select specific files
        console::print_header();
        std::cout << "Enter file numbers to recover (comma-separated): ";
        
        std::string input;
        std::getline(std::cin, input);
        
        std::vector<RecoveredFile> selected_files;
        std::stringstream ss(input);
        std::string item;
        
        while (std::getline(ss, item, ',')) {
            try {
                int idx = std::stoi(item) - 1;
                if (idx >= 0 && idx < result.files.size()) {
                    selected_files.push_back(result.files[idx]);
                }
            } catch (...) {
                // Invalid number, skip
            }
        }
        
        if (!selected_files.empty()) {
            console::print_header();
            console::print_success("Recovering selected files...");
            auto recovered = recover_files(selected_files, config);
            console::print_success("Recovery complete!");
        } else {
            console::print_error("No valid files selected!");
        }
        console::wait_key();
    }
}

int main() {
    try {
        // Set console title
    #ifdef _WIN32
        SetConsoleTitleA("Restora - File Recovery Tool");
    #endif
        
        ScanConfig config;
        config.scan_type = "fast";
        config.destination = "C:\\Recovered";
        config.filter_thumbnails = true;
        config.repair_damaged = true;
        config.skip_duplicates = true;
        config.min_file_size = 0;
        config.max_file_size = 5000000000; // 5GB
        
        while (true) {
            console::print_header();
            console::print_menu();
            
            int choice = console::get_user_choice();
            
            switch (choice) {
                case 1:
                    scan_menu(config);
                    break;
                case 2:
                    console::print_header();
                    console::print_drives(get_drives());
                    console::wait_key();
                    break;
                case 3:
                    show_settings(config);
                    break;
                case 4:
                    show_about();
                    break;
                case 0:
                    console::print_header();
                    std::cout << "Thank you for using Restora!\n";
                    std::cout << "Goodbye!\n\n";
                    #ifdef _WIN32
                    system("pause");
                    #endif
                    return 0;
                default:
                    console::print_error("Invalid choice!");
                    console::wait_key();
                    break;
            }
        }
        
        #ifdef _WIN32
        system("pause");
        #endif
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\nFATAL ERROR: " << e.what() << "\n";
        std::cerr << "The application will close. Please try again.\n";
        #ifdef _WIN32
        system("pause");
        #endif
        return 1;
    } catch (...) {
        std::cerr << "\nUNKNOWN FATAL ERROR occurred!\n";
        std::cerr << "The application will close. Please try again.\n";
        #ifdef _WIN32
        system("pause");
        #endif
        return 1;
    }
}
