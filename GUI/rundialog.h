#ifndef RUNDIALOG_H
#define RUNDIALOG_H

#include <QDialog>

namespace Ui {
class runDialog;
}

class runDialog : public QDialog
{
    Q_OBJECT

public:
    explicit runDialog(QWidget *parent = 0);
    ~runDialog();

private:
    Ui::runDialog *ui;
};

#endif // RUNDIALOG_H
