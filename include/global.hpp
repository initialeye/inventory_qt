#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <stdint.h>

class QColor;
class QFont;

// Поскольку в задаче не предусматривается подгрузка элементов, они закреплены в коде
enum ItemId : uint32_t {
	iiNone,
	iiApple,
};

// Объявление пространства имён, элементы которого глобальны и могут быть доступны в любой точке программы.
struct Global {
	// Структура, описывающая метаданные объекта
	struct ItemMeta {
		ItemId id;
		const char* name;
		const char* image;
	};

	static QColor fontColor();
	static QFont font();
	static ItemMeta itemMeta(ItemId id);
};

#endif // GLOBAL_HPP
