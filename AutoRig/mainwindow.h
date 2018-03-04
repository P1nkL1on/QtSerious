#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QKeyEvent"

#include "testautorig.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    // D:/QT-projects/QT-projects/Prohor/Models/
    QString modelsAdress = "D:/QT-serious/Models/";///"/home/daiver/coding/QT-serious/Models/";  ///home/daiver/coding/QT-serious/Models

    void keyPressEvent(QKeyEvent *e);

 // controls
    void mouseMoveEvent(QMouseEvent *m);
    void mousePressEvent(QMouseEvent *m);
    void mouseReleaseEvent(QMouseEvent *m);


protected:
    void paintEvent(QPaintEvent *e);
};

#endif // MAINWINDOW_H
