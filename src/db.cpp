#include "db.hpp"
#include <QVariant>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <iostream>

static bool createItems(QSqlQuery& query) {
	return query.exec(QString("CREATE TABLE IF NOT EXISTS items (itemId INTEGER, name TEXT, image TEXT);"))
		&& query.exec(QString("CREATE UNIQUE INDEX IF NOT EXISTS itemsIdx ON items (itemId);"));
}

static bool createInventory(QSqlQuery& query) {
	return query.exec(QString("CREATE TABLE IF NOT EXISTS inventory (row INTEGER, column INTEGER, count INTEGER,"
							  "itemId INTEGER, FOREIGN KEY(itemId) REFERENCES items(itemId));"))
		&& query.exec(QString("CREATE UNIQUE INDEX IF NOT EXISTS inventoryIdx ON inventory (row, column);"));
}

Db::Db() {
	mDb = std::make_unique<QSqlDatabase>(QSqlDatabase::addDatabase("QSQLITE"));
	mDb->setDatabaseName("database.sqlite");
	if (!mDb->open()) {
		std::cerr << "Can't open database.sqlite" << std::endl;
		return;
	}

	QSqlQuery query(*mDb);
	if (!createItems(query) || !createInventory(query)) {
		std::cerr << "Error while creating table" << std::endl;
		return;
	}
}

Db& Db::instance() {
	static Db d;
	return d;
}

void Db::storeData(uint32_t row, uint32_t column, uint32_t count, ItemId iid) {
	QSqlQuery query(*mDb);
	bool res = query.prepare(QString("INSERT OR REPLACE INTO inventory VALUES($1, $2, $3, $4);"));
	query.bindValue(0, row);
	query.bindValue(1, column);
	query.bindValue(2, count);
	query.bindValue(3, iid);
	if (!res || !query.exec()) {
		std::cerr << "Error while writing to table" << std::endl;
	}
}

std::tuple<uint32_t, ItemId> Db::getData(uint32_t row, uint32_t column) {
	QSqlQuery query(*mDb);
	bool res = query.prepare(QString("SELECT count, itemId FROM inventory WHERE row = $1 AND column = $2;"));
	query.bindValue(0, row);
	query.bindValue(1, column);
	if (!res || !query.exec() || !query.next()) {
		std::cerr << "Error while reading from table" << std::endl;
		return { 0, iiNone };
	}
	return { query.value(0).toUInt(), (ItemId)query.value(1).toUInt() };
}

void Db::registerItem(Global::ItemMeta meta) {
	QSqlQuery query(*mDb);
	bool res = query.prepare(QString("INSERT OR REPLACE INTO items VALUES($1, $2, $3);"));
	query.bindValue(0, meta.id);
	query.bindValue(1, meta.name);
	query.bindValue(2, meta.image);
	if (!res || !query.exec()) {
		std::cerr << "Error while writing to table" << std::endl;
	}
}

QString Db::getItemName(ItemId iid) {
	QSqlQuery query(*mDb);
	bool res = query.prepare(QString("SELECT name FROM items WHERE itemId = $1;"));
	query.bindValue(0, iid);
	if (!res || !query.exec() || !query.next()) {
		std::cerr << "Error while reading from 'items' table" << std::endl;

	}
	return query.value(0).toString();
}

QString Db::getItemImage(ItemId iid) {
	QSqlQuery query(*mDb);
	bool res = query.prepare(QString("SELECT image FROM items WHERE itemId = $1;"));
	query.bindValue(0, iid);
	if (!res || !query.exec() || !query.next()) {
		std::cerr << "Error while reading from 'items' table" << std::endl;

	}
	return query.value(0).toString();
}
