#include "global.hpp"

#include <QColor>
#include <QFont>

static const Global::ItemMeta lItems[] = {
	[iiNone] = Global::ItemMeta{ .id = iiNone, .name = "", .image = "" },
	[iiApple] = Global::ItemMeta{ .id = iiApple, .name = "Apple", .image = ":/image/apple.png" },
};

QColor Global::fontColor() {
	return QColor(192, 192, 192, 255);
}

QFont Global::font() {
	return QFont("Arial", 14);
}

Global::ItemMeta Global::itemMeta(ItemId id) {
	if (id >= sizeof lItems/sizeof(Global::ItemMeta)) {
		return lItems[iiNone];
	} else {
		return lItems[id];
	}
}
