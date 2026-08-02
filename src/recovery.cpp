#include "types.h"
#include "console.h"
#include <fstream>
#include <filesystem>

namespace restora {

std::vector<RecoveredFile> recover_files(const std::vector<RecoveredFile>& files, const ScanConfig& config) {
    std::vector<RecoveredFile> recovered;
    
    // Create destination directory
#ifdef _WIN32
    _mkdir(config.destination.c_str());
#else
    mkdir(config.destination.c_str(), 0755);
#endif
    
    int success_count = 0;
    
    for (const auto& file : files) {
        std::string category_dir = config.destination + "/" + file.category;
#ifdef _WIN32
        _mkdir(category_dir.c_str());
#else
        mkdir(category_dir.c_str(), 0755);
#endif
        
        // Generate unique filename
        std::string base_name = file.original_name;
        size_t dot_pos = base_name.find_last_of('.');
        if (dot_pos != std::string::npos) {
            base_name = base_name.substr(0, dot_pos);
        }
        
        std::string recovered_name = base_name + "_" + file.id.substr(0, 8) + file.file_type;
        std::string dest_path = category_dir + "/" + recovered_name;
        
        // Copy file
        std::ifstream src(file.path, std::ios::binary);
        std::ofstream dst(dest_path, std::ios::binary);
        
        if (src.is_open() && dst.is_open()) {
            dst << src.rdbuf();
            src.close();
            dst.close();
            
            RecoveredFile rec = file;
            rec.recovered_path = dest_path;
            rec.status = "recovered";
            recovered.push_back(rec);
            success_count++;
        } else {
            console::print_error("Failed to copy: " + file.original_name);
        }
    }
    
    console::print_success("\nRecovered " + std::to_string(success_count) + " of " + 
                          std::to_string(files.size()) + " files");
    console::print_success("Saved to: " + config.destination);
    
    return recovered;
}

} // namespace restora
