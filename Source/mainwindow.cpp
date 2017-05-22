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
}

void MainWindow::CreateMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
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

void MainWindow::Save()
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
            outputStream << code;
            file.close();

            statusMessage = tr("Saved file");
        }
    }


    statusBar()->showMessage(statusMessage);
}

void MainWindow::SaveAs()
{
    statusMessage = tr("Saved file");
    statusBar()->showMessage(statusMessage);
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
