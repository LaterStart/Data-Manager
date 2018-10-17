#include <iostream> //debug
#include <fstream>
#include "XMLparser.h"
using namespace std;

XMLparser::XMLparser(const char* cfgPath) {
	ifstream file(cfgPath, ifstream::ate | ifstream::binary);
	fileSize = file.tellg();
	if (fileSize > 0) {
		status = xps::filePathCorrect;
		filePath = cfgPath;
		LoadFile();
	}		
	else status = xps::incorrectFilePath;	
}

void XMLparser::LoadFile() {
	if (status == filePathCorrect) {
		buffer = new char[fileSize];
		ifstream file(filePath, ifstream::binary);
		file.read(buffer, fileSize);
		status = xps::fileLoaded;
	}
	else return;
}

XMLdocument* XMLparser::ParseFile() {	
	if (status == fileLoaded) {
		XMLdocument* doc = new XMLdocument;
		streamoff pos = 0, len, offset;

		while ((fileSize - pos) > 1) {
			doc->nodes->push_back(new XMLnode);
			ReadXML(buffer, pos, *doc->nodes->back());
		}	

		status = xps::successParse;
		return doc;
	}
	else return nullptr;
}

void XMLparser::ReadXML(char* buffer, streamoff& pos, XMLnode& node) {
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
		if (node.nodes->size() > 0) {
			if (node.parentNode == nullptr)
				return;
			else
				ReadXML(buffer, pos, *node.parentNode);
		}
		else {
			CopyCharBuff(node.value, buffer + memPos1, memPos2 - memPos1);
			ReadXML(buffer, pos, *node.parentNode);
		}
	}
	else {
		XMLnode* childNode = new XMLnode(tag);
		childNode->parentNode = &node;
		node.nodes->push_back(childNode);

		ReadXML(buffer, pos, *childNode);
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
	} while (++pos <= fileSize);
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

XMLnode::XMLnode() {
	nodes = new list<XMLnode*>;
}

XMLnode::XMLnode(char* name) {
	this->name = name;
	nodes = new list<XMLnode*>;
}

XMLnode::~XMLnode() {
	delete[]name;
	delete[]value;
	delete nodes;
}

XMLdocument::XMLdocument() {
	nodes = new list<XMLnode*>;
}

XMLparser::~XMLparser() {
	delete[]buffer;
}