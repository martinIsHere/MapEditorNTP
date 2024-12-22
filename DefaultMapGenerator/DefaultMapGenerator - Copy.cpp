#include <iostream>
#include <fstream>
#include <string>

int main()
{
    uint16_t widthOfMap = 20, heightOfMap = 20;
    std::fstream writer;
    //writer.open("D:/source/repos/MapEditorNTP/DefaultMapGenerator/map/Town1.bin", std::ios::out | std::ios::binary);
    //char *buffer = (char*)malloc(2);
    if (!writer) {
        std::cout << "cancer\n";
    }
    //*(buffer+1) = 0x0014;
    //writer.write(buffer, 2);
    //writer << buffer[1];
    //writer.write(buffer, 2);
    //writer << buffer[1];
    for (int y = 0; y < heightOfMap; y++) {
        for (int x = 0; x < widthOfMap; x++) {
            //buffer[1] = 0x01;
            //writer << buffer[1];
            //buffer[1] = 0x06;
            //writer << buffer[1];
            //buffer[0] = 0b00000001;
            //writer << buffer[0];
        }
    }
    writer.close();
    std::cin.get();
}
