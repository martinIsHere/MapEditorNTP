#include <iostream>
#include <fstream>
#include <string>

int main()
{
    int xMax = 48, yMax = 48;
    std::fstream writer;
    writer.open("D:/source/repos/The Narrator/The Narrator/res/map/Town1.txt", std::ios::out | std::ios::binary);
    if (!writer) {
        std::cout << "cancer\n";
    }
    writer << std::to_string(xMax) << ' ' << std::to_string(yMax) << ' ';
    for (int y = 0; y < yMax; y++) {
        for (int x = 0; x < xMax; x++) {
            writer << '0' << ' ' << '0' << ' ' << '0' << ' ';
        }
    }
    writer.close();
    std::cin.get();
}