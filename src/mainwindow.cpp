#include "mainwindow.hpp"
#include "inventory.hpp"

#include <QPushButton>
#include <QDockWidget>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	mMainMenu = new QWidget;

	auto buttonsLayout = new QVBoxLayout(mMainMenu);
	auto button = new QPushButton;
	buttonsLayout->addWidget(button);
	button->setText("Новая игра");
	connect(button, SIGNAL(pressed()), this, SLOT(newGamePressed()));

	button = new QPushButton;
	buttonsLayout->addWidget(button);
	button->setText("Выход");
	connect(button, SIGNAL(pressed()), this, SLOT(exitPressed()));

	mCentralWidget = new QWidget;
	setCentralWidget(mCentralWidget);
	auto centralLayout = new QBoxLayout(QBoxLayout::LeftToRight, mCentralWidget);
	mCentralWidget->setLayout(centralLayout);
	mInventory = new Inventory(3, 3, this);
	mInventory->disable();
	centralLayout->addWidget(mInventory);
	centralLayout->addWidget(mMainMenu);

	setMinimumWidth(640);
	setMinimumHeight(480);
}

void MainWindow::resizeEvent(QResizeEvent* event) {
	(void)event;
	mMainMenu->setMinimumWidth(size().width()/4);
}

void MainWindow::newGamePressed() {
	mInventory->reset();
	mInventory->enable();
	mInventory->resize(size());
	mMainMenu->setVisible(false);
}

void MainWindow::mainMenuPressed() {
	mInventory->disable();
	mInventory->resize(size());
	mMainMenu->setVisible(true);
}

void MainWindow::exitPressed() {
	close();
}
