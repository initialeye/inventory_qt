#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include <QWidget>

class QTableWidget;
class QPushButton;
class QLabel;
class MainWindow;
class Inventory;

// Виджет инвентаря
class Inventory : public QWidget {
	Q_OBJECT
	QTableWidget* mInventory;
	QPushButton* mMenuButton;
	QLabel* mApple;

public:
	explicit Inventory(uint32_t w, uint32_t h, MainWindow *parent = nullptr);
	void reset();
	void disable();
	void enable();
protected:
	void resizeEvent(QResizeEvent* event);
	void mousePressEvent(QMouseEvent* event);
};

#endif // INVENTORY_HPP
