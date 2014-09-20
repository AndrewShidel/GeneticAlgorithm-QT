#include <iostream>
#include <fstream>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QString>
#include <QResizeEvent>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "newproject.h"
#include "openproject.h"
#include "rundialog.h"

std::string _exec(const char* cmd);
std::string _exec(const char* cmd, bool stderr);


std::string projName;
std::string projPath;
int test = 3;
std::string libPath = "";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->dockWidget_4->installEventFilter(this);

    QFont font;
    font.setFamily("Courier");
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    font.setPointSize(12);

    QTextEdit* editor = new QTextEdit();
    ui->main_code->setFont(font);
    ui->test_code->setFont(font);

    const int tabStop = 4;  // 4 characters
    QFontMetrics metrics(font);
    ui->main_code->setTabStopWidth(tabStop * metrics.width(' '));
    ui->test_code->setTabStopWidth(tabStop * metrics.width(' '));
    sizer();
    //ui->textEdit_3->toPlainText();
    //GE::GE ge("../UserSpace/", &blackBox, questions);
    //ge.stop();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_toolButton_clicked()
{
    newProject *wdg = new newProject(this);
    wdg->open();
}

/*void MainWindow::onNewClose(std::string pName,std::string pPath, bool opening){

    projName = pName;
    projPath = pPath;
    if (opening){
        QFile mainCode(QString((projPath+"/orgs/main.cpp").c_str()));
        if (!mainCode.open(QIODevice::ReadOnly | QIODevice::Text))
                 return;
        ui->main_code->setText(mainCode.readAll());

        QFile testCode(QString((projPath+"/runner.cpp").c_str()));
        if (!testCode.open(QIODevice::ReadOnly | QIODevice::Text))
                 return;
        ui->test_code->setText(testCode.readAll());
    }
}*/

void MainWindow::sizer(){
    const int newHeight = this->height()-88-ui->dockWidget_4->height();
    const int newWidth = this->width()-ui->tabWidget_2->width();
    ui->tabWidget->resize(newWidth, newHeight);
    ui->main_code->resize(newWidth, newHeight-7);
    ui->test_code->resize(newWidth, newHeight-7);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
  if (event->type() == QEvent::Resize && (obj == ui->dockWidget_4 || obj==this)) {
      //QResizeEvent *resizeEvent = static_cast<QResizeEvent*>(event);
      sizer();
  }

  if (event->type() == QEvent::KeyPress){
      QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

      int keyInt = keyEvent->key();
      Qt::Key key = static_cast<Qt::Key>(keyInt);
      if(key == Qt::Key_unknown){
          qDebug() << "Unknown key from a macro probably";
          return QWidget::eventFilter(obj, event);
      }
      // the user have clicked just and only the special keys Ctrl, Shift, Alt, Meta.
      if(key == Qt::Key_Control ||
          key == Qt::Key_Shift ||
          key == Qt::Key_Alt ||
          key == Qt::Key_Meta)
      {
          qDebug() << "Single click of special key: Ctrl, Shift, Alt or Meta";
          qDebug() << "New KeySequence:" << QKeySequence(keyInt).toString(QKeySequence::NativeText);
          return QWidget::eventFilter(obj, event);
      }

      // check for a combination of user clicks
      Qt::KeyboardModifiers modifiers = keyEvent->modifiers();
      QString keyText = keyEvent->text();
      // if the keyText is empty than it's a special key like F1, F5, ...
      qDebug() << "Pressed Key:" << keyText;

      QList<Qt::Key> modifiersList;
      if(modifiers & Qt::ShiftModifier)
          keyInt += Qt::SHIFT;
      if(modifiers & Qt::ControlModifier)
          keyInt += Qt::CTRL;
      if(modifiers & Qt::AltModifier)
          keyInt += Qt::ALT;
      if(modifiers & Qt::MetaModifier)
          keyInt += Qt::META;
      if (keyInt==QKeySequence::Save){
        on_save_button_clicked();
        return QWidget::eventFilter(obj, event);
      }
      qDebug() << "New KeySequence:" << QKeySequence(keyInt).toString(QKeySequence::NativeText);
  }
  return QWidget::eventFilter(obj, event);
}
void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   sizer();
}

void MainWindow::writeToConsole(QString content){
    ui->console->append(content);
}

bool MainWindow::compileMain(){
    std::string command = "cd "+projPath+" && g++ -o runner runner.cpp GE.o Maker.o statTracker.o -lboost_system -lboost_filesystem";
    std::string res = _exec(command.c_str());

    if(res[0]=='*'){
        writeToConsole(QString(res.c_str()));
        return false;
    }
    return true;
}
void *MainWindow::runMain(){
    std::string command = "cd "+projPath+" && ./runner";
    std::string res = _exec(command.c_str(), true);
    writeToConsole("Unexpected error: "+QString(res.c_str()));
    pthread_exit(NULL);
}

std::string _exec(const char* cmd){
    return _exec(cmd, false);
}

std::string _exec(const char* cmd, bool stderr) {
    if(stderr){
        std::string buf(cmd);
        buf.append("");
        buf = "2>&1 "+buf;
        cmd = buf.c_str();
    }
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    char *getResult;
    while(!feof(pipe)) {
        if((getResult=fgets(buffer, 128, pipe)) != NULL)
            result += buffer;
        //if (ferror (pipe))
        //    return "* An error occured";
    }
    pclose(pipe);
    return result;
}

void MainWindow::on_open_button_clicked()
{
    OpenProject *open = new OpenProject(this);
    open->open();
}

void MainWindow::on_save_button_clicked()
{
    writeToConsole("Saving");
    QDir dir(QString((projPath+"/orgs/").c_str()));
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    std::ofstream mainCpp;
    mainCpp.open( (projPath+"/orgs/main.cpp").c_str() );
    mainCpp << ui->main_code->toPlainText().toStdString();
    mainCpp.close();

    mainCpp.open( (projPath+"/runner.cpp").c_str() );
    mainCpp << ui->test_code->toPlainText().toStdString();
    mainCpp.close();
}

void copy(std::string output, std::string input){
    QString qinput = QString(input.c_str());
    QFile localHeader(qinput);
    if (localHeader.exists())
        localHeader.remove();
    QFile header(QString(output.c_str()));
    header.copy(qinput);
}

void MainWindow::on_run_button_clicked()
{

    on_save_button_clicked();

    copy("://Maker.o", projPath+"/Maker.o");
    copy("://GE.o", projPath+"/GE.o");
    copy("://statTracker.o", projPath+"/statTracker.o");
    copy("://GE.h", projPath+"/GE.h");
    copy("://statTracker.h",projPath+"/statTracker.h");

    writeToConsole("Compiling...");
    bool success = compileMain();
    if (success){
        writeToConsole("Compiled Successfully!\nRunning "+QString(projName.c_str())+"...");
        pthread_t tid;
        pthread_create(&tid,NULL, &MainWindow::runHelper, this);
        //runMain();
        runDialog *runningWindow = new runDialog();
        runningWindow->open();
    }
}
