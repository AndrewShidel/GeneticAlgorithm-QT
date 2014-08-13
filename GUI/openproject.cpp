#include "openproject.h"
#include "ui_openproject.h"
#include <QStandardItemModel>
#include <QStandardItem>
#include <fstream>
#include <vector>
//#include <map>

void (*onClose2)(std::string, std::string);
int selected=0;
std::vector<std::pair<std::string, std::string>> project;

OpenProject::OpenProject(QWidget *parent, void (*_close)(std::string, std::string)) :
    QDialog(parent),
    ui(new Ui::OpenProject)
{
    ui->setupUi(this);


    QStandardItemModel *stdModel = new QStandardItemModel(this);

    std::ifstream projects;
    projects.open("projects", std::ifstream::in);

    char c = projects.get();
    std::string projectName = "";
    projectName+=c;
    std::string temp;
    while (projects.good()) {
        c = projects.get();
        if (c==' '){
            temp = projectName;
            stdModel->appendRow(new QStandardItem(QString(projectName.c_str())));
            projectName = "";
        }else if (c=='\n'){
            project.push_back(std::pair<std::string, std::string>(temp,projectName));
            projectName = "";
        }else{
            projectName+=c;
        }
    }


    //stdModel->appendRow(new QStandardItem("New item"));
    ui->listView_2->setModel(stdModel);

    onClose2 = _close;
}

OpenProject::~OpenProject()
{
    delete ui;
}

void OpenProject::on_buttonBox_accepted()
{
    //int row = ui->listView_2->currentIndex().QModelIndex.row();
    int row2 = selected;

    onClose2(project[row2].first, project[row2].second);
    //close()

}

void OpenProject::on_listView_2_clicked(const QModelIndex &index)
{
    selected = index.row();
}

void OpenProject::on_listView_2_doubleClicked(const QModelIndex &index)
{
    selected = index.row();
    this->close();
    on_buttonBox_accepted();

}
