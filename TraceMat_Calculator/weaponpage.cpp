#include "weaponpage.h"
#include "datamodel.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QFormLayout>

WeaponPage::WeaponPage(QObject *parent) {
    QHBoxLayout *h_layout = new QHBoxLayout;
    h_layout->addLayout(createDataLayout());

    QVBoxLayout *button_layout = new QVBoxLayout;
    QHBoxLayout *add_update_layout = new QHBoxLayout;

    QVBoxLayout *add_form = createAddLayout();
    add_update_layout->addLayout(add_form);

    QVBoxLayout *update_form = createUpdateLayout();
    add_update_layout->addLayout(update_form);

    button_layout->addLayout(add_update_layout);

    QVBoxLayout *view_form = createViewLayout();
    button_layout->addLayout(view_form);

    h_layout->addLayout(button_layout);
    setLayout(h_layout);
}

void WeaponPage::handleAddWepClicked(){
    const auto name = nameWepEdit->text().toStdString();
    const auto path = pathWepEdit->text().toStdString();
    const auto rarity = rarityWepEdit->text().toInt();

    if(model->addWeapon(name, path, rarity) == DataModel::Errors::SUCCESS){
        updateWepBox->addItem(QString::fromStdString(name));
        viewWepBox->addItem(QString::fromStdString(name));
    }
}

void WeaponPage::handleUpdateWepClicked(){
    const auto name = updateWepBox->currentText().toStdString();
    const auto purples = wepPurpleEdit->text().toInt();
    const auto blues = wepBlueEdit->text().toInt();
    const auto greens = wepGreenEdit->text().toInt();

    if(model->updateWepMats(name, purples, blues, greens) == DataModel::Errors::SUCCESS){
        Weapon *w = model->getWep(name);
        vector<int> mats = w->getMaterials();
        nameWep->setText(QString::fromStdString(name));
        purplesWep->setText(QString::number(mats[0]));
        bluesWep->setText(QString::number(mats[1]));
        greensWep->setText(QString::number(mats[2]));
    }
}

void WeaponPage::handleWepBoxEdited(){
    const auto name = viewWepBox->currentText().toStdString();
    Weapon *w = model->getWep(name);
    vector<int> mats = w->getMaterials();
    nameWep->setText(QString::fromStdString(name));
    purplesWep->setText(QString::number(mats[0]));
    bluesWep->setText(QString::number(mats[1]));
    greensWep->setText(QString::number(mats[2]));
}

QVBoxLayout* WeaponPage::createDataLayout(){
    model = new DataModel(this);
    QVBoxLayout *data_form_layout = new QVBoxLayout;

    data_form_layout->addWidget(new QLabel("Weapon Materials", this));
    QFormLayout *formWepLayout = new QFormLayout;

    nameWep = new QLabel(this);
    purplesWep = new QLabel(this);
    bluesWep = new QLabel(this);
    greensWep = new QLabel(this);

    formWepLayout->addRow("Name: ", nameWep);
    formWepLayout->addRow("Purples Required: ", purplesWep);
    formWepLayout->addRow("Blues Required: ", bluesWep);
    formWepLayout->addRow("Greens Required: ", greensWep);

    data_form_layout->addLayout(formWepLayout);

    return data_form_layout;
}

QVBoxLayout* WeaponPage::createAddLayout(){
    //Add Weapon
    QVBoxLayout *add_form_layout = new QVBoxLayout();
    add_form_layout->addWidget(new QLabel("Add Weapon", this));
    QFormLayout *formWepLayout = new QFormLayout;

    nameWepEdit = new QLineEdit(this);
    rarityWepEdit = new QLineEdit(this);
    pathWepEdit = new QLineEdit(this);

    formWepLayout->addRow("Name: ", nameWepEdit);
    formWepLayout->addRow("Rarity: ", rarityWepEdit);
    formWepLayout->addRow("Path: ", pathWepEdit);

    add_form_layout->addLayout(formWepLayout);
    addWepButton = new QPushButton("Add", this);
    add_form_layout->addWidget(addWepButton);

    return add_form_layout;
}

QVBoxLayout* WeaponPage::createUpdateLayout(){
    //Update Weapon Mats
    QVBoxLayout *update_form_layout = new QVBoxLayout;
    update_form_layout->addWidget(new QLabel("Required Weapon Materials", this));
    QFormLayout *formWepLayout = new QFormLayout;

    wepPurpleEdit = new QLineEdit(this);
    wepBlueEdit = new QLineEdit(this);
    wepGreenEdit = new QLineEdit(this);

    updateWepBox = new QComboBox(this);
    updateWepBox->setEditable(true);
    vector<Weapon*> w_list = model->getWepList();
    for(auto w: w_list){
        updateWepBox->addItem(QString::fromStdString(w->getName()));
    }

    updateWepBox->setCurrentIndex(-1);
    updateWepBox->setPlaceholderText("");

    formWepLayout->addRow("Name: ", updateWepBox);
    formWepLayout->addRow("Purples: ", wepPurpleEdit);
    formWepLayout->addRow("Blues: ", wepBlueEdit);
    formWepLayout->addRow("Greens: ", wepGreenEdit);

    update_form_layout->addLayout(formWepLayout);
    updateWepButton = new QPushButton("Update", this);
    update_form_layout->addWidget(updateWepButton);

    return update_form_layout;
}

QVBoxLayout* WeaponPage::createViewLayout(){
    //View Weapon
    QVBoxLayout *view_form_layout = new QVBoxLayout;
    view_form_layout->addWidget(new QLabel("View Weapon", this));

    viewWepBox = new QComboBox(this);
    viewWepBox->setEditable(true);
    vector<Weapon*> w_list = model->getWepList();
    for(auto w: w_list){
        viewWepBox->addItem(QString::fromStdString(w->getName()));
    }

    viewWepBox->setCurrentIndex(-1);
    viewWepBox->setPlaceholderText("");

    view_form_layout->addWidget(viewWepBox);
    return view_form_layout;
}
