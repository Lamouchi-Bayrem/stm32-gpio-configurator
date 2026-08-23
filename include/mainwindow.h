#pragma once
#include <QMainWindow>
class QComboBox; class QListWidget; class QTextEdit;
class MainWindow : public QMainWindow {
 Q_OBJECT
public:
 MainWindow();
private:
 QComboBox *portBox,*modeBox,*typeBox;
 QListWidget *pinList;
 QTextEdit *output;
 void generateCode();
};
