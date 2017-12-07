
#ifndef _directory_H
#define _directory_H

#include <iostream>
#include <fstream>
#include <string>
#include "vdi.h"

using namespace std;

namespace dir {

	class directory {
	private:
		int cursor,
			blockSize,
			fd;
		int inode,
			recordLength,
			nameLength,
			fileType;
		unsigned char buffer;
		unsigned char* block;
		fstream vdiFile;
	public:
		directory();
		void openFile(string fileName, int sizeOfBlocks);
		void dirOpen(int inodeNum);
		void closeFile();
		int dirRead(char buffer[], int byteSize);
		void dirRewind();
		void dirTraverse();

		void fetchBlock(int blockCopy, int iBlock);

		int getInode();
		int getRecordLength();
		int getNameLength();
		int getFileType();
	};
};
#endif