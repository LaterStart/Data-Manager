#include <iostream> // debug
#include "dm_1.0_src/XMLparser.h"
using namespace std;

// TEST FILE
int main() {
	XMLparser parser("db_config.xml");
	XMLdocument* cfg = parser.ParseFile();



	return 0;
}