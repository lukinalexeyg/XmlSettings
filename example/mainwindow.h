#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once

#include <QMainWindow>
#include <QCloseEvent>
#include <QHBoxLayout>
#include "helpviewer.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void closeEvent(QCloseEvent *);

private:
    HelpViewer *helpViewer;
    QHBoxLayout *hBoxLayout;

    template<typename Func>
    void addPushButton(const QString &name, Func slot);
};

#endif // MAINWINDOW_H
