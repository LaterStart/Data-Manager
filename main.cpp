#include <iostream> // debug
#include "dm_1.0_src/XMLparser.h"
using namespace std;

// TESTER FILE
int main() {
	XMLparser parser("db_config.xml");
	XMLdocument* cfg = parser.ParseFile();

	XMLnode* node01 = cfg->SelectNode("db_config_01");
	XMLnode* node02 = cfg->SelectNode("db_config_02");
	XMLnode* node03 = cfg->SelectNode("tables");
	XMLnode* node04 = cfg->SelectNode("table");
	XMLnode* node05 = cfg->SelectNode("table", "TestTable_01");
	XMLnode* node06 = cfg->SelectNode("table", "TestTable_02");
	XMLnode* node07 = cfg->SelectNode("table", "TestTable_03");
	XMLnode* node08 = cfg->SelectNode("table", "TestTable_04");

	return 0;
}