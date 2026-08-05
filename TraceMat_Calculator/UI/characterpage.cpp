#include "characterpage.h"
#include "datamodel.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QSlider>
#include <QCheckBox>
#include <iostream>

using namespace std;

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
    connect(basicSlider, &QSlider::valueChanged, this, &CharacterPage::handleBasicSlider);
    connect(skillSlider, &QSlider::valueChanged, this, &CharacterPage::handleSkillSlider);
    connect(talentSlider, &QSlider::valueChanged, this, &CharacterPage::handleTalentSlider);
    connect(ultSlider, &QSlider::valueChanged, this, &CharacterPage::handleUltSlider);
    //connect(traceCheck, &QCheckBox::checkStateChanged, this, &CharacterPage::handleTraceChecked);
}

void CharacterPage::handleAddCharClicked(){
    const auto name = nameCharEdit->text().toStdString();
    const auto path = pathCharEdit->text().toStdString();
    const auto rarity = rarityCharEdit->text().toInt();
    const auto minBasic = basicSlider->minimum();
    const auto currBasic = basicSlider->value();
    const auto minSkill = skillSlider->minimum();
    const auto currSkill = skillSlider->value();
    const auto minTalent = talentSlider->minimum();
    const auto currTalent = talentSlider->value();
    const auto minUlt = ultSlider->minimum();
    const auto currUlt = ultSlider->value();

    if(model->addCharacter(name, path, rarity) == DataModel::Errors::SUCCESS){
        updateCharBox->addItem(QString::fromStdString(name));
        viewCharBox->addItem(QString::fromStdString(name));

        cout<<"Create vector of basic mats"<<endl;
        model->initializeCharMap(rarity);
        vector<int> basicMats = model->findCharBasic(minBasic, currBasic);
        cout<<"Basic Mats"<<endl;
        for(auto i: basicMats){
            cout<<i<<endl;
        }
        vector<int> skillMats = model->findCharMats(minSkill, currSkill);
        vector<int> talentMats = model->findCharMats(minTalent, currTalent);
        vector<int> ultMats = model->findCharMats(minUlt, currUlt);

        vector<int> total;
        total.push_back(basicMats[0] + skillMats[0] + talentMats[0] + ultMats[0]);
        total.push_back(basicMats[1] + skillMats[1] + talentMats[1] + ultMats[1]);
        total.push_back(basicMats[2] + skillMats[2] + talentMats[2] + ultMats[2]);

        Character *c = model->getChar(name);
        if(traceCheck->isChecked()){
            if(rarity == 5){
                c->setMaterials(total[0]+38, total[1]+16, total[2]+6);
            }
            else{
                c->setMaterials(total[0]+28, total[1]+12, total[2]+4);
            }
        }
        else{
            c->setMaterials(total[0], total[1], total[2]);
        }
        model->saveCharData(name, path, rarity, c->getMaterials());
        cout<<"total purples: " <<total[0]<<endl;
    }
}

void CharacterPage::handleUpdateCharClicked(){
    const auto name = updateCharBox->currentText().toStdString();
    const auto purples = charPurpleEdit->text().toInt();
    const auto blues = charBlueEdit->text().toInt();
    const auto greens = charGreenEdit->text().toInt();
    qDebug()<<"purple edit: "<<purples;
    if(model->updateCharMats(name, purples, blues, greens) == DataModel::Errors::SUCCESS){
        Character *c = model->getChar(name);
        vector<int> mats = c->getMaterials();
        qDebug()<<"Name: "<<c->getName();
        qDebug()<<"Mats: "<<mats[0]<<mats[1]<<mats[2];
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
    cout<<"Purples: " <<mats[0]<<endl;
    purplesChar->setText(QString::number(mats[0]));
    cout<<"Blues: " <<mats[1]<<endl;
    bluesChar->setText(QString::number(mats[1]));
    cout<<"Greens: " <<mats[2]<<endl;
    greensChar->setText(QString::number(mats[2]));
}

void CharacterPage::handleBasicSlider(){
    currentBasic->setText(QString::number(basicSlider->value()));
}

void CharacterPage::handleSkillSlider(){
    currentSkill->setText(QString::number(skillSlider->value()));
}

void CharacterPage::handleTalentSlider(){
    currentTalent->setText(QString::number(talentSlider->value()));
}

void CharacterPage::handleUltSlider(){
    currentUlt->setText(QString::number(ultSlider->value()));
}

void CharacterPage::handleTraceChecked(){
    const auto name = nameCharEdit->text().toStdString();
    const auto rarity = rarityCharEdit->text().toInt();
    Character *c = model->getChar(name);
    vector<int> mats = c->getMaterials();

    if(traceCheck->isChecked()){
        if(rarity == 5){
            c->setMaterials(mats[0]+38, mats[1]+16, mats[2]+6);
        }
        else{
            c->setMaterials(mats[0]+37, mats[1]+22, mats[2]+6);
        }
    }
    else{
        if(rarity == 5){
            c->setMaterials(mats[0]-38, mats[1]-16, mats[2]-6);
        }
        else{
            c->setMaterials(mats[0]-37, mats[1]-22, mats[2]-6);
        }
    }
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

    basicSlider = new QSlider(this);
    skillSlider = new QSlider(this);
    talentSlider = new QSlider(this);
    ultSlider = new QSlider(this);

    basicSlider->setRange(1,6);
    basicSlider->setOrientation(Qt::Horizontal);

    skillSlider->setRange(1,10);
    skillSlider->setOrientation(Qt::Horizontal);

    talentSlider->setRange(1,10);
    talentSlider->setOrientation(Qt::Horizontal);

    ultSlider->setRange(1,10);
    ultSlider->setOrientation(Qt::Horizontal);

    traceCheck = new QCheckBox(this);

    formCharLayout->addRow("Name: ", nameCharEdit);
    formCharLayout->addRow("Rarity: ", rarityCharEdit);
    formCharLayout->addRow("Path: ", pathCharEdit);

    //Basic Level Slider
    formCharLayout->addRow("Basic: ", basicSlider);
    QHBoxLayout *basicLvls = new QHBoxLayout;
    basicLvls->addWidget(new QLabel("Min: 1"));

    QFormLayout *basicCurrForm = new QFormLayout;
    currentBasic = new QLabel("1", this);
    basicCurrForm->addRow("Current:", currentBasic);
    basicLvls->addLayout(basicCurrForm);

    basicLvls->addWidget(new QLabel(" Max: 6"));
    formCharLayout->addRow("Level: ", basicLvls);

    //Skill Level Slider
    formCharLayout->addRow("Skill: ", skillSlider);
    QHBoxLayout *skillLvls = new QHBoxLayout;
    skillLvls->addWidget(new QLabel("Min: 1"));

    QFormLayout *skillCurrForm = new QFormLayout;
    currentSkill = new QLabel("1", this);
    skillCurrForm->addRow("Current:", currentSkill);
    skillLvls->addLayout(skillCurrForm);

    skillLvls->addWidget(new QLabel(" Max: 10"));
    formCharLayout->addRow("Level: ", skillLvls);

    //Talent Level Slider
    formCharLayout->addRow("Talent: ", talentSlider);
    QHBoxLayout *talentLvls = new QHBoxLayout;
    talentLvls->addWidget(new QLabel("Min: 1"));

    QFormLayout *talentCurrForm = new QFormLayout;
    currentTalent = new QLabel("1", this);
    talentCurrForm->addRow("Current:", currentTalent);
    talentLvls->addLayout(talentCurrForm);

    talentLvls->addWidget(new QLabel(" Max: 10"));
    formCharLayout->addRow("Level: ", talentLvls);

    //Ult Level Slider
    formCharLayout->addRow("Ultimate: ", ultSlider);
    QHBoxLayout *ultLvls = new QHBoxLayout;
    ultLvls->addWidget(new QLabel("Min: 1"));

    QFormLayout *ultCurrForm = new QFormLayout;
    currentUlt = new QLabel("1", this);
    ultCurrForm->addRow("Current:", currentUlt);
    ultLvls->addLayout(ultCurrForm);

    ultLvls->addWidget(new QLabel(" Max: 10"));
    formCharLayout->addRow("Level: ", ultLvls);

    formCharLayout->addRow("Max traces?", traceCheck);

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


