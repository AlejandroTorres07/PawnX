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
#ifndef Q_OS_MAC
    QAction *exitAct;
#endif

    Highlighter *highlighter;

    QString currentFile;
    QString currentFilePath;
    int fileChanged;

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void NewFile();
    void OpenFile();
    int Save();
    void SaveAs();
    void Exit();
    bool ExitConfirm();
    /*void Undo();
    void Redo();
    void Cut();
    void Copy();
    void About();*/

    void on_actionNewFile_triggered();
    void on_actionSaveFile_triggered();
    void on_textEdit_textChanged();
    void on_actionOpenFile_triggered();
};

#endif // MAINWINDOW_H
