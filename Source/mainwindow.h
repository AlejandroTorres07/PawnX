#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QProcess>

#include "highlighter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void CreateActions();
    void CreateMenus();

    QMenu *fileMenu;

    QString statusMessage;

    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;

    Highlighter *highlighter;

    QString currentFile;

private slots:
    void NewFile();
    void OpenFile();
    void Save();
    void SaveAs();
    /*void Undo();
    void Redo();
    void Cut();
    void Copy();
    void About();*/

    void on_actionNewFile_triggered();
    void on_actionSaveFile_triggered();
};

#endif // MAINWINDOW_H
