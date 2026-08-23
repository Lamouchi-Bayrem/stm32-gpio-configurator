#include "mainwindow.h"
#include <QtWidgets>
MainWindow::MainWindow(){
 auto *w=new QWidget; auto *l=new QVBoxLayout(w);
 portBox=new QComboBox; portBox->addItems({"GPIO_A","GPIO_B","GPIO_C","GPIO_D"});
 modeBox=new QComboBox; modeBox->addItems({"INPUT","OUTPUT","ALT","ANALOG"});
 typeBox=new QComboBox; typeBox->addItems({"PP","OD"});
 pinList=new QListWidget; pinList->setSelectionMode(QAbstractItemView::MultiSelection);
 for(int i=0;i<16;i++) pinList->addItem(QString("Pin %1").arg(i));
 auto *btn=new QPushButton("Generate");
 output=new QTextEdit;
 QObject::connect(btn,&QPushButton::clicked,[this]{generateCode();});
 l->addWidget(portBox); l->addWidget(modeBox); l->addWidget(typeBox); l->addWidget(pinList); l->addWidget(btn); l->addWidget(output);
 setCentralWidget(w);
}
void MainWindow::generateCode(){
 QString txt;
 for(auto *it: pinList->selectedItems()){
 int p=pinList->row(it);
 txt += QString("GPIO_Init(%1, %2, %3, %4);
")
 .arg(portBox->currentText()).arg(modeBox->currentText()).arg(typeBox->currentText()).arg(p);
 }
 txt += "
MODER Preview: 0x00000000 -> 0x10000000
";
 output->setText(txt);
}
