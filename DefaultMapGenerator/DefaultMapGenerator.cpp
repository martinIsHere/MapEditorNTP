#include <iostream>
#include <fstream>
#include <string>

int main()
{
    uint16_t widthOfMap = 20, heightOfMap = 20;
    std::fstream writer;
    writer.open("D:/source/repos/MapEditorNTP/DefaultMapGenerator/map/Town1.bin", std::ios::out | std::ios::binary);
    char* buffer = (char*)malloc(2);
    buffer[0]=0;
    buffer[1]=0;
    if (!writer) {
        std::cout << "cancer\n";
    }
    buffer[1] = 0x0014;
    writer.write(buffer, 2);
    buffer[1] = 0x0014;
    writer.write(buffer, 2);
    for (int y = 0; y < heightOfMap; y++) {
        for (int x = 0; x < widthOfMap; x++) {
            buffer[0] = 0x00;
            buffer[1] = 0x12;
            writer.write(buffer, 2);
            buffer[0] = 0x00;
            buffer[1] = 0x01;
            writer.write(buffer, 2);
            buffer[0] = 0x00;
            buffer[1] = 0x06;
            writer.write(buffer, 2);
            buffer[0] = 0b00000001;
            writer.write(buffer, 1);
        }
    }
    free(buffer);
    writer.close();
}
