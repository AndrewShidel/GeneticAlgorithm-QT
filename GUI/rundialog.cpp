#include "rundialog.h"
#include "ui_rundialog.h"

runDialog::runDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::runDialog)
{
    ui->setupUi(this);
}

runDialog::~runDialog()
{
    delete ui;
}
