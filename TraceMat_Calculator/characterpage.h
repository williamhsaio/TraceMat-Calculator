#ifndef CHARACTERPAGE_H
#define CHARACTERPAGE_H

#include <QWidget>

class QLabel;
class QLineEdit;
class QPushButton;
class QComboBox;
class QVBoxLayout;
class QCheckBox;
class QSlider;
class DataModel;

class CharacterPage: public QWidget
{
    Q_OBJECT
public:
    explicit CharacterPage(QObject *parent = nullptr);
    QVBoxLayout* createDataLayout();
    QVBoxLayout* createAddLayout();
    QVBoxLayout* createUpdateLayout();
    QVBoxLayout* createViewLayout();

private slots:
    void handleAddCharClicked();
    void handleUpdateCharClicked();
    void handleCharBoxEdited();
    void handleBasicSlider();
    // void handleSkillSlider();
    // void handleTalentSlider();
    // void handleUltSlider();
    // void handleTraceChecked();

private:
    DataModel *model;

    QLabel *nameChar;
    QLabel *purplesChar;
    QLabel *bluesChar;
    QLabel *greensChar;
    QLabel *currentBasic;
    QLabel *currentSkill;
    QLabel *currentTalent;
    QLabel *currentUlt;

    QLineEdit *nameCharEdit;
    QLineEdit *rarityCharEdit;
    QLineEdit *pathCharEdit;
    QLineEdit *charPurpleEdit;
    QLineEdit *charBlueEdit;
    QLineEdit *charGreenEdit;

    QPushButton *addCharButton;
    QPushButton *updateCharButton;

    QComboBox *updateCharBox;
    QComboBox *viewCharBox;

    QCheckBox *traceCheck;

    QSlider *basicSlider;
    QSlider *skillSlider;
    QSlider *talentSlider;
    QSlider *ultSlider;
};

#endif // CHARACTERPAGE_H
