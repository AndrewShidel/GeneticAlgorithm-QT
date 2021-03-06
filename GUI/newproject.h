#ifndef NEWPROJECT_H
#define NEWPROJECT_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class newProject;
}

class newProject : public QDialog
{
    Q_OBJECT

public:
    explicit newProject(QWidget *parent = 0);
    ~newProject();

private slots:
    void on_create_button_clicked();

    void on_browse_button_clicked();

    void on_cancel_button_clicked();

private:
    Ui::newProject *ui;
};

#endif // NEWPROJECT_H
