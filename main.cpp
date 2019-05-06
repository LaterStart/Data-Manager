#include <iostream> // debug
#include "XMLparser.h"
using namespace std;

// TESTER FILE
int main() {
	// Parse XML document
	XMLparser parser("example.xml");
	XMLdocument* cfg = parser.ParseFile();

	// Select first node to meet tag id condition
	XMLnode* node01 = cfg->SelectNode("db_config_01");
	XMLnode* node02 = cfg->SelectNode("db_config_02");
	XMLnode* node03 = cfg->SelectNode("tables");
	XMLnode* node04 = cfg->SelectNode("columns");
	XMLnode* node05 = cfg->SelectNode("table");

	// Select first node to meet tag id condition and first child node value condition
	XMLnode* node06 = cfg->SelectNode("table", "TestTable_01");
	XMLnode* node07 = cfg->SelectNode("table", "TestTable_02");
	XMLnode* node08 = cfg->SelectNode("table", "TestTable_03");
	XMLnode* node09 = cfg->SelectNode("table", "TestTable_04");

	// Select all nodes which meet selected conditions
	vector<XMLnode*> nodes01 = cfg->SelectNodes("column");
	vector<XMLnode*> nodes02 = cfg->SelectNodes("table");
	vector<XMLnode*> nodes03 = node03->SelectNodes("table");
	vector<XMLnode*> nodes04 = node04->SelectNodes("column");

	return 0;
}
