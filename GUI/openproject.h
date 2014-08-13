#ifndef OPENPROJECT_H
#define OPENPROJECT_H

#include <QDialog>

namespace Ui {
class OpenProject;
}

class OpenProject : public QDialog
{
    Q_OBJECT

public:
    explicit OpenProject(QWidget *parent = 0, void (*_close)(std::string, std::string)=NULL);
    ~OpenProject();

private slots:
    void on_buttonBox_accepted();

    void on_listView_2_clicked(const QModelIndex &index);

    void on_listView_2_doubleClicked(const QModelIndex &index);

private:
    Ui::OpenProject *ui;
};

#endif // OPENPROJECT_H
