#include "types.h"
#include "console.h"
#include <fstream>
#include <algorithm>
#include <cstring>
#include <chrono>
#include <stdexcept>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

namespace restora {

std::vector<FileSignature> get_signatures() {
    return {
        {"JPEG", {".jpg", ".jpeg"}, "images", {0xFF, 0xD8, 0xFF}, 0, 20000000, {0xFF, 0xD9}},
        {"PNG", {".png"}, "images", {0x89, 0x50, 0x4E, 0x47}, 0, 50000000, {0x49, 0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82}},
        {"GIF", {".gif"}, "images", {'G', 'I', 'F', '8'}, 0, 20000000, {}},
        {"BMP", {".bmp"}, "images", {'B', 'M'}, 0, 100000000, {}},
        {"WEBP", {".webp"}, "images", {'R', 'I', 'F', 'F'}, 0, 20000000, {}},
        {"TIFF", {".tiff", ".tif"}, "images", {0x49, 0x49, 0x2A, 0x00}, 0, 100000000, {}},
        {"MP4", {".mp4"}, "videos", {0x00, 0x00, 0x00}, 0, 4000000000, {}},
        {"AVI", {".avi"}, "videos", {'R', 'I', 'F', 'F'}, 0, 4000000000, {}},
        {"MKV", {".mkv"}, "videos", {0x1A, 0x45, 0xDF, 0xA3}, 0, 4000000000, {}},
        {"MOV", {".mov"}, "videos", {0x00, 0x00, 0x00, 0x14, 'f', 't', 'y', 'p'}, 0, 4000000000, {}},
        {"MP3", {".mp3"}, "audio", {0xFF, 0xFB}, 0, 50000000, {}},
        {"MP3v2", {".mp3"}, "audio", {'I', 'D', '3'}, 0, 50000000, {}},
        {"WAV", {".wav"}, "audio", {'R', 'I', 'F', 'F'}, 0, 2000000000, {}},
        {"FLAC", {".flac"}, "audio", {'f', 'L', 'a', 'C'}, 0, 500000000, {}},
        {"OGG", {".ogg"}, "audio", {'O', 'g', 'g', 'S'}, 0, 200000000, {}},
        {"PDF", {".pdf"}, "documents", {'%', 'P', 'D', 'F'}, 0, 500000000, {0x25, 0x25, 'E', 'O', 'F'}},
        {"DOCX", {".docx"}, "documents", {0x50, 0x4B, 0x03, 0x04}, 0, 200000000, {}},
        {"DOC", {".doc"}, "documents", {0xD0, 0xCF, 0x11, 0xE0}, 0, 200000000, {}},
        {"XLSX", {".xlsx"}, "documents", {0x50, 0x4B, 0x03, 0x04}, 0, 200000000, {}},
        {"PPTX", {".pptx"}, "documents", {0x50, 0x4B, 0x03, 0x04}, 0, 200000000, {}},
        {"ZIP", {".zip"}, "archives", {0x50, 0x4B, 0x03, 0x04}, 0, 4000000000, {}},
        {"RAR", {".rar"}, "archives", {'R', 'a', 'r', '!'}, 0, 4000000000, {}},
        {"7Z", {".7z"}, "archives", {0x37, 0x7A, 0xBC, 0xAF}, 0, 4000000000, {}}
    };
}

std::vector<DriveInfo> get_drives() {
    std::vector<DriveInfo> drives;
    
#ifdef _WIN32
    try {
        DWORD drives_mask = GetLogicalDrives();
        for (int i = 0; i < 26; i++) {
            if (drives_mask & (1 << i)) {
                try {
                    DriveInfo drive;
                    drive.letter = 'A' + i;
                    drive.label = std::string(1, drive.letter) + ":\\";
                    
                    char volume_name[MAX_PATH];
                    char fs_name[MAX_PATH];
                    DWORD serial = 0, max_comp = 0, flags = 0;
                    volume_name[0] = '\0';
                    fs_name[0] = '\0';
                    
                    if (GetVolumeInformationA(drive.label.c_str(), volume_name, MAX_PATH, 
                                      &serial, &max_comp, &flags, fs_name, MAX_PATH)) {
                        drive.file_system = fs_name[0] ? fs_name : "Unknown";
                        if (volume_name[0]) {
                            drive.label = std::string(volume_name) + " (" + drive.label + ")";
                        }
                    } else {
                        drive.file_system = "Unknown";
                    }
                    
                    ULARGE_INTEGER free_bytes, total_bytes;
                    free_bytes.QuadPart = 0;
                    total_bytes.QuadPart = 0;
                    if (GetDiskFreeSpaceExA(drive.label.c_str(), &free_bytes, &total_bytes, nullptr)) {
                        drive.total_size = total_bytes.QuadPart;
                        drive.free_space = free_bytes.QuadPart;
                    }
                    
                    UINT drive_type = GetDriveTypeA(drive.label.c_str());
                    drive.is_removable = (drive_type == DRIVE_REMOVABLE);
                    
                    drives.push_back(drive);
                } catch (...) {
                    // Skip this drive
                }
            }
        }
    } catch (...) {
        // Return empty or partial list
    }
#else
    DIR* dir = opendir("/media");
    if (dir) {
        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            if (entry->d_name[0] != '.') {
                DriveInfo drive;
                drive.letter = entry->d_name[0];
                drive.label = std::string("/media/") + entry->d_name;
                drive.file_system = "ext4";
                drive.total_size = 0;
                drive.free_space = 0;
                drive.is_removable = true;
                drives.push_back(drive);
            }
        }
        closedir(dir);
    }
#endif
    
    return drives;
}

bool match_signature(const std::vector<uint8_t>& header, const FileSignature& sig) {
    if (sig.magic.empty()) return true;
    
    size_t start = sig.offset;
    size_t end = start + sig.magic.size();
    
    if (end > header.size()) return false;
    
    for (size_t i = 0; i < sig.magic.size(); i++) {
        if (header[start + i] != sig.magic[i]) return false;
    }
    
    return true;
}

bool check_footer(const std::string& path, const FileSignature& sig) {
    try {
        if (sig.footer.empty()) return false;
        
        std::ifstream file(path, std::ios::binary | std::ios::ate);
        if (!file.is_open()) return false;
        
        uint64_t file_size = file.tellg();
        if (file_size < sig.footer.size()) return false;
        
        file.seekg(static_cast<std::streamoff>(-static_cast<std::streamoff>(sig.footer.size())), std::ios::end);
        std::vector<uint8_t> footer_bytes(sig.footer.size());
        file.read(reinterpret_cast<char*>(footer_bytes.data()), sig.footer.size());
        
        return footer_bytes != sig.footer;
    } catch (...) {
        return false;
    }
}

bool is_thumbnail_file(const std::string& path, uint64_t size) {
    std::string lower_path = path;
    std::transform(lower_path.begin(), lower_path.end(), lower_path.begin(), ::tolower);
    
    const std::vector<std::string> thumb_patterns = {
        "thumb", "thumbnail", ".thumb", "tn_", "_tn", "preview", "icon", "cache", ".tmp"
    };
    
    for (const auto& pattern : thumb_patterns) {
        if (lower_path.find(pattern) != std::string::npos) return true;
    }
    
    if (size < 5000 && (lower_path.find(".jpg") != std::string::npos || 
                        lower_path.find(".png") != std::string::npos)) {
        return true;
    }
    
    return false;
}

std::string generate_id() {
    static int counter = 0;
    return std::string("file_") + std::to_string(++counter) + "_" + 
           std::to_string(std::chrono::steady_clock::now().time_since_epoch().count() % 100000);
}

ScanResult scan_directory(const std::string& dir_path, const ScanConfig& config, bool is_fast) {
    ScanResult result;
    auto signatures = get_signatures();
    auto start_time = std::chrono::steady_clock::now();
    
#ifdef _WIN32
    WIN32_FIND_DATAA find_data;
    std::string search_path = dir_path + "\\*";
    HANDLE hFind = FindFirstFileA(search_path.c_str(), &find_data);
    
    if (hFind == INVALID_HANDLE_VALUE) return result;
    
    do {
        try {
            if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;
            
            std::string file_path = dir_path + "\\" + find_data.cFileName;
            
            if (find_data.dwFileAttributes & (FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_HIDDEN)) {
                if (config.scan_type == "fast") continue;
            }
            
            uint64_t file_size = ((uint64_t)find_data.nFileSizeHigh << 32) | find_data.nFileSizeLow;
            
            if (file_size < config.min_file_size || file_size > config.max_file_size) continue;
            if (config.filter_thumbnails && is_thumbnail_file(file_path, file_size)) continue;
            
            std::ifstream file(file_path, std::ios::binary);
            if (!file.is_open()) continue;
            
            std::vector<uint8_t> header(512);
            file.read(reinterpret_cast<char*>(header.data()), 512);
            size_t bytes_read = file.gcount();
            
            if (bytes_read < 4) continue;
            
            for (const auto& sig : signatures) {
                if (match_signature(header, sig)) {
                    if (!config.categories.empty()) {
                        bool category_match = false;
                        for (const auto& cat : config.categories) {
                            if (cat == sig.category) {
                                category_match = true;
                                break;
                            }
                        }
                        if (!category_match) continue;
                    }
                    
                    RecoveredFile recovered;
                    recovered.id = generate_id();
                    recovered.original_name = find_data.cFileName;
                    recovered.file_type = sig.extensions[0];
                    recovered.category = sig.category;
                    recovered.size = file_size;
                    recovered.path = file_path;
                    recovered.status = "found";
                    recovered.is_damaged = check_footer(file_path, sig);
                    recovered.is_thumbnail = false;
                    recovered.confidence = recovered.is_damaged ? 0.6f : 0.95f;
                    
                    if (recovered.is_damaged && config.repair_damaged) {
                        recovered.status = "damaged";
                    }
                    
                    result.files.push_back(recovered);
                    result.total_size += file_size;
                    break;
                }
            }
        } catch (...) {
            // Skip this file
        }
    } while (FindNextFileA(hFind, &find_data));
    
    FindClose(hFind);
#else
    DIR* dir = opendir(dir_path.c_str());
    if (!dir) return result;
    
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        try {
            if (entry->d_name[0] == '.') continue;
            
            std::string file_path = dir_path + "/" + entry->d_name;
            
            struct stat file_stat;
            if (stat(file_path.c_str(), &file_stat) != 0) continue;
            if (!S_ISREG(file_stat.st_mode)) continue;
            
            uint64_t file_size = file_stat.st_size;
            
            if (file_size < config.min_file_size || file_size > config.max_file_size) continue;
            if (config.filter_thumbnails && is_thumbnail_file(file_path, file_size)) continue;
            
            std::ifstream file(file_path, std::ios::binary);
            if (!file.is_open()) continue;
            
            std::vector<uint8_t> header(512);
            file.read(reinterpret_cast<char*>(header.data()), 512);
            size_t bytes_read = file.gcount();
            
            if (bytes_read < 4) continue;
            
            for (const auto& sig : signatures) {
                if (match_signature(header, sig)) {
                    if (!config.categories.empty()) {
                        bool category_match = false;
                        for (const auto& cat : config.categories) {
                            if (cat == sig.category) {
                                category_match = true;
                                break;
                            }
                        }
                        if (!category_match) continue;
                    }
                    
                    RecoveredFile recovered;
                    recovered.id = generate_id();
                    recovered.original_name = entry->d_name;
                    recovered.file_type = sig.extensions[0];
                    recovered.category = sig.category;
                    recovered.size = file_size;
                    recovered.path = file_path;
                    recovered.status = "found";
                    recovered.is_damaged = check_footer(file_path, sig);
                    recovered.is_thumbnail = false;
                    recovered.confidence = recovered.is_damaged ? 0.6f : 0.95f;
                    
                    if (recovered.is_damaged && config.repair_damaged) {
                        recovered.status = "damaged";
                    }
                    
                    result.files.push_back(recovered);
                    result.total_size += file_size;
                    break;
                }
            }
        } catch (...) {
            // Skip this file
        }
    }
    
    closedir(dir);
#endif
    
    auto end_time = std::chrono::steady_clock::now();
    result.elapsed_seconds = std::chrono::duration<double>(end_time - start_time).count();
    
    return result;
}

ScanResult fast_scan(const ScanConfig& config) {
    console::print_header();
    console::print_success("Starting fast scan...");
    
    ScanResult result = scan_directory(config.drive_path, config, true);
    
    console::print_progress(100, "Scan complete");
    console::print_success("\nScan finished in " + std::to_string(result.elapsed_seconds) + " seconds");
    console::print_success("Found " + std::to_string(result.files.size()) + " files");
    console::print_success("Total size: " + format_bytes(result.total_size));
    
    return result;
}

ScanResult deep_scan(const ScanConfig& config) {
    console::print_header();
    console::print_success("Starting deep scan...");
    
    ScanResult result;
    auto start_time = std::chrono::steady_clock::now();
    
    std::vector<std::string> directories;
    directories.push_back(config.drive_path);
    
    int progress = 0;
    
    while (!directories.empty()) {
        std::string current_dir = directories.back();
        directories.pop_back();
        
        try {
            ScanResult dir_result = scan_directory(current_dir, config, false);
            result.files.insert(result.files.end(), dir_result.files.begin(), dir_result.files.end());
            result.total_size += dir_result.total_size;
        } catch (...) {
            // Skip this directory
        }
        
#ifdef _WIN32
        WIN32_FIND_DATAA find_data;
        std::string search_path = current_dir + "\\*";
        HANDLE hFind = FindFirstFileA(search_path.c_str(), &find_data);
        
        if (hFind != INVALID_HANDLE_VALUE) {
            do {
                try {
                    if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                        if (strcmp(find_data.cFileName, ".") != 0 && 
                            strcmp(find_data.cFileName, "..") != 0) {
                            directories.push_back(current_dir + "\\" + find_data.cFileName);
                        }
                    }
                } catch (...) {
                    // Skip
                }
            } while (FindNextFileA(hFind, &find_data));
            FindClose(hFind);
        }
#else
        DIR* dir = opendir(current_dir.c_str());
        if (dir) {
            struct dirent* entry;
            while ((entry = readdir(dir)) != nullptr) {
                if (entry->d_name[0] == '.') continue;
                
                std::string entry_path = current_dir + "/" + entry->d_name;
                struct stat entry_stat;
                if (stat(entry_path.c_str(), &entry_stat) == 0 && S_ISDIR(entry_stat.st_mode)) {
                    directories.push_back(entry_path);
                }
            }
            closedir(dir);
        }
#endif
        
        progress = (progress + 1 < 95) ? progress + 1 : 95;
        console::print_progress(progress, "Scanning " + current_dir.substr(0, 30) + "...");
    }
    
    auto end_time = std::chrono::steady_clock::now();
    result.elapsed_seconds = std::chrono::duration<double>(end_time - start_time).count();
    
    console::print_progress(100, "Scan complete");
    console::print_success("\nDeep scan finished in " + std::to_string(result.elapsed_seconds) + " seconds");
    console::print_success("Found " + std::to_string(result.files.size()) + " files");
    console::print_success("Total size: " + format_bytes(result.total_size));
    
    return result;
}

std::string format_bytes(uint64_t bytes) {
    const char* sizes[] = {"B", "KB", "MB", "GB", "TB"};
    int i = 0;
    double dbl_bytes = bytes;
    
    while (dbl_bytes >= 1024 && i < 4) {
        dbl_bytes /= 1024;
        i++;
    }
    
    char buffer[50];
    snprintf(buffer, sizeof(buffer), "%.2f %s", dbl_bytes, sizes[i]);
    return std::string(buffer);
}

} // namespace restora
