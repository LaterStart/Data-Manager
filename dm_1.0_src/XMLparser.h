#pragma once
#include <list>

const enum xps {
	inactive, filePathCorrect, incorrectFilePath, fileLoaded, fileInvalid, successParse
};

struct XMLnode {
	XMLnode();
	XMLnode(char* name);
	~XMLnode();

	XMLnode* parentNode{};
	char* name{};
	char* value{};
	std::list<XMLnode*>* nodes{};
};

struct XMLdocument {
	XMLdocument();

	std::list<XMLnode*>* nodes;
};

class XMLparser {
public:
	XMLparser(const char* cfgPath);
	~XMLparser();
	
private:
	std::streamoff Find(char* keyword, std::streamoff startPos);
	void CopyCharBuff(char* &dest, char* source, std::streamoff len);
	void ReadXML(char* buffer, std::streamoff& pos, XMLnode& node);
	bool ClosingTag(char* buffer) const;	
	void LoadFile();

private:
	const char* filePath{};
	char* buffer{};
	std::streamoff fileSize{};
	xps status = xps::inactive;

public:
	XMLdocument * ParseFile();
};
