#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    /*void onNewClose(std::string,std::string, bool);*/
    void *runMain();
    static void *runHelper(void *classRef)
    {
        return ((MainWindow *)classRef)->runMain();
    }

private slots:
    void on_toolButton_clicked();

    void on_open_button_clicked();

    void on_save_button_clicked();

    void on_run_button_clicked();

private:
    Ui::MainWindow *ui;
    bool compileMain();
    void writeToConsole(QString content);
    void sizer();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void resizeEvent(QResizeEvent* event);

};

#endif // MAINWINDOW_H
