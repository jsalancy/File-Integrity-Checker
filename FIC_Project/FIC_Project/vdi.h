#ifndef _vdi_H
#define _vdi_H

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

namespace vdi {

	class vdiR {
	private:
		int VDIpageSize = 0,
			VDIoffsetData = 0,
			disk_size,
			cursor, //next byte to read
			page, //index in map
			offset, //(doesn't need to be a variable?)
			frame, //(doesn't need to be a variable?)
			location, //location wanted
			fd; //to open file
		int * map;
		char descriptor[400]; //512 bytes
		char buffer[500];
		fstream vdiFile;
	public:
		vdiR();
		void vOpen(string fileName);
		void vClose();
		int vRead(string fileName, char buffer[], int byteSize);
		void vSeek(int offset, int type);
	};
};
#endif
