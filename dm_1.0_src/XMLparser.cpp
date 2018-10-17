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
			doc->trees.push_back(new XMLnode);
			ReadXML(buffer, pos, *doc->trees.back());
		}	

		status = xps::successParse;
		return doc;
	}
	else return nullptr;
}

void XMLparser::ReadXML(char* buffer, streamoff& pos, XMLnode& node) {
	streamoff offset, len, memPos1, memPos2;
	if (node.tagID == nullptr) {
		pos = Find("<", pos) + 1;
		if (pos == 0)
			return;

		offset = len = pos;
		pos = Find(">", pos);
		len = pos - len;

		CopyCharBuff(node.tagID, buffer + offset, len);
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
		delete[]tag;
		if (node.nodes.size() > 0) {
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
		node.nodes.push_back(childNode);

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

XMLnode::XMLnode(char* name) {
	this->tagID = name;	
}

XMLnode::~XMLnode() {
	delete[]tagID;
	delete[]value;
	for (auto& node : nodes) {
		delete node;
	}
}

XMLparser::~XMLparser() {
	delete[]buffer;
}

XMLdocument::~XMLdocument() {
	for (auto& tree : trees) {
		delete tree;
	}
}

XMLnode* XMLdocument::SelectNode(char* tagID, char* nodeHeader, int treeID) {
	size_t range = (treeID < 0) ? trees.size() : treeID + 1;
	treeID = (treeID < 0) ? 0 : treeID;
	for (size_t i = treeID; i < range; i++) {
		XMLnode* node = trees[i]->SelectNode(tagID, nodeHeader);
		if (node != nullptr)
			return node;
	}
	return nullptr;
}

XMLnode* XMLnode::SelectNode(char* tagID, char* nodeHeader) {
	if (strcmp(this->tagID, tagID) == 0) {
		if (nodeHeader == nullptr)
			return this;
		else if (nodes.size() > 0 && strcmp(nodeHeader, nodes.front()->value) == 0)
			return this;
	}
	for (auto& node : nodes) {
		XMLnode* result = node->SelectNode(tagID, nodeHeader);
		if (result != nullptr)
			return result;
	}
	return nullptr;
}

vector<XMLnode*> XMLdocument::SelectNodes(char* tagID, char* nodeHeader, int treeID) {
	vector<XMLnode*> nodes;
	size_t range = (treeID < 0) ? trees.size() : treeID + 1;
	treeID = (treeID < 0) ? 0 : treeID;
	for (size_t i = treeID; i < range; i++) 
		trees[i]->SelectNodes(tagID, nodeHeader, nodes);

	return nodes;
}

void XMLnode::SelectNodes(char* tagID, char* nodeHeader, vector<XMLnode*> &vNodes) {
	if (strcmp(this->tagID, tagID) == 0) {
		if (nodeHeader == nullptr)
			vNodes.push_back(this);
		else if (nodes.size() > 0 && strcmp(nodeHeader, nodes.front()->value) == 0)
			vNodes.push_back(this);
	}
	for (auto& node : nodes) 
		node->SelectNodes(tagID, nodeHeader, vNodes);
}

std::vector<XMLnode*> XMLnode::SelectNodes(char* tagID, char* nodeHeader) {
	vector<XMLnode*> vNodes;
	for (auto& node : nodes)
		node->SelectNodes(tagID, nodeHeader, vNodes);	
	return vNodes;
}

XMLnode* XMLnode::SelectNode(char* tagID, bool singleLevel) {
	for (auto& node : nodes) {
		if (strcmp(node->tagID, tagID) == 0) 			
				return node;
	}
	return nullptr;
}