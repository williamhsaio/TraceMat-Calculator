#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QString>
#include <QFormLayout>
#include <QCoreApplication>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    tabWidget = new QTabWidget;
    charTab = new CharacterPage;
    wepTab = new WeaponPage;

    tabWidget->addTab(charTab, "Characters");
    tabWidget->addTab(wepTab, "Weapons");

    setCentralWidget(tabWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}
