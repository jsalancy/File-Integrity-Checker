#include "vdi.h"
#include "directory.h"
#include <iostream>
#include <string>

using namespace std;
using namespace vdi;
using namespace dir;

//display stats
int fileSize,
availableForFiles,
usedSpace,
possibleFiles,
possibleDirect,
existFiles,
existDirect,
blockGroups,
blockSize,
wrongCount;

//additional checks
int magicNumber = 21930,
actualNumber;

char buffer[500];
//char buffer[500];
int change;//remove once numbers are determined?

char filename[20];

string systemState,
fileName;

vdiR file;
directory dFile;//use for additional information

int main(){

	cout << "Input file name: ";
	cin >> fileName;
	file.vOpen(fileName);
	//-------------------------------------------------------------------------------------------------------------------------------------------
	///display statisics
	cout << "STATISTICS: \n";
	//-------------------------------------------------------------------------------------------------------------------------------------------

	///file system size:bytes
	file.vSeek(1,0);
	fileSize = file.vRead(filename, buffer, change); //NOTE: define change
	cout << "File Size: " << fileSize << "\n\n";

	///available space for files: used/unused
	file.vSeek(1, 0);
	availableForFiles = file.vRead(filename, buffer, change); //NOTE: define change
	cout << "Space Avaiable For Files: " << availableForFiles << "\n\n";

	///used space
	file.vSeek(1, 0);
	usedSpace = file.vRead(filename, buffer, change); //NOTE: define change
	cout << "Used Space: " << usedSpace << "\n\n";

	///number of inodes:possible files/directories
	file.vSeek(1, 0);
	possibleFiles = file.vRead(filename, buffer, change); //NOTE: define change
	cout << "Number of Possible Files: " << possibleFiles << "\n\n";

	///exisiting files
	file.vSeek(1, 0);
	existFiles = file.vRead(filename, buffer, change); //NOTE: define change
	cout << "Number of Existing Files: " << existFiles << "\n\n";

	///existing directories
	file.vSeek(1, 0);
	existDirect = file.vRead(filename, buffer, change); //NOTE: define change
	cout << "Number of Existing Directories: " << existDirect << "\n\n";

	///block size
	file.vSeek(1, 0);
	fileSize = file.vRead(filename, buffer, change); //NOTE: define change
	cout << "Block Size: " << fileSize << "\n\n";

	///state of filesystem
	file.vSeek(1, 0);
	blockSize = file.vRead(filename, buffer, change); //NOTE: define change
	cout << "Block Size: " << blockSize << "\n\n";

	cout << "------------------------------\n";

	//-------------------------------------------------------------------------------------------------------------------------------------------
	///additional checks
	cout << "ADDITIONAL CHECKS: \n";
	//-------------------------------------------------------------------------------------------------------------------------------------------
	dFile.openFile(fileName, fileSize);

	///superblock has appropriate "magic number"
	//dFile.dirOpen(inodeNumber)
	actualNumber = dFile.dirRead(buffer, change/*location of magic number: replace with location*/);/*replace with driTraverse -> dirRead?*/
	//if correct
	if (actualNumber == magicNumber) {
		cout << "The magic number is correct at " << actualNumber << "\n\n";
	}
	//if incorrect
	else {
		cout << "The magic number is incorrect and is currently " << actualNumber << "\n\n";
	}

	///consistency in superblock copies and report differences
	//dFile.dirOpen(inodeNumber)
	wrongCount = 0;
	file.vSeek(1, 0/*original superblock*/);
	int original = dFile.dirRead(buffer, change/*location of magic number: replace with location*/);/*replace with driTraverse -> dirRead?*/

	file.vSeek(1, 0/*number of superblock copies*/);
	for (int i = 0; i < blockGroups; i++) {
		file.vSeek(1, 0/*superblock copy[i]*/);
		int copy = dFile.dirRead(buffer, change);/*replace with driTraverse? -> dirRead?*/

		//if incorrect
		if (copy != original) {
			cout << "Superblock copy " << i << " has value " << copy << "\n\n";
			wrongCount++;
		}
		//repeat for recordLength, nameLength, fileType
	}

	//if correct
	if (wrongCount == 0) {
		cout << "The superblock copies are consistent \n\n";
	}

	///consistency in block group table copies and report differences
	//dFile.dirOpen(inodeNumber)
	wrongCount = 0;
	//if correct
	file.vSeek(1, 0/*number of block group table copies*/);
	for (int i = 0; i < dFile.dirRead(buffer,  4/*number of block group table copies: replace "4" if should be different amount read*/); i++) {
		//file.vSeek(1, 0/*block group table copy[i]*/);
		int copy = dFile.dirRead(buffer, change);
		if (fileSize == 1024) {
			dFile.fetchBlock(i, 1);
		}
		else if (fileSize == 2048) {
			dFile.fetchBlock(i, 2);
		}
		else {
			dFile.fetchBlock(i, 4);
		}

		if (dFile.dirRead(buffer, change) == 1) {
			dFile.dirTraverse();
		}

		//if incorrect
		if (true /*copy != original: what would be checked here through fetchBlock to make it work?*/) {
			cout << "Block group table copy " << i << " has value an incorrect value \n\n";
			wrongCount++;
		}
	}

	//if correct
	if (wrongCount == 0) {
		cout << "All block group table copies are consistent \n\n";
	}

	///each inode marked as used is reachable by root directory
	//dFile.dirOpen(inodeNumber)
	wrongCount = 0;
	file.vSeek(1, 0/*number of inodes*/);
	int *inUseInode = new int[dFile.dirRead(buffer, 4)]; //number of inodes

	for (int i = 0; i < dFile.dirRead(buffer, 4/*number of inodes: replace "4" if should be different amount read*/); i++) {
		//if incorrect
		if (dFile.dirRead(buffer, change) == 1) {
			dFile.dirTraverse();
		}

		if (dFile.getInode() != 0) {
			inUseInode[i] = 1;
		}
	}
	for (int i = 0; i < sizeof(inUseInode); i++) {
		if (inUseInode[i] != 1) {
			cout << "Inode " << i << " cannot be reached from root directory even though they are used \n\n";
			wrongCount++;
		}
	}

	//if correct
	if (wrongCount == 0) {
		cout << "All used inodes are reachable by the root directory\n\n";
	}

	///no unused inode reachable by root directory
	//dFile.dirOpen(inodeNumber)
	wrongCount = 0;
	file.vSeek(1, 0/*number of inodes*/);
	for (int i = 0; i < dFile.dirRead(buffer, 4/*number of inodes: replace "4" if should be different amount read*/); i++) {

		if (dFile.dirRead(buffer, change) == 1) {
			dFile.dirTraverse();
		}

		//if incorrect
		if (dFile.getInode() == 0) {
			cout << "Inode " << i << " reaches the root directory but are not used \n\n";
			wrongCount++;
		}
	}

	//if correct
	if (wrongCount == 0) {
		cout << "All unused inodes are not reachable by the root directory \n\n";
	}

	///each directory references in-use inode
	//dFile.dirOpen(inodeNumber)
	wrongCount = 0;
	file.vSeek(1, 0/*number of directories*/);
	for (int i = 0; i < dFile.dirRead(buffer, 4/*number of directories: replace "4" if should be different amount read*/); i++) {
		//file.vSeek(1, 0/*directory[i]*/);
		//int hasInode =dFile.dirRead(buffer, 4);

		//if incorrect
		if (dFile.dirRead(buffer, change) == 1) {
			dFile.dirTraverse();
		}

		//if incorrect
		if (dFile.getInode() == 0) {
			cout << "Directory " << i << " doesn't reference a in-use inode \n\n";
			wrongCount++;
		}
	}

	//if correct
	if (wrongCount == 0) {
		cout << "Each directory entry refernces an in-use inode \n\n";
	}

	///used data blocks referenced by exactly 1 inode as data or as an indirect block
	//dFile.dirOpen(inodeNumber)
	wrongCount = 0;
	file.vSeek(1, 0/*number of data blocks*/);
	for (int i = 0; i < dFile.dirRead(buffer, 4/*number of data block: replace "4" if should be different amount read*/); i++) {
		//file.vSeek(1, 0/*data block[i]*/);
		//int referenced = dFile.dirRead(buffer, 4);

		if (fileSize == 1024) {
			dFile.fetchBlock(i, 1);
		}
		else if (fileSize == 2048) {
			dFile.fetchBlock(i, 2);
		}
		else {
			dFile.fetchBlock(i, 4);
		}

		if (dFile.dirRead(buffer, change) == 1) {
			dFile.dirTraverse();
		}
		//if incorrect
		if (true/*referenced != 0: something to tell if referenced by node: need to change*/) {
			cout << "Data block " << i << " is not refenced by an inode \n\n";
			wrongCount++;
		}
	}

	//if correct
	if (wrongCount == 0) {
		cout << "Each data block is refenced by exactly one inode \n\n";
	}

	///no unused data block refenced by any inode
	//dFile.dirOpen(inodeNumber)
	wrongCount = 0;
	file.vSeek(1, 0/*number of data blocks*/);
	for (int i = 0; i < dFile.dirRead(buffer, 4/*number of directories: replace "4" if should be different amount read*/); i++) {
		//file.vSeek(1, 0/*data block[i]*/);
		//int referenced = dFile.dirRead(buffer, 4);

		if (fileSize == 1024) {
			dFile.fetchBlock(i, 1);
		}
		else if (fileSize == 2048) {
			dFile.fetchBlock(i, 2);
		}
		else {
			dFile.fetchBlock(i, 4);
		}

		if (dFile.dirRead(buffer, change) == 1) {
			dFile.dirTraverse();
		}
		//if incorrect
		if (true/*referenced != 0:something to tell if referenced by node && is Unused(00 if no inode) */) {
			cout << "Data block " << i << " is unused and refencing an inode \n\n";
			wrongCount++;
		}
	}

	//if correct
	if (wrongCount == 0) {
		cout << "No unused data block is refenced by any node \n\n";
	}

	///number of existing files reported by superblock correct
	//dFile.dirOpen(inodeNumber)
	//if correct
	int count = 0;
	for (int i = 0; i < existFiles; i++) {
		file.vSeek(1, 0/*number of data blocks*/);
		if (dFile.dirRead(buffer, 4) == 0 /*replace 4 and 0 with somethings to determine if in superblock*/) {
			count++;
		}
	}
	if (existFiles == count) {
		cout << "The number of exisiting files reported in the superblock is correct \n\n";
	}
	//if incorrect
	else {
		cout << "The number of exisiting files reported in the superblock is off by ";
		if (count > existFiles) {
			cout << (count - existFiles);
		}
		else {
			cout << (existFiles - count) << "\n\n";
		}
	}

	///number of existing directories reported by super block correct
	//dFile.dirOpen(inodeNumber)
	count = 0;
	for (int i = 0; i < existDirect; i++) {
		if (dFile.dirRead(buffer, change) == 1) {
			count++;
		}
	}
	//if correct
	if (existDirect == count) {
		cout << "The number of exisiting directories reported in the superblock is correct \n\n";
	}
	//if incorrect
	else {
		cout << "The number of exisiting directories reported in the superblock is off by ";
		if (count > existDirect) {
			cout << (count - existDirect);
		}
		else {
			cout << (existDirect - count) << "\n\n";
		}
	}

	cout << "------------------------------\n";

	file.vClose();
	dFile.closeFile();

	system("pause");
	return 0;
}

//https://forums.virtualbox.org/viewtopic.php?t=8046 
//https://en.wikipedia.org/wiki/Master_boot_record 

/*
TO DO:
Replace temporary values
vRead Function
dirRead
dirTraverse
*/