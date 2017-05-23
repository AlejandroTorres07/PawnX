#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

#ifdef Q_OS_MAC
    setUnifiedTitleAndToolBarOnMac(true);
#endif

    CreateActions();
    CreateMenus();

    MainWindow::setWindowTitle("Untitled - PawnX");

    QFont font;
    font.setFamily("Courier");
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    font.setPointSize(20);

    highlighter = new Highlighter(ui->textEdit->document());
    ui->textEdit->setFont(font);

    const int tabStop = 4;  // 4 characters

    QFontMetrics metrics(font);
    ui->textEdit->setTabStopWidth(tabStop * metrics.width(' '));

    statusMessage = tr("Program Ready...");
    statusBar()->showMessage(statusMessage);
}

void MainWindow::CreateActions()
{
    newAct = new QAction(tr("&New File"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new script"));
    connect(newAct, &QAction::triggered, this, &MainWindow::NewFile);

    openAct = new QAction(tr("&Open File..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing script"));
    connect(openAct, &QAction::triggered, this, &MainWindow::OpenFile);

    saveAct = new QAction(tr("&Save File..."), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save current script"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::Save);

    saveAsAct = new QAction(tr("&Save File As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save current script"));
    connect(saveAsAct, &QAction::triggered, this, &MainWindow::SaveAs);

#ifndef Q_OS_MAC
    exitAct = new QAction(tr("&Close"), this);
    exitAct->setShortcuts(QKeySequence::Close);
    exitAct->setStatusTip(tr("Exit PawnX"));
    connect(exitAct, &QAction::triggered, this, &MainWindow::Exit);
#endif
}

void MainWindow::CreateMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
#ifndef Q_OS_MAC
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);
#endif
}

void MainWindow::NewFile()
{
    QString fileName;

    if(!fileName.isEmpty())
    {
        statusMessage = tr("Created a new script with Template");
    }
    else statusMessage = tr("Created a new script");
    MainWindow::setWindowTitle("Untitled - PawnX");
    statusBar()->showMessage(statusMessage);
    fileChanged = 0;
}

void MainWindow::OpenFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), QString(),
                                    tr("Pawn File (*.pwn *.p *.pawn);;Text Files (*.txt);;All types (*.*)"));
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        QFileInfo fileInfo(fileName);

        if (!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            statusMessage = tr("Could not open file");
        }
        else
        {
            QTextStream in(&file);
            //ui->textEdit->setPlainText(in.readAll());
            ui->textEdit->setPlainText(in.readAll());

            currentFile = fileInfo.completeBaseName();

            MainWindow::setWindowTitle(currentFile + " - PawnX");
            statusMessage = tr("Opened file");
            file.close();
        }
    }
    else statusMessage = tr("File is empty");
    statusBar()->showMessage(statusMessage);
}

int MainWindow::Save()
{
    if(currentFilePath.isEmpty())
    {
        SaveAs();
        return 0;
    }

    QFile file(currentFilePath);
    QFileInfo fileInfo(currentFilePath);

    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::information(this, tr("Unable to open file"),
            file.errorString());
        statusMessage = tr("Saved file");
    }
    else
    {

        QTextStream outputStream(&file);
        QString code = ui->textEdit->toPlainText();

        currentFile = fileInfo.completeBaseName();
        currentFilePath = fileInfo.absoluteFilePath();

        MainWindow::setWindowTitle(currentFile + " - PawnX");

        outputStream << code;
        file.close();

        statusMessage = tr("Saved file");
    }

    statusMessage = tr("Saved file");
    statusBar()->showMessage(statusMessage);
    fileChanged = 0;
    return 0;
}

void MainWindow::SaveAs()
{

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QString(),
                                tr("Pawn File (*.pwn *.p *.pawn);;Text Files (*.txt);;All types (*.*)"));

    if (fileName.isEmpty())
    {
        statusMessage = tr("File is empty");
    }
    else
    {
        QFile file(fileName);
        QFileInfo fileInfo(fileName);
        if (!file.open(QIODevice::WriteOnly))
        {
            QMessageBox::information(this, tr("Unable to open file"),
                file.errorString());
            statusMessage = tr("Saved file");
        }
        else
        {

            QTextStream outputStream(&file);
            QString code = ui->textEdit->toPlainText();

            currentFile = fileInfo.completeBaseName();
            currentFilePath = fileInfo.absoluteFilePath();

            MainWindow::setWindowTitle(currentFile + " - PawnX");

            outputStream << code;
            file.close();

            statusMessage = tr("Saved file");
        }
    }
    fileChanged = 0;
    statusBar()->showMessage(statusMessage);
}

void MainWindow::Exit()
{
    QApplication::quit();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    ExitConfirm();
}

void MainWindow::ExitConfirm()
{
    if (!ui->textEdit->document()->isModified())
    {
            Exit();
    }
    else
    {
        const QMessageBox::StandardButton ret
            = QMessageBox::warning(this, tr("Application"),
                                   tr("The document has been modified.\n"
                                      "Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        switch (ret)
        {
            case QMessageBox::Save:
                Save();
                break;
            case QMessageBox::Cancel:
                //Cancel Exit
                break;
            default:
                Exit();
                break;
        }

    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNewFile_triggered()
{
    NewFile();
}

void MainWindow::on_actionSaveFile_triggered()
{
    Save();
}

void MainWindow::on_textEdit_textChanged()
{
    fileChanged = 1;
}

void MainWindow::on_actionOpenFile_triggered()
{
    OpenFile();
}
