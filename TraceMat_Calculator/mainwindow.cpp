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

    //QWidget *widget = new QWidget;

    QHBoxLayout *h_layout = new QHBoxLayout;
    h_layout->addLayout(createDataLayout());

    QVBoxLayout *button_layout = new QVBoxLayout;
    QHBoxLayout *add_update_layout = new QHBoxLayout();

    QVBoxLayout *add_form = createAddLayout();
    add_update_layout->addLayout(add_form);

    QVBoxLayout *update_form = createUpdateLayout();
    add_update_layout->addLayout(update_form);

    button_layout->addLayout(add_update_layout);

    QVBoxLayout *view_form = createViewLayout();
    button_layout->addLayout(view_form);

    h_layout->addLayout(button_layout);

    ui->centralwidget->setLayout(h_layout);

    connect(addCharButton, &QPushButton::clicked, this, &MainWindow::handleAddCharClicked);
    connect(addWepButton, &QPushButton::clicked, this, &MainWindow::handleAddWepClicked);
    connect(updateCharButton, &QPushButton::clicked, this, &MainWindow::handleUpdateCharClicked);
    connect(updateWepButton, &QPushButton::clicked, this, &MainWindow::handleUpdateWepClicked);
    connect(viewCharBox, &QComboBox::currentIndexChanged, this, &MainWindow::handleCharBoxEdited);
    connect(viewWepBox, &QComboBox::currentIndexChanged, this, &MainWindow::handleWepBoxEdited);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleAddCharClicked(){
    const auto name = nameCharEdit->text().toStdString();
    const auto path = pathCharEdit->text().toStdString();
    const auto rarity = rarityCharEdit->text().toInt();

    if(model->addCharacter(name, path, rarity) == DataModel::Errors::SUCCESS){
        updateCharBox->addItem(QString::fromStdString(name));
        viewCharBox->addItem(QString::fromStdString(name));
    }
}

void MainWindow::handleAddWepClicked(){
    const auto name = nameWepEdit->text().toStdString();
    const auto path = pathWepEdit->text().toStdString();
    const auto rarity = rarityWepEdit->text().toInt();

    if(model->addWeapon(name, path, rarity) == DataModel::Errors::SUCCESS){
        updateWepBox->addItem(QString::fromStdString(name));
        viewWepBox->addItem(QString::fromStdString(name));
    }
}

void MainWindow::handleUpdateCharClicked(){
    const auto name = updateCharBox->currentText().toStdString();
    const auto purples = charPurpleEdit->text().toInt();
    const auto blues = charBlueEdit->text().toInt();
    const auto greens = charGreenEdit->text().toInt();

    if(model->updateCharMats(name, purples, blues, greens) == DataModel::Errors::SUCCESS){
        Character *c = model->getChar(name);
        vector<int> mats = c->getMaterials();
        nameChar->setText(QString::fromStdString(name));
        purplesChar->setText(QString::number(mats[0]));
        bluesChar->setText(QString::number(mats[1]));
        greensChar->setText(QString::number(mats[2]));
    }
}

void MainWindow::handleUpdateWepClicked(){
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

void MainWindow::handleCharBoxEdited(){
    const auto name = viewCharBox->currentText().toStdString();
    Character *c = model->getChar(name);
    vector<int> mats = c->getMaterials();
    nameChar->setText(QString::fromStdString(name));
    purplesChar->setText(QString::number(mats[0]));
    bluesChar->setText(QString::number(mats[1]));
    greensChar->setText(QString::number(mats[2]));
}
void MainWindow::handleWepBoxEdited(){
    const auto name = viewWepBox->currentText().toStdString();
    Weapon *w = model->getWep(name);
    vector<int> mats = w->getMaterials();
    nameWep->setText(QString::fromStdString(name));
    purplesWep->setText(QString::number(mats[0]));
    bluesWep->setText(QString::number(mats[1]));
    greensWep->setText(QString::number(mats[2]));
}

QVBoxLayout* MainWindow::createDataLayout(){
    model = new DataModel(this);
    QVBoxLayout *data_form_layout = new QVBoxLayout;
    data_form_layout->addWidget(new QLabel("Character Materials", this));
    QFormLayout *formCharLayout = new QFormLayout;

    nameChar = new QLabel(this);
    purplesChar = new QLabel(this);
    bluesChar = new QLabel(this);
    greensChar = new QLabel(this);

    formCharLayout->addRow("Name: ", nameChar);
    formCharLayout->addRow("Purples Required: ", purplesChar);
    formCharLayout->addRow("Blues Required: ", bluesChar);
    formCharLayout->addRow("Greens Required: ", greensChar);

    data_form_layout->addLayout(formCharLayout);

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

QVBoxLayout* MainWindow::createAddLayout(){
    //Add Character
    QVBoxLayout *add_form_layout = new QVBoxLayout;
    add_form_layout->addWidget(new QLabel("Add Character", this));
    QFormLayout *formCharLayout = new QFormLayout;

    nameCharEdit = new QLineEdit(this);
    rarityCharEdit = new QLineEdit(this);
    pathCharEdit = new QLineEdit(this);

    formCharLayout->addRow("Name: ", nameCharEdit);
    formCharLayout->addRow("Rarity: ", rarityCharEdit);
    formCharLayout->addRow("Path: ", pathCharEdit);

    add_form_layout->addLayout(formCharLayout);
    addCharButton = new QPushButton("Add", this);
    add_form_layout->addWidget(addCharButton);

    //Add Weapon
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

QVBoxLayout* MainWindow::createUpdateLayout(){
    //Update Character Mats
    QVBoxLayout *update_form_layout = new QVBoxLayout;
    update_form_layout->addWidget(new QLabel("Required Character Materials", this));
    QFormLayout *formCharLayout = new QFormLayout;

    charPurpleEdit = new QLineEdit(this);
    charBlueEdit = new QLineEdit(this);
    charGreenEdit = new QLineEdit(this);

    updateCharBox = new QComboBox(this);
    updateCharBox->setEditable(true);
    vector<Character*> c_list = model->getCharList();
    for(auto c: c_list){
        updateCharBox->addItem(QString::fromStdString(c->getName()));
    }

    updateCharBox->setCurrentIndex(-1);
    updateCharBox->setPlaceholderText("");

    formCharLayout->addRow("Name: ", updateCharBox);
    formCharLayout->addRow("Purples: ", charPurpleEdit);
    formCharLayout->addRow("Blues: ", charBlueEdit);
    formCharLayout->addRow("Greens: ", charGreenEdit);

    update_form_layout->addLayout(formCharLayout);
    updateCharButton = new QPushButton("Update", this);
    update_form_layout->addWidget(updateCharButton);

    //Update Weapon Mats
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

QVBoxLayout* MainWindow::createViewLayout(){
    //View Character
    QVBoxLayout *view_form_layout = new QVBoxLayout;
    view_form_layout->addWidget(new QLabel("View Character", this));

    viewCharBox = new QComboBox(this);
    viewCharBox->setEditable(true);
    vector<Character*> c_list = model->getCharList();
    for(auto c: c_list){
        viewCharBox->addItem(QString::fromStdString(c->getName()));
    }

    viewCharBox->setCurrentIndex(-1);
    viewCharBox->setPlaceholderText("");

    view_form_layout->addWidget(viewCharBox);

    //View Weapon
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
