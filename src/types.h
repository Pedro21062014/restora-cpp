#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace restora {

struct FileSignature {
    const char* name;
    const std::vector<const char*> extensions;
    const char* category;
    const std::vector<uint8_t> magic;
    size_t offset;
    uint64_t max_size;
    const std::vector<uint8_t> footer;
};

struct DriveInfo {
    char letter;
    std::string label;
    std::string file_system;
    uint64_t total_size;
    uint64_t free_space;
    bool is_removable;
};

struct RecoveredFile {
    std::string id;
    std::string original_name;
    std::string file_type;
    std::string category;
    uint64_t size;
    std::string path;
    std::string recovered_path;
    std::string status;
    bool is_damaged;
    bool is_thumbnail;
    float confidence;
};

struct ScanConfig {
    std::string drive_path;
    std::string destination;
    std::string scan_type;  // "fast" or "deep"
    std::vector<std::string> categories;
    bool filter_thumbnails;
    bool repair_damaged;
    bool skip_duplicates;
    uint64_t min_file_size;
    uint64_t max_file_size;
};

struct ScanResult {
    std::vector<RecoveredFile> files;
    uint64_t total_size;
    double elapsed_seconds;
};

// Signatures database
std::vector<FileSignature> get_signatures();

// Drive detection
std::vector<DriveInfo> get_drives();

// Scan functions
ScanResult fast_scan(const ScanConfig& config);
ScanResult deep_scan(const ScanConfig& config);

// Recovery
std::vector<RecoveredFile> recover_files(const std::vector<RecoveredFile>& files, const ScanConfig& config);

// File info
std::string format_bytes(uint64_t bytes);
bool is_thumbnail_file(const std::string& path, uint64_t size);

} // namespace restora
