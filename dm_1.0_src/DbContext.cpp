#include "DbContext.h"
#include "XMLparser.h"
using namespace std;

DbContext::DbContext(XMLdocument* cfg) {
	vector<XMLnode*> databases = cfg->trees;
	this->databases.reserve(databases.size());

	// Create separated databases
	for(auto& db : databases){
		XMLnode* tables = db->SelectNode("tables");
		
		// Fill database with tables
		this->databases.push_back(Database(db));
		Database& tdb = this->databases.back();
		for(auto& table : tables->nodes){
			XMLnode* columns = table->SelectNode("columns");

			// Fill tables with columns
			tdb.tables.push_back(Table(table));
			Table& ttb = tdb.tables.back();
			for(auto& column : columns->nodes){
				ttb.columns.push_back(Column(column));

			}
		}
	}
}

Database::Database(XMLnode* node) {
	XMLnode* nameNode = node->SelectNode("name", true);
	if (nameNode != nullptr) {
		size_t s = strlen(nameNode->value);
		name = new char[s + 1];
		strcpy(name, nameNode->value);
	}

	tables.reserve(node->nodes.size());
}

Table::Table(XMLnode* node) {
	XMLnode* nameNode = node->SelectNode("name", true);
	if (nameNode != nullptr) {
		size_t s = strlen(nameNode->value);
		name = new char[s + 1];
		strcpy(name, nameNode->value);
	}

	columns.reserve(node->nodes.size());
}

Column::Column(XMLnode* node) {	
	XMLnode* propertyNode = node->SelectNode("name");
	if (propertyNode != nullptr) {
		size_t s = strlen(propertyNode->value);
		name = new char[s + 1];
		strcpy(name, propertyNode->value);
	}

	propertyNode = node->SelectNode("type");
	if (propertyNode != nullptr)
		type = ParseType(propertyNode->value);

	propertyNode = node->SelectNode("nullable");
	if (propertyNode != nullptr)
		nullable = ParseBoolean(propertyNode->value);

	propertyNode = node->SelectNode("identity");
	if (propertyNode != nullptr)
		identity = ParseBoolean(propertyNode->value);

	propertyNode = node->SelectNode("autoIncrement");
	if (propertyNode != nullptr)
		autoIncrement = ParseBoolean(propertyNode->value);

	propertyNode = node->SelectNode("seedStart");
	if (propertyNode != nullptr)
		seedStart = atoi(propertyNode->value);

	propertyNode = node->SelectNode("seedIncrease");
	if (propertyNode != nullptr)
		seedIncrease = atoi(propertyNode->value);
	
}

DataType Column::ParseType(char* value) {
	if (strcmp(value, "bit") == 0)
		return DataType::_bit;
	
	if (strcmp(value, "byte") == 0)
		return DataType::_byte;

	if (strcmp(value, "int") == 0)
		return DataType::_int;

	if (strcmp(value, "uint") == 0)
		return DataType::_uint;
	
	if (strcmp(value, "double") == 0)
		return DataType::_double;

	if (strcmp(value, "string") == 0)
		return DataType::_string;

	if (strcmp(value, "long") == 0)
		return DataType::_long;

	if (strcmp(value, "ulong") == 0)
		return DataType::_ulong;

	if (strcmp(value, "date") == 0)
		return DataType::_date;
	
	if (strcmp(value, "binary") == 0)
		return DataType::_binary;

	return DataType::_unset;
}

bool Column::ParseBoolean(char* value) {
	if (strcmp(value, "true") == 0)
		return true;
	else return false;
}
