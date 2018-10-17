#pragma once
#include <vector>

const enum class DataType {
	_unset, _bit, _byte, _int, _uint, _long, _ulong, _double, _string, _date, _binary
};

class XMLdocument;
class Database;
class DbContext {
public:
	DbContext(XMLdocument* cfg);

private:
	std::vector<Database> databases;

public:
	std::vector<Database> Databases() const;
};

class XMLnode;
class Table;
class Database {
	friend class DbContext;

private:
	Database(XMLnode* node);

public:
	std::vector<Table> Tables() const;

private:
	char* name{};
	std::vector<Table> tables;

};

class Column;
class Table {
	friend class DbContext;

private:
	Table(XMLnode* node);

public:
	std::vector<Column> Columns() const;

private:
	char* name{};
	std::vector<Column> columns;
};


class Column {
	friend class DbContext;

private:
	Column(XMLnode* node);

private:
	DataType ParseType(char* value);
	bool ParseBoolean(char* value);	

private:
	char* name{};
	DataType type{DataType::_unset};
	bool nullable{ true };
	bool identity{};
	bool autoIncrement{};
	int seedStart{};
	int seedIncrease{};
};

inline std::vector<Database> DbContext::Databases() const {
	return databases;
}

inline std::vector<Table> Database::Tables() const {
	return tables;
}

inline std::vector<Column> Table::Columns() const {
	return columns;
}