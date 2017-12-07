#include "vdi.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
using namespace vdi;

namespace vdi {

	vdiR::vdiR() {}

	void vdiR::vOpen(string fileName) {
		//assume filename includes type extension
		//low level open
		vdiFile.open(fileName);

		//read file descriptor
		//read from file VDI header
		///vRead(fileName, descriptor, sizeof(descriptor));

		//set cursor to 0
		cursor = 0;

		//get field blocks from HDD
		//allocate map

		//reading 289th through 296th byte in the example to get information for disk size. 
		disk_size = vRead(fileName, buffer, 8); 

		//offset blocks from VDI header(lseek CFD, offest block, SEEK_SET) (from header)

	}

	void vdiR::vClose() {
		//deallocate map

		//close
		vdiFile.close();
	}
	///come back to
	int vdiR::vRead(string fileName, char buffer[], int byteSize) {
		
		//cursor through VDI translation
		vSeek(cursor, 0);

		//low level read
		string target;
		while (getline(vdiFile, target)) {
			vdiFile << target;
		}

		int result = 0;
		
		//if return from read =/= size, error has occured

		for (int i = 0; i < strlen(buffer); i++) {
			if (strlen(buffer) != byteSize)
				break;
			if (buffer[i] == '0') {
				result += 0;
			}
			else if (buffer[i] == '1') {
				result += 1 * 16 ^ i;
			}
			else if (buffer[i] == '2') {
				result += 2 * 16 ^ i;
			}
			else if (buffer[i] == '3') {
				result += 3 * 16 ^ i;
			}
			else if (buffer[i] == '4') {
				result += 4 * 16 ^ i;
			}
			else if (buffer[i] == '5') {
				result += 5 * 16 ^ i;
			}
			else if (buffer[i] == '6') {
				result += 6 * 16 ^ i;
			}
			else if (buffer[i] == '7') {
				result += 7 * 16 ^ i;
			}
			else if (buffer[i] == '8') {
				result += 8 * 16 ^ i;
			}
			else if (buffer[i] == '9') {
				result += 9 * 16 ^ i;
			}
			else if (buffer[i] == 'A') {
				result += 10 * 16 ^ i;
			}
			else if (buffer[i] == 'B') {
				result += 11 * 16 ^ i;
			}
			else if (buffer[i] == 'C') {
				result += 12 * 16 ^ i;
			}
			else if (buffer[i] == 'D') {
				result += 13 * 16 ^ i;
			}
			else if (buffer[i] == 'E') {
				result += 14 * 16 ^ i;
			}
			else if (buffer[i] == 'F') {
				result += 15 * 16 ^ i;
			}
		}
		return result;
	}
	///done?
	void vdiR::vSeek(int offset, int type) {
		//options of:
		//SEEK_SET
		if (type == 0) {
			cursor = offset;
			vdiFile.seekp(cursor);
		}
		//SEEK_END
		else if (type == 1) {
			cursor = disk_size + offset;
			vdiFile.seekp(cursor);
		}
		//SEEK_CUP
		else if (type == 2) {
			cursor = cursor + offset;
			vdiFile.seekp(cursor);
		}
	}
}