#ifndef WEAPONPAGE_H
#define WEAPONPAGE_H

#include <QWidget>

class DataModel;
class QLabel;
class QLineEdit;
class QPushButton;
class QComboBox;
class QVBoxLayout;

class WeaponPage: public QWidget
{
    Q_OBJECT
public:
    explicit WeaponPage(QObject *parent = nullptr);
    QVBoxLayout* createDataLayout();
    QVBoxLayout* createAddLayout();
    QVBoxLayout* createUpdateLayout();
    QVBoxLayout* createViewLayout();

private slots:
    void handleAddWepClicked();
    void handleUpdateWepClicked();
    void handleWepBoxEdited();

private:
    DataModel *model;

    QLabel *nameWep;
    QLabel *purplesWep;
    QLabel *bluesWep;
    QLabel *greensWep;

    QLineEdit *nameWepEdit;
    QLineEdit *rarityWepEdit;
    QLineEdit *pathWepEdit;
    QLineEdit *wepPurpleEdit;
    QLineEdit *wepBlueEdit;
    QLineEdit *wepGreenEdit;

    QPushButton *addWepButton;
    QPushButton *updateWepButton;

    QComboBox *updateWepBox;
    QComboBox *viewWepBox;
};

#endif // WEAPONPAGE_H
