#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "statemachine.h"

#include <QMessageBox>


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

    ui->plainTextEdit->clear();
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

void MainWindow::on_lexemButton_clicked()
{
    ui->tableWidget->clear();
    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Лексема"));
    ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Значение"));
    ui->tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("Строка"));
    ui->tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("Позиция"));
    StateMachine *state_machine = new StateMachine();
    QObject::connect(state_machine, &StateMachine::errorSignal,
                     this,&MainWindow::errorMessage);
    state_machine->work(ui->plainTextEdit->toPlainText());
    auto lexems = state_machine->getLexemList();
    ui->tableWidget->setRowCount(lexems.length());
    int i = 0;
    for (auto it:lexems)
    {
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(QString(it.name.c_str())));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(QString(it.type.c_str())));
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(QString::number(it.line)));
        ui->tableWidget->setItem(i,3,new QTableWidgetItem(QString::number(it.pos)));
        ui->tableWidget->resizeRowToContents(i);
        i++;
    }
    ui->tableWidget->resizeColumnsToContents();
    delete state_machine;
}

void MainWindow::errorMessage(QString str)
{
    QMessageBox msgBox;
    msgBox.setText(str);
    msgBox.exec();
}
