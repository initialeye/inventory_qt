#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Inventory;
class QResizeEvent;

class MainWindow : public QMainWindow {
	Q_OBJECT

	Inventory* mInventory;
	QWidget* mMainMenu;
	QWidget* mCentralWidget;

public:
	MainWindow(QWidget *parent = nullptr);

protected:
	void resizeEvent(QResizeEvent* event);

public slots:
	void newGamePressed();
	void mainMenuPressed();
	void exitPressed();
};
#endif // MAINWINDOW_H
