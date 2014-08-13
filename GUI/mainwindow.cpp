#include <iostream>
#include <fstream>

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
std::string blackBox(std::string question){
    return "";
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    std::vector<std::string> questions;
    ui->textEdit_3->toPlainText();
    GE::GE ge("../UserSpace/", &blackBox, questions);
    ge.stop();
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

void MainWindow::compileMain(){
    /*std::ofstream main;
    main.open (*projectPath);

    main << ui->textEdit_3->toPlainText().toStdString();
    main.close();
    _exec(("g++ "+*projectPath).c_str());*/
}

std::string _exec(const char* cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
        if(fgets(buffer, 128, pipe) != NULL)
            result += buffer;
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
    //TODO - Write cpp files to the projPath.
}
