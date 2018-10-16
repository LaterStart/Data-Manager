#pragma once
#include <list>

const enum xps {
	inactive, filePathCorrect, incorrectFilePath, fileLoaded, fileInvalid
};

class XMLnode {
private: 
	unsigned int level;

public:
	XMLnode(int level);
	XMLnode(int level, char* name);
	~XMLnode();

public:
	XMLnode* parentNode{};
	char* name{};
	char* value{};
	std::list<XMLnode*>* nodes;
	unsigned int Level() const;
};

struct XMLdocument {
	const char* filePath{};
	std::list<XMLnode> nodes;
};

class XMLparser {
private:
	const char* filePath{};
	char* buffer{};
	std::streamoff fileSize{};
	xps status = xps::inactive;	

	
private:
	std::streamoff Find(char* keyword, std::streamoff startPos);
	void CopyCharBuff(char* &dest, char* source, std::streamoff len);
	void ReadNode(char* buffer, std::streamoff& pos, XMLnode& node);
	bool ClosingTag(char* buffer) const;
	bool XMLparser::ParentNodeCloseTag(char* tag, char* parentNodeName) const;
	XMLdocument ParseFile();

public:
	XMLparser(const char* cfgPath);

public:
	void LoadFile();
};

inline unsigned int XMLnode::Level() const {
	return level;
}
