#include <iostream>
#include <fstream>

#include "image_processor.h"

Header* readJPG(const std::string& filename) {
    // open JPG
    std::ifstream inFile = std::ifstream(filename, std::ios::in | std::ios::binary);
    if (!inFile.is_open()) {
        std::cout << "Error opening file\n";
        return nullptr;
    }

    Header* header = new(std::nothrow) Header;

    if (header == nullptr) {
        std::cout << "Error - memmory error.\n";
        inFile.close();
        return nullptr;
    }

    byte last = inFile.get();
    byte current = inFile.get();

    if (last != 0xFF || current != SOI) {
        header->valid = false;
        inFile.close();
        return header;
    }
    byte last = inFile.get();
    byte current = inFile.get();
    while (header->valid == true) {
        if(!inFile) {
            std::cout << "Error file ended early.\n";
            header->valid = false;
            inFile.close();
            return header;
        }
        if (last != 0xFF) {
            std::cout << "Error expected a marker\n";
            header->valid = false;
            inFile.close();
            return header;
        }

        if (current >= APP0 && current <= APP15) {

        }
    }
    {
        /* code */
    }
    
    return header;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Invalid argument\n";
        return 1;
    }
    for (int i = 1; i < argc; ++i) {
        const std::string filename(argv[i]);

        Header* header = readJPG(filename);
        if (header == nullptr) {
            continue;
        }
        if (header->valid == false) {
            std::cout << "Error invalid jpeg\n";
            continue;
        }

        delete header;
    }
    return 0;
}
