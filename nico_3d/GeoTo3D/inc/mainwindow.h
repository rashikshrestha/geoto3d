#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ncqtmainwindow.h"
#include "ncapp.h"

class MainWindow : public ncQtMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

private:
    ncApp*			    glapp;

};

#endif // MAINWINDOW_H
