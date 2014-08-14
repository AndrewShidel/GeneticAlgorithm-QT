#include <iostream>
#include <fstream>
#include <QFile>
#include <QDebug>
#include <QString>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "GE.h"
#include "newproject.h"
#include "openproject.h"

std::string _exec(const char* cmd);
void onNewClose(std::string,std::string);
std::string projName;
std::string projPath;
int test = 3;
std::string libPath = "";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    std::vector<std::string> questions;
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
    newProject *wdg = new newProject(this, &onNewClose);
    wdg->show();
}

void onNewClose(std::string pName,std::string pPath){
    projName = pName;
    projPath = pPath;
}

void MainWindow::writeToConsole(QString content){
    ui->console->append(content);
}

bool MainWindow::compileMain(){
    std::string command = "cd "+projPath+" && g++ -o runner runner.cpp GE.o Maker.o -lboost_system -lboost_filesystem";
    std::string res = _exec(command.c_str());

    if(res[0]=='*'){
        writeToConsole(QString(res.c_str()));
        return false;
    }
    return true;
}
void runMain(){
    std::string command = "cd "+projPath+" && ./runner";
    _exec(command.c_str());
}


std::string _exec(const char* cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    char *getResult;
    while(!feof(pipe)) {
        if((getResult=fgets(buffer, 128, pipe)) != NULL)
            result += buffer;
        if (ferror (pipe))
            return "* An error occured";
    }
    pclose(pipe);
    return result;
}

void MainWindow::on_open_button_clicked()
{
    OpenProject *open = new OpenProject(this, &onNewClose);
    open->open();
}

void MainWindow::on_save_button_clicked()
{
    std::ofstream mainCpp;
    mainCpp.open(projPath+"/main.cpp");
    mainCpp << ui->main_code->toPlainText().toStdString();
    mainCpp.close();

    mainCpp.open(projPath+"/runner.cpp");
    mainCpp << ui->test_code->toPlainText().toStdString();
    mainCpp.close();
}

void MainWindow::on_run_button_clicked()
{
    writeToConsole("Writting files...");
    on_save_button_clicked();
    QFile localMaker(QString((projPath+"/Maker.o").c_str()));
    if (localMaker.exists())
        localMaker.remove();
    QFile maker("://Maker.o");
    maker.remove();
    maker.copy(QString((projPath+"/Maker.o").c_str()));


    QFile localLib(QString((projPath+"/GE.o").c_str()));
    if (localLib.exists())
        localLib.remove();
    QFile lib("://GE.o");
    lib.copy(QString((projPath+"/GE.o").c_str()));

    QFile localHeader(QString((projPath+"/GE.h").c_str()));
    if (localHeader.exists())
        localHeader.remove();
    QFile header("://GE.h");
    header.copy(QString((projPath+"/GE.h").c_str()));

    writeToConsole("Compiling...");
    bool success = compileMain();
    if (success){
        writeToConsole("Compiled Successfully!\nRunning "+QString(projName.c_str())+"...");
        runMain();
    }
}
