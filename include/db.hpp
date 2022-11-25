#ifndef DB_H
#define DB_H

#include <memory>
#include "global.hpp"

class QSqlDatabase;
class QString;

// Класс базы данных, инкапсулирует в себя всю логику по записи и чтению
class Db  {
	std::unique_ptr<QSqlDatabase> mDb;
	Db();
public:
	static Db& instance();
	Db(Db&) = delete;
	Db(Db&&) = delete;
	// Работа с инвентарём
	void storeData(uint32_t row, uint32_t column, uint32_t count, ItemId iid);
	std::tuple<uint32_t, ItemId> getData(uint32_t row, uint32_t column);
	// Работа с предметом
	void registerItem(Global::ItemMeta meta);
	QString getItemName(ItemId iid);
	QString getItemImage(ItemId iid);
};

#endif // DB_H
