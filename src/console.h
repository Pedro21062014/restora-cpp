#pragma once

#include "types.h"
#include <string>

namespace restora {
namespace console {

void print_header();
void print_menu();
int get_user_choice();
std::string get_user_input(const std::string& prompt);
void print_drives(const std::vector<DriveInfo>& drives);
void print_results(const std::vector<RecoveredFile>& files);
void print_progress(int percent, const std::string& status);
void print_success(const std::string& msg);
void print_error(const std::string& msg);
void print_warning(const std::string& msg);
void clear_screen();
void wait_key();

} // namespace console
} // namespace restora
