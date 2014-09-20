#include "newproject.h"
#include "mainwindow.h"
#include "ui_newproject.h"
#include "GE.h"
#include <stdlib.h>
#include <QFileDialog>
#include <fstream>

std::string projectName;
std::string projectPath;
std::string pathWithoutProjectName;
void (*onClose)(std::string, std::string,bool);


newProject::newProject(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newProject)
{
    ui->setupUi(this);
    //onClose=_close;

}

newProject::~newProject()
{
    delete ui;
}

void newProject::on_create_button_clicked()
{

    projectName = ui->lineEdit->text().toStdString();
    projectPath = pathWithoutProjectName+"/"+projectName;
    mkdir(projectPath.c_str(), 0777);
    //onClose(projectName, projectPath,false;);

    MainWindow *w=static_cast<MainWindow*>(this->parentWidget());
    //w->onNewClose(projectName, projectPath,false);

    //((MainWindow *)this->parent())->onNewClose(projectName, projectPath,false);

    std::ofstream logfile;
    logfile.open ("projects", std::fstream::app);

    logfile << projectName << " " << projectPath << "\n";
    logfile.close();

    this->hide();
}

void newProject::on_browse_button_clicked()
{
    QUrl dir = QFileDialog::getExistingDirectoryUrl(this,"Select the workspace");
    QString p = dir.path();
    pathWithoutProjectName = dir.path().toStdString();
    ui->lineEdit_2->setText(p);
}

void newProject::on_cancel_button_clicked()
{
    this->close();
}
