#include "mainwindow.hpp"
#include "db.hpp"

#include <QApplication>

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	Db::instance();
	MainWindow w;
	w.show();
	return a.exec();
}
