#include <iostream>
#include <fstream>

#include "image_processor.h"

void readQuantizationTable(std::ifstream& inFile, Header* const header) {
    std::cout << "Reading DQT Markerr\n";
    uint length = (inFile.get() << 8) + inFile.get();
    length -= 2;

    while (length > 0) {
        byte tableInfo = inFile.get();
        length -= 1;
        byte tableID = tableInfo & 0x0F;

        if (tableID > 3) {
            std::cout << "Error invalid quantizazion table ID" << (uint)tableID << "\n";
            header->valid = false;
            return;
        }
        header->quantizationTable[tableID].set = true;

        if (tableInfo >> 4 != 0) {
            for (uint i = 0; i < 64; ++i) {
                header->quantizationTable[tableID].table[i] = (inFile.get() << 8) + inFile.get();            
            }
            length -= 128;
        }
        else {
            for (uint i = 0; i < 64; ++i) {
                header->quantizationTable[tableID].table[i] = inFile.get();
            }
            length -= 64;
        }
    }
    
    if (length != 0) {
        std::cout << "Error DQT invalid\n";
    }

}

void readAPPN(std::ifstream& inFile, Header* const header) {
    std::cout << "Reading APPN Marker\n";
    uint length = (inFile.get() << 8) + inFile.get();
    for (uint i = 0; i < length - 2; ++i) {
        inFile.get();
    }
}

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
    last = inFile.get();
    current = inFile.get();
    //read markers
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

        if (current == DQT) {
            readQuantizationTable(inFile, header);
            break;
        }

        if (current >= APP0 && current <= APP15) {
            readAPPN(inFile, header);
            break;
        }


        last = inFile.get();
        current = inFile.get();
    }
    return header;
}

void printHeader(const Header* const header) {
    if (header == nullptr) return;
    std::cout << "DQT============";
    for (uint i = 0; i < 4; ++i) {
        if (header->quantizationTable[i].set) {
            std::cout << "Table ID: " << i << "\n";
            std::cout << "Table Data: " ;
            for (uint j = 0; j < 64; ++j) {
                if (j % 8 == 0) {
                    std::cout << "\n";
                }
                std::cout << header->quantizationTable[i].table[i] << " ";
            }
        }
    }
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
