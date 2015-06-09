#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

void MainWindow::addQwtPlot(QwtPlot* plot)
{
	ui->plotLayout->addWidget(plot);
}


MainWindow::~MainWindow()
{
    delete ui;
}
