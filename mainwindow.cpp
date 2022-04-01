#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_openFileButton_clicked()
{
    path = QFileDialog::getOpenFileName();
        QFile file(path);
        QList<QString> path_words  = path.split("/");
        QString name = path_words[path_words.length()-1];
        ui->lineEdit->setText(name);
        if (file.open(QIODevice::ReadOnly))
        {
            ui->plainTextEdit->appendPlainText(file.readAll());
            file.close();
        }

}
