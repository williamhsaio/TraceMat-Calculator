#include "characterpage.h"
#include "datamodel.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>

CharacterPage::CharacterPage(QObject *parent) {
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

    connect(addCharButton, &QPushButton::clicked, this, &CharacterPage::handleAddCharClicked);
    connect(updateCharButton, &QPushButton::clicked, this, &CharacterPage::handleUpdateCharClicked);
    connect(viewCharBox, &QComboBox::currentIndexChanged, this, &CharacterPage::handleCharBoxEdited);
}

void CharacterPage::handleAddCharClicked(){
    const auto name = nameCharEdit->text().toStdString();
    const auto path = pathCharEdit->text().toStdString();
    const auto rarity = rarityCharEdit->text().toInt();

    if(model->addCharacter(name, path, rarity) == DataModel::Errors::SUCCESS){
        updateCharBox->addItem(QString::fromStdString(name));
        viewCharBox->addItem(QString::fromStdString(name));
    }
}

void CharacterPage::handleUpdateCharClicked(){
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

void CharacterPage::handleCharBoxEdited(){
    const auto name = viewCharBox->currentText().toStdString();
    Character *c = model->getChar(name);
    vector<int> mats = c->getMaterials();
    nameChar->setText(QString::fromStdString(name));
    purplesChar->setText(QString::number(mats[0]));
    bluesChar->setText(QString::number(mats[1]));
    greensChar->setText(QString::number(mats[2]));
}

QVBoxLayout* CharacterPage::createDataLayout(){
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

    return data_form_layout;
}

QVBoxLayout* CharacterPage::createAddLayout(){
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

    return add_form_layout;
}

QVBoxLayout* CharacterPage::createUpdateLayout(){
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

    return update_form_layout;
}

QVBoxLayout* CharacterPage::createViewLayout(){
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

    return view_form_layout;
}


