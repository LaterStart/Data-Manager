#pragma once
#include <list>
#include <vector>

const enum xps {
	inactive, filePathCorrect, incorrectFilePath, fileLoaded, fileInvalid, successParse
};

class XMLnode;
struct XMLdocument {
	XMLdocument() = default;
	~XMLdocument();

	std::vector<XMLnode*> trees;
	XMLnode* SelectNode(char* tagID, char* nodeHeader = nullptr, int treeID = -1);
	std::vector<XMLnode*> SelectNodes(char* tagID, char* nodeHeader = nullptr, int treeID = -1);
};

class XMLnode {
public:
	XMLnode() = default;
	XMLnode(char* name);
	~XMLnode();

public:
	XMLnode* parentNode{};
	char* tagID{};
	char* value{};
	std::list<XMLnode*> nodes{};

private:
	friend std::vector<XMLnode*> XMLdocument::SelectNodes(char* tagID, char* nodeHeader, int treeID);
	void SelectNodes(char* tagID, char* nodeHeader, std::vector<XMLnode*> &vNodes);

public:
	XMLnode* SelectNode(char* tagID, char* nodeHeader = nullptr);
	std::vector<XMLnode*> SelectNodes(char* tagID, char* nodeHeader = nullptr);
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
