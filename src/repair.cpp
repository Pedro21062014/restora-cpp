#include "types.h"
#include <fstream>
#include <algorithm>

namespace restora {

void repair_jpeg(const std::string& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) return;
    
    uint64_t file_size = file.tellg();
    file.seekg(0);
    
    std::vector<uint8_t> data(file_size);
    file.read(reinterpret_cast<char*>(data.data()), file_size);
    file.close();
    
    bool modified = false;
    
    // Check SOI marker
    if (data.size() >= 2 && !(data[0] == 0xFF && data[1] == 0xD8)) {
        std::vector<uint8_t> new_data = {0xFF, 0xD8, 0xFF, 0xE0};
        new_data.insert(new_data.end(), data.begin(), data.end());
        data = new_data;
        modified = true;
    }
    
    // Check EOI marker
    if (data.size() >= 2) {
        size_t len = data.size();
        if (!(data[len-2] == 0xFF && data[len-1] == 0xD9)) {
            data.push_back(0xFF);
            data.push_back(0xD9);
            modified = true;
        }
    }
    
    if (modified) {
        std::ofstream out(path, std::ios::binary);
        if (out.is_open()) {
            out.write(reinterpret_cast<const char*>(data.data()), data.size());
            out.close();
        }
    }
}

void repair_png(const std::string& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) return;
    
    uint64_t file_size = file.tellg();
    file.seekg(0);
    
    std::vector<uint8_t> data(file_size);
    file.read(reinterpret_cast<char*>(data.data()), file_size);
    file.close();
    
    // Check PNG signature
    std::vector<uint8_t> png_sig = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
    if (data.size() < 8 || !std::equal(png_sig.begin(), png_sig.end(), data.begin())) {
        std::vector<uint8_t> new_data = png_sig;
        new_data.insert(new_data.end(), data.begin(), data.end());
        data = new_data;
    }
    
    // Ensure IEND chunk
    std::vector<uint8_t> iend = {0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E, 0x44, 
                                  0xAE, 0x42, 0x60, 0x82};
    if (data.size() < 12 || !std::equal(iend.rbegin(), iend.rend(), data.rbegin())) {
        data.insert(data.end(), iend.begin(), iend.end());
    }
    
    std::ofstream out(path, std::ios::binary);
    if (out.is_open()) {
        out.write(reinterpret_cast<const char*>(data.data()), data.size());
        out.close();
    }
}

void repair_pdf(const std::string& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) return;
    
    uint64_t file_size = file.tellg();
    file.seekg(0);
    
    std::vector<uint8_t> data(file_size);
    file.read(reinterpret_cast<char*>(data.data()), file_size);
    file.close();
    
    bool modified = false;
    
    // Check PDF header
    std::string header(data.begin(), data.begin() + std::min((size_t)5, data.size()));
    if (header.find("%PDF") == std::string::npos) {
        std::vector<uint8_t> pdf_header = {'%', 'P', 'D', 'F', '-', '1', '.', '4', '\n'};
        data.insert(data.begin(), pdf_header.begin(), pdf_header.end());
        modified = true;
    }
    
    // Check EOF marker
    std::string data_str(data.begin(), data.end());
    if (data_str.find("%%EOF") == std::string::npos) {
        std::vector<uint8_t> eof_marker = {'\n', '%', '%', 'E', 'O', 'F', '\n'};
        data.insert(data.end(), eof_marker.begin(), eof_marker.end());
        modified = true;
    }
    
    if (modified) {
        std::ofstream out(path, std::ios::binary);
        if (out.is_open()) {
            out.write(reinterpret_cast<const char*>(data.data()), data.size());
            out.close();
        }
    }
}

void repair_file(const std::string& path, const std::string& file_type) {
    std::string ext = file_type;
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    
    if (ext == ".jpg" || ext == ".jpeg") {
        repair_jpeg(path);
    } else if (ext == ".png") {
        repair_png(path);
    } else if (ext == ".pdf") {
        repair_pdf(path);
    }
}

} // namespace restora
