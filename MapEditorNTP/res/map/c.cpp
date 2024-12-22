#include <iostream>
#include <fstream>

int main(){
	std::fstream writer;
	writer.open("Town1.txt", std::ios::out | std::ios::binary);
	writer << 48 << " " << 48;
	for (int y = 0; y < 48; y++) {
		for (int x = 0; x < 48; x++) {
			writer << " " << 0 << " " << 0 << " " << 0;
		}
	}

	return 0;
}