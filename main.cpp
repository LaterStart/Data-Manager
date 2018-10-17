#include <iostream> // debug
#include "dm_1.0_src/XMLparser.h"
using namespace std;

int main() {
	XMLparser* parser = new XMLparser("db_config.xml");
	XMLdocument* cfg = parser->ParseFile();

	return 0;
}