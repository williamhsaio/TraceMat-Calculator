#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "datamodel.h"
#include "characterpage.h"
#include "weaponpage.h"

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QTabWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    // QVBoxLayout* createDataLayout();
    // QVBoxLayout* createAddLayout();
    // QVBoxLayout* createUpdateLayout();
    // QVBoxLayout* createViewLayout();

private slots:
    // void handleAddCharClicked();
    // void handleAddWepClicked();
    // void handleUpdateCharClicked();
    // void handleUpdateWepClicked();
    // void handleCharBoxEdited();
    // void handleWepBoxEdited();

private:
    Ui::MainWindow *ui;
    QTabWidget *tabWidget;
    CharacterPage *charTab;
    WeaponPage *wepTab;

    // DataModel *model = nullptr;

    // QLabel *nameChar;
    // QLabel *purplesChar;
    // QLabel *bluesChar;
    // QLabel *greensChar;
    // QLabel *nameWep;
    // QLabel *purplesWep;
    // QLabel *bluesWep;
    // QLabel *greensWep;

    // QLineEdit *nameCharEdit;
    // QLineEdit *rarityCharEdit;
    // QLineEdit *pathCharEdit;
    // QLineEdit *nameWepEdit;
    // QLineEdit *rarityWepEdit;
    // QLineEdit *pathWepEdit;
    // QLineEdit *charPurpleEdit;
    // QLineEdit *charBlueEdit;
    // QLineEdit *charGreenEdit;
    // QLineEdit *wepPurpleEdit;
    // QLineEdit *wepBlueEdit;
    // QLineEdit *wepGreenEdit;

    // QPushButton *addCharButton;
    // QPushButton *addWepButton;
    // QPushButton *updateCharButton;
    // QPushButton *updateWepButton;

    // QComboBox *updateCharBox;
    // QComboBox *updateWepBox;
    // QComboBox *viewCharBox;
    // QComboBox *viewWepBox;
};
#endif // MAINWINDOW_H
