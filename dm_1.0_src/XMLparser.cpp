#include <iostream> //debug
#include <fstream>
#include "XMLparser.h"
using namespace std;

XMLparser::XMLparser(const char* cfgPath) {
	ifstream file(cfgPath, ifstream::ate | ifstream::binary);
	fileSize = file.tellg();
	if (fileSize > 0)
		status = xps::filePathCorrect;
	else status = xps::incorrectFilePath;
	filePath = cfgPath;
}

void XMLparser::LoadFile() {
	if (status == filePathCorrect) {
		buffer = new char[fileSize];
		ifstream file(filePath, ifstream::binary);
		file.read(buffer, fileSize);
		status = xps::fileLoaded;
		ParseFile();
	}
	else return;
}

XMLdocument XMLparser::ParseFile() {
	if (status == fileLoaded) {
		XMLdocument doc;
		streamoff pos = 0, len, offset;
		XMLnode node(0);
		ReadNode(buffer, pos, node);

		int test = 0;







	}
	else return XMLdocument();
}

void XMLparser::ReadNode(char* buffer, streamoff& pos, XMLnode& node) {
	streamoff offset, len, memPos1, memPos2;
	if (node.name == nullptr) {
		pos = Find("<", pos) + 1;
		if (pos == 0)
			return;

		offset = len = pos;
		pos = Find(">", pos);
		len = pos - len;

		CopyCharBuff(node.name, buffer + offset, len);
	}
	memPos1 = ++pos;

	pos = Find("<", pos) + 1;
	memPos2 = pos - 1;
	offset = len = pos;
	pos = Find(">", pos);
	len = pos - len;	

	char* tag;
	CopyCharBuff(tag, buffer + offset, len);

	if (ClosingTag(tag)) {		
		if (ParentNodeCloseTag(tag, node.parentNode->name)) {
			int test = 0;
			return;
		}
			
		CopyCharBuff(node.value, buffer + memPos1, memPos2 - memPos1);

		ReadNode(buffer, pos, *node.parentNode);
	}
	else {
		char* rnt;		
		CopyCharBuff(rnt, buffer + memPos1, memPos2 - memPos1);
		size_t lvl = strlen(rnt) - 2;
		delete[]rnt;

		XMLnode* childNode = new XMLnode(node.Level() + 1, tag);
		childNode->parentNode = &node;
		node.nodes->push_back(childNode);

		ReadNode(buffer, pos, *childNode);
	}	
}

streamoff XMLparser::Find(char* keyword, streamoff startPos = 0) {
	streamoff pos = startPos;
	char ch = keyword[0];
	size_t size = strlen(keyword) +1;
	if (pos + size > fileSize)
		return -1;

	char* checker = new char[size];
	checker[size - 1] = '\0';
	do {
		if (buffer[pos] == ch) {
			for (size_t i = 0; i < size - 1; i++)
				checker[i] = buffer[pos + i];
			if (strcmp(checker, keyword) == 0) {
				delete[]checker;
				return pos;
			}
		}
	} while (pos++ < (fileSize - size));
	delete[]checker;
	return -1;
}

void XMLparser::CopyCharBuff(char* &dest, char* source, streamoff len) {
	dest = new char[len + 1];
	dest[len] = '\0';
	memcpy(dest, source, len);
}

bool XMLparser::ClosingTag(char* tag) const {
	if (tag[0] == '/')
		return true;
	else return false;
}

bool XMLparser::ParentNodeCloseTag(char* tag, char* parentNodeName) const {
	if (strcmp(tag + 1, parentNodeName) == 0)
		return true;
	else return false;
}

XMLnode::XMLnode(int level) {
	this->level = level;
	nodes = new list<XMLnode*>;
}

XMLnode::XMLnode(int level, char* name) {
	this->level = level;
	this->name = name;
	nodes = new list<XMLnode*>;
}

XMLnode::~XMLnode() {
	delete[]name;
	delete[]value;
	delete nodes;
}