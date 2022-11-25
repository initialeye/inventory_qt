#include "inventory.hpp"
#include "global.hpp"
#include "db.hpp"
#include "mainwindow.hpp"

#include <QTableWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPixmap>
#include <QHeaderView>
#include <QResizeEvent>
#include <QPainter>
#include <QDrag>
#include <QMimeData>
#include <QSound>
#include <QPushButton>

struct DragData {
	uint32_t count;
	ItemId item;
};

// Класс элемента инвентаря
class InventoryElement : public QLabel {
	uint32_t mRow = 0;
	uint32_t mColumn = 0;
	ItemId mIid = iiNone;
	uint32_t mCount = 0; // Храним число в элементе, ибо удобно
	QString mImageSrc; // Избыточно

	void flushActual() const {
		Db::instance().storeData(mRow, mColumn, mCount, mIid);
	}

public:
	InventoryElement(uint posX, uint posY, QWidget* parent = nullptr)
		: QLabel(parent)
	{
		mRow = posX;
		mColumn = posY;
		setAcceptDrops(true);
		resetFlush();
	}
	void updatePixmap() {
		if (mCount) {
			QPixmap lpix = QPixmap(mImageSrc).scaled(size());
			QPainter painter(&lpix);
			painter.setPen(Global::fontColor());
			painter.setFont(Global::font());
			painter.drawText(rect(), Qt::AlignBottom | Qt::AlignRight, QString::number(mCount));
			painter.end();
			setPixmap(lpix);
		} else {
			setPixmap(QPixmap());
		}
	}
	void reset() {
		mCount = 0;
		mIid = iiNone;
		mImageSrc = QString();
	}
	void resetFlush() {
		reset();
		flushActual();
	}
protected:
	void dragEnterEvent(QDragEnterEvent* event) {
		if (event->mimeData()->hasFormat("data"))
			event->acceptProposedAction();
	}
	void dropEvent(QDropEvent *event) {
		QByteArray countData = event->mimeData()->data("data");
		if (countData.size() == sizeof(DragData)) {
			auto dragD = *(DragData*)countData.data();
			if (mIid == iiNone || mIid == dragD.item) {
				mCount += dragD.count;
				mIid = dragD.item;
				mImageSrc = Global::itemMeta(mIid).image;
				flushActual();
				event->acceptProposedAction();
				updatePixmap();
				return;
			}
		}
		event->ignore();
	}
	void mousePressEvent(QMouseEvent *event) {
		if (event->button() == Qt::RightButton && mCount) {
			--mCount;
			if (mCount == 0) {
				reset();
			}
			flushActual();
			updatePixmap();
			QSound::play(":/sound/apple.wav");
		} else if (event->button() == Qt::LeftButton && mCount) {
			QMimeData *mimeData = new QMimeData;
			DragData dragD = { mCount, mIid };
			mimeData->setData("data", QByteArray((const char*)&dragD, sizeof(DragData)));

			QDrag *drag = new QDrag(this);
			drag->setMimeData(mimeData);
			drag->setPixmap(QPixmap(mImageSrc).scaled(size()));
			if (drag->exec() == Qt::MoveAction) {
				resetFlush();
				updatePixmap();
			}
		}
	}
};

Inventory::Inventory(uint32_t w, uint32_t h, MainWindow* parent)
	: QWidget{parent}
{
	auto mainGrid = new QHBoxLayout;
	setLayout(mainGrid);

	mInventory = new QTableWidget(this);
	mInventory->setColumnCount(w);
	mInventory->setRowCount(h);
	mInventory->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	mInventory->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	mInventory->horizontalHeader()->hide();
	mInventory->verticalHeader()->hide();
	mainGrid->addWidget(mInventory);

	auto subGrid = new QVBoxLayout;
	mainGrid->addLayout(subGrid);
	mApple = new QLabel;
	subGrid->addStretch(5);
	subGrid->addWidget(mApple);
	subGrid->addStretch(1);
	mMenuButton = new QPushButton("Главное меню");
	subGrid->addWidget(mMenuButton);
	mApple->setAlignment(Qt::AlignCenter);
	subGrid->addStretch(3);

	Db::instance().registerItem(Global::itemMeta(iiApple));
	connect(mMenuButton, SIGNAL(pressed()), parent, SLOT(mainMenuPressed()));

	for (uint32_t i = 0; i < h; ++i) {
		for (uint32_t n = 0; n < w; ++n) {
			mInventory->setCellWidget(n, i, new InventoryElement(n, i));
		}
	}
}

// сброс инвентаря
void Inventory::reset() {
	for (uint32_t i = 0, ie = mInventory->rowCount(); i < ie; ++i) {
		for (uint32_t n = 0, ne = mInventory->columnCount(); n < ne; ++n) {
			auto elem = (InventoryElement*)mInventory->cellWidget(i, n);
			elem->resetFlush();
		}
	}
}

void Inventory::disable() {
	mMenuButton->setVisible(false);
	mApple->setVisible(false);
	setDisabled(true);
}

void Inventory::enable() {
	mMenuButton->setVisible(true);
	mApple->setVisible(true);
	setDisabled(false);
}

// подстройка элемента по размеру окна
void Inventory::resizeEvent(QResizeEvent* event) {
	(void)event;
	if (mInventory->rowCount() == 0 || mInventory->columnCount() == 0) return;
	auto item = Global::itemMeta(iiApple);
	mApple->setPixmap(QPixmap(item.image).scaled(((InventoryElement*)mInventory->cellWidget(0, 0))->size()));
	for (uint32_t i = 0, ie = mInventory->rowCount(); i < ie; ++i) {
		for (uint32_t n = 0, ne = mInventory->columnCount(); n < ne; ++n) {
			auto elem = (InventoryElement*)mInventory->cellWidget(i, n);
			elem->updatePixmap();
		}
	}
}

// отслеживаем, когда начнётся перетаскивание
void Inventory::mousePressEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton && mApple->geometry().contains(event->pos())) {
		QMimeData *mimeData = new QMimeData;
		DragData dragD = { 1, iiApple };
		// и переправляем данные прямо в объект
		mimeData->setData("data", QByteArray((const char*)&dragD, sizeof(DragData)));

		auto size = mApple->size();
		QDrag *drag = new QDrag(this);
		drag->setHotSpot(QPoint(size.width()/2, size.height()/2));
		drag->setMimeData(mimeData);
		drag->setPixmap(mApple->pixmap(Qt::ReturnByValue));
		drag->exec();
	}
}
