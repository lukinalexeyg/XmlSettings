#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle(QApplication::applicationName());
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    helpViewer = new HelpViewer(QApplication::applicationDirPath() + "/help/ural_ru.qhc", this);
    helpViewer->setWindowTitle(QApplication::applicationName());
    helpViewer->setHomeSource("qthelp://ural/doc/about.html");

    hBoxLayout = new QHBoxLayout;

    addPushButton("Empty", [this](){helpViewer->show();});
//    addPushButton("Constructor", [this](){helpViewer->show("qthelp://ural/doc/constructor.html");});
    addPushButton("Constructor", [this](){helpViewer->show("https://ru.wikipedia.org/wiki/HTMLHelp");});
    addPushButton("Pattern", [this](){helpViewer->show("qthelp://ural/doc/pattern.html");});

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(hBoxLayout);
    setCentralWidget(centralWidget);
}



void MainWindow::closeEvent(QCloseEvent *)
{
    helpViewer->close();
}



template<typename Func>
void MainWindow::addPushButton(const QString &name, Func slot)
{
    QPushButton *pushButton = new QPushButton(name, this);

    pushButton->setFixedSize(120, 40);
    pushButton->setStyleSheet("font-size: 12pt");

    hBoxLayout->addWidget(pushButton);

    connect(pushButton, &QPushButton::released, this, slot);
}
