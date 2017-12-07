#include "directory.h"
#include "vdi.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
using namespace dir;

namespace dir {
	directory::directory() {}

	void directory::openFile(string fileName, int sizeOfBlock) {
		//assume filename includes type extension
		vdiFile.open(fileName);
		blockSize = sizeOfBlock;

	}

	void directory::dirOpen(int inodeNum) {
		const int size = blockSize;
		inode = inodeNum;
		block = new unsigned char[size];

		//fetch contents
		//`->fetch blocks in directory
		//pointer -> ext2dir entry 2 from ext2.h

		for (int i; i < size; i++) {
			vdiFile << block[i];
		}

		cursor = 0;
	}

	void directory::closeFile() {
		vdiFile.close();
	}

	int directory::dirRead(char buffer[], int byteSize) {
		if (cursor == blockSize) { return 0; }

		//(struct_dir_entry_2*)(pointer block + cursor);
		//1st 4 bytes inode##
		//Next 4 bytes record length
		//Next byte name length
		//Next byte file type
		//Next byte dot
		//4 padding bytes
		//myString(pointer struct pointer mainland)2 = 0
		//ignore 1st 2 records because . and .. -> cursor = 24 to start?
		//do all this in traverse
		//convert like in vdiResources then store in variables?
		int result = 0;
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

		return 1;
	}

	void directory::dirRewind() {
		cursor = 0;
	}

	void directory::dirTraverse() {
		//allocate block of bit space per bitmap
		//when traverse is set in bitmap -> error
		//if finds inode, traverse (for info)
	}

	void directory::fetchBlock(int blockCopy, int iBlock) {
		int b = blockCopy;
		int list;

		//these are likely wrong, but not sure what thye should be
		int apb;
		int temp = iBlock;

		if (b < 12) {
			list = iBlock;
			goto direct;
		}
		else if (b < 12 + apb) {
			list = iBlock + 12;
			b -= 12;
			goto single;
		}
		else if (b < 12 + apb * (1 + apb)) {
			list = iBlock + 13;
			b -= 12 + apb;
			goto dooble;
		}
		else {
			fetchBlock(b, iBlock);
			b -= 12 + apb * (1 + apb);
		}
	dooble:
		fetchBlock(b / apb / apb, temp);
		list = temp;
		b %= apb;
	single:
		int size = b / apb;
		int *sizePointer = &size;
		fetchBlock(list[sizePointer], temp);
		list = temp;
		b %= apb;
	direct:
		int *pointB = &b;
		fetchBlock(list[pointB], temp);
	}

	int directory::getInode() {
		return inode;
	}

	int directory::getRecordLength() {
		return recordLength;
	}

	int directory::getNameLength() {
		return nameLength;
	}

	int directory::getFileType() {
		return fileType;
	}

}