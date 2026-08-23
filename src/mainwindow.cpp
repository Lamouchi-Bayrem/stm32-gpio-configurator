#include "mainwindow.h"
#include <QtWidgets>
#include <algorithm>
#ifndef GPIO_TEMPLATE_DIR
#define GPIO_TEMPLATE_DIR "templates"
#endif
namespace {
QString hex32(quint32 value){ return QStringLiteral("0x%1UL").arg(value,8,16,QLatin1Char('0')).toUpper(); }
QString copyFailure(const QString &path){ return QStringLiteral("Could not copy template: %1").arg(path); }
bool copyTree(const QString &sourcePath,const QString &destinationPath,QString *error){
    const QDir source(sourcePath); if(!source.exists()){*error=QStringLiteral("Template directory not found: %1").arg(sourcePath);return false;}
    QDir destination; if(!destination.mkpath(destinationPath)){*error=QStringLiteral("Cannot create: %1").arg(destinationPath);return false;}
    for(const QFileInfo &entry:source.entryInfoList(QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot)){
        const QString target=QDir(destinationPath).filePath(entry.fileName());
        if(entry.isDir()){if(!copyTree(entry.absoluteFilePath(),target,error)){return false;}}
        else {QFile::remove(target);if(!QFile::copy(entry.absoluteFilePath(),target)){*error=copyFailure(entry.absoluteFilePath());return false;}}
    } return true;
}
bool writeText(const QString &path,const QString &text,QString *error){ QFile file(path);if(!file.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Truncate)){*error=file.errorString();return false;}return file.write(text.toUtf8())==text.toUtf8().size(); }
QString modeName(int i){return QStringList{"GPIO_MODE_INPUT","GPIO_MODE_OUTPUT","GPIO_MODE_ALTERNATE","GPIO_MODE_ANALOG"}.at(i);}
QString typeName(int i){return QStringList{"GPIO_OUTPUT_PUSH_PULL","GPIO_OUTPUT_OPEN_DRAIN"}.at(i);}
QString speedName(int i){return QStringList{"GPIO_SPEED_LOW","GPIO_SPEED_MEDIUM","GPIO_SPEED_FAST","GPIO_SPEED_HIGH"}.at(i);}
QString pullName(int i){return QStringList{"GPIO_PULL_NONE","GPIO_PULL_UP","GPIO_PULL_DOWN"}.at(i);}
QString levelName(int i){return QStringList{"GPIO_LEVEL_LOW","GPIO_LEVEL_HIGH"}.at(i);}
}
MainWindow::MainWindow(QWidget *parent):QMainWindow(parent){
    loadBoards(); auto *central=new QWidget(this);auto *grid=new QGridLayout(central);int row=0;
    boardBox=new QComboBox(central);for(const auto &b:boards){boardBox->addItem(b.name);}
    portBox=new QComboBox(central);for(char p='A';p<='I';++p){portBox->addItem(QStringLiteral("GPIO%1").arg(QChar(p)));}
    modeBox=new QComboBox(central);modeBox->addItems({"Input","Output","Alternate function","Analog"});
    typeBox=new QComboBox(central);typeBox->addItems({"Push-pull","Open-drain"});
    speedBox=new QComboBox(central);speedBox->addItems({"Low","Medium","Fast","High"});
    pullBox=new QComboBox(central);pullBox->addItems({"No pull","Pull-up","Pull-down"});
    levelBox=new QComboBox(central);levelBox->addItems({"Low","High"});
    pinList=new QListWidget(central);pinList->setSelectionMode(QAbstractItemView::ExtendedSelection);for(int pin=0;pin<16;++pin){pinList->addItem(QStringLiteral("Pin %1").arg(pin));}
    preview=new QTextEdit(central);preview->setReadOnly(true);preview->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    auto add=[&](const QString &label,QWidget *widget){grid->addWidget(new QLabel(label,central),row,0);grid->addWidget(widget,row,1);++row;};
    add("Board / MCU",boardBox);add("GPIO port",portBox);add("Mode",modeBox);add("Output type",typeBox);add("Speed",speedBox);add("Pull",pullBox);add("Initial level",levelBox);add("Pins (Ctrl-click for multiple)",pinList);
    auto *buttons=new QHBoxLayout;auto *generate=new QPushButton("Generate preview",central);auto *exportButton=new QPushButton("Export MISRA-oriented project",central);buttons->addWidget(generate);buttons->addWidget(exportButton);grid->addLayout(buttons,row++,0,1,2);grid->addWidget(preview,row++,0,1,2);statusLabel=new QLabel(central);statusLabel->setWordWrap(true);grid->addWidget(statusLabel,row,0,1,2);
    connect(boardBox,&QComboBox::currentIndexChanged,this,[this](int){updateBoard();});connect(generate,&QPushButton::clicked,this,[this]{generatePreview();});connect(exportButton,&QPushButton::clicked,this,[this]{exportProject();});
    setCentralWidget(central);setWindowTitle("STM32 GPIO Configurator - MISRA-oriented generator");resize(900,800);updateBoard();
}
void MainWindow::loadBoards(){boards={
    {"NUCLEO-F401RE","STM32F401RE","RM0368 + UM1724",0x40020000UL,0x40023800UL,0,5},
    {"NUCLEO-F411RE","STM32F411RE","RM0383 + UM1724",0x40020000UL,0x40023800UL,0,5},
    {"NUCLEO-F446RE","STM32F446RE","RM0390 + UM1724",0x40020000UL,0x40023800UL,0,5},
    {"STM32F407G-DISC1","STM32F407VG","RM0090 + UM1472",0x40020000UL,0x40023800UL,3,12}};}
void MainWindow::updateBoard(){const auto &b=boards.at(boardBox->currentIndex());portBox->setCurrentIndex(b.defaultPort);pinList->clearSelection();pinList->item(b.defaultPin)->setSelected(true);statusLabel->setText(QStringLiteral("Profile: %1. Generated output is MISRA-oriented, not a formal compliance claim. Verify the exact device documents and errata.").arg(b.documents));generatePreview();}
QString MainWindow::registerPreview() const{quint32 moder=0,otyper=0,speed=0,pull=0,odr=0;for(auto *item:pinList->selectedItems()){int pin=pinList->row(item);moder|=(quint32(modeBox->currentIndex())&3UL)<<(2*pin);otyper|=(quint32(typeBox->currentIndex())&1UL)<<pin;speed|=(quint32(speedBox->currentIndex())&3UL)<<(2*pin);pull|=(quint32(pullBox->currentIndex())&3UL)<<(2*pin);if(levelBox->currentIndex()==1){odr|=1UL<<pin;}}return QStringLiteral("MODER   %1\nOTYPER  %2\nOSPEEDR %3\nPUPDR   %4\nODR     %5").arg(hex32(moder),hex32(otyper),hex32(speed),hex32(pull),hex32(odr));}
QString MainWindow::makeConfigHeader() const{return QStringLiteral("#ifndef GPIO_CONFIG_H\n#define GPIO_CONFIG_H\n#include \"gpio.h\"\nGpioStatus_t Gpio_ConfigGenerated(void);\n#endif\n");}
QString MainWindow::makeConfigSource() const{
    const auto &b=boards.at(boardBox->currentIndex());const int port=portBox->currentIndex();const QChar letter=portBox->currentText().back();QList<QListWidgetItem*> pins=pinList->selectedItems();std::sort(pins.begin(),pins.end(),[this](auto *a,auto *c){return pinList->row(a)<pinList->row(c);});
    QString s=QStringLiteral("#include \"gpio_config.h\"\n#include <stdint.h>\n\n#define GPIO_BASE_ADDRESS (%1)\n#define RCC_BASE_ADDRESS (%2)\n#define RCC_AHB1ENR_OFFSET (0x30UL)\n#define RCC_GPIO_ENABLE_MASK (%3)\n#define GPIO_SELECTED ((GpioRegisters_t *)(GPIO_BASE_ADDRESS + (%4UL * 0x400UL)))\n#define RCC_AHB1ENR (*(volatile uint32_t *)(RCC_BASE_ADDRESS + RCC_AHB1ENR_OFFSET))\n\n").arg(hex32(b.gpioBase),hex32(b.rccBase),hex32(1UL<<port)).arg(port);
    int n=0;for(auto *item:pins){int pin=pinList->row(item);s+=QStringLiteral("static const GpioPinConfig_t gpioConfig%1 =\n{\n    %2U,\n    %3,\n    %4,\n    %5,\n    %6,\n    %7\n};\n\n").arg(n++).arg(pin).arg(modeName(modeBox->currentIndex()),typeName(typeBox->currentIndex()),speedName(speedBox->currentIndex()),pullName(pullBox->currentIndex()),levelName(levelBox->currentIndex()));}
    s+=QStringLiteral("GpioStatus_t Gpio_ConfigGenerated(void)\n{\n    GpioStatus_t status = GPIO_STATUS_OK;\n    RCC_AHB1ENR |= RCC_GPIO_ENABLE_MASK;\n    (void)RCC_AHB1ENR;\n");for(int i=0;i<n;++i){s+=QStringLiteral("    if (status == GPIO_STATUS_OK)\n    {\n        status = Gpio_InitPin(GPIO_SELECTED, &gpioConfig%1);\n    }\n").arg(i);}s+=QStringLiteral("    return status;\n}\n");Q_UNUSED(letter);return s;}
void MainWindow::generatePreview(){if(pinList->selectedItems().isEmpty()){preview->setPlainText("Select at least one pin.");return;}preview->setPlainText(registerPreview()+"\n\n"+makeConfigSource());}
void MainWindow::exportProject(){if(pinList->selectedItems().isEmpty()){QMessageBox::warning(this,"No pins","Select at least one pin.");return;}const QString target=QFileDialog::getExistingDirectory(this,"Choose export directory");if(target.isEmpty()){return;}QString error;if(!copyTree(QStringLiteral(GPIO_TEMPLATE_DIR),target,&error)){QMessageBox::critical(this,"Export failed",error);return;}if(!writeText(QDir(target).filePath("Core/Inc/gpio_config.h"),makeConfigHeader(),&error)||!writeText(QDir(target).filePath("Core/Src/gpio_config.c"),makeConfigSource(),&error)){QMessageBox::critical(this,"Export failed",error);return;}QMessageBox::information(this,"Export complete",QStringLiteral("Generated MISRA-oriented driver, configuration, tests, analysis scripts, and compliance documents in:\n%1").arg(target));}
