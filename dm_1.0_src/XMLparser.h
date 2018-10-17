#pragma once
#include <list>
#include <vector>

const enum xps {
	inactive, filePathCorrect, incorrectFilePath, fileLoaded, fileInvalid, successParse
};

struct XMLnode {
	XMLnode() = default;
	XMLnode(char* name);
	~XMLnode();

	XMLnode* parentNode{};
	char* tagID{};
	char* value{};
	std::list<XMLnode*> nodes{};

	XMLnode* SelectNode(char* tagID, char* nodeHeader = nullptr);
};

struct XMLdocument {
	XMLdocument() = default;
	~XMLdocument();

	std::vector<XMLnode*> trees;
	XMLnode* SelectNode(char* tagID, char* nodeHeader = nullptr, int treeID = -1);
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
