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

private slots:
    void on_actionLoad_Rig_triggered();
    void on_actionLoad_Guad_triggered();

    void on_actionLoad_Ivan_Sergeich_triggered();

private:
    Ui::MainWindow *ui;
    void loadRigByAdress (QString path);
    void loadRigGuardYelling ();
    void loadRigIvanSergeich ();
    void runTests();
};

#endif // MAINWINDOW_H
