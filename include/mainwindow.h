#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QVector>
class QComboBox; class QListWidget; class QTextEdit; class QLabel;
struct BoardDefinition { QString name; QString mcu; QString documents; quint32 gpioBase; quint32 rccBase; int defaultPort; int defaultPin; };
class MainWindow final : public QMainWindow {
    Q_OBJECT
public: explicit MainWindow(QWidget *parent = nullptr);
private:
    QComboBox *boardBox{}; QComboBox *portBox{}; QComboBox *modeBox{}; QComboBox *typeBox{};
    QComboBox *speedBox{}; QComboBox *pullBox{}; QComboBox *levelBox{};
    QListWidget *pinList{}; QTextEdit *preview{}; QLabel *statusLabel{};
    QVector<BoardDefinition> boards;
    void loadBoards(); void updateBoard(); void generatePreview(); void exportProject();
    QString makeConfigHeader() const; QString makeConfigSource() const; QString registerPreview() const;
};
#endif
