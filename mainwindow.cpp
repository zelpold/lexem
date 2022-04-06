#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "statemachine.h"

#include <QMessageBox>
#include "Synt.h"


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
    Synt * synt = new Synt();
    synt->Work(lexems);
    QMap<int,QString> rules = synt->back_rules;
    ui->treeWidget->clear();
    QTreeWidgetItem *topLevelItem=new QTreeWidgetItem(ui->treeWidget);
    ui->treeWidget->addTopLevelItem(topLevelItem);
    topLevelItem->setText(0,"S");
    QList<int> operations = synt->getOperations();
    operations_it = operations.begin();
    getTree(rules,operations,topLevelItem);
    delete synt;
}

void MainWindow::getTree(QMap<int, QString> rules, QList<int> operations,   QTreeWidgetItem *item)
{
    if (operations_it!=operations.end()){

        QString str = rules[*operations_it];
        QStringList lexems = str.split(' ');
        QList<QTreeWidgetItem *> items;
        for (auto i: lexems){
            QTreeWidgetItem *new_item=new QTreeWidgetItem(item);
            //        // укажем текст итема
            new_item->setText(0,i);
            if (i == "S"){
                items.push_front(new_item);
            }
        }
        for (auto i: items){
            operations_it++;
            getTree(rules,operations,i);
        }
    }
    // создаем новый итем (пусть сначала базовый)
//        QTreeWidgetItem *topLevelItem=new QTreeWidgetItem(ui->treeWidget);
//        // вешаем его на наше дерево в качестве топ узла.
//        ui->treeWidget->addTopLevelItem(topLevelItem);
//        // укажем текст итема
//        topLevelItem->setText(0,"S");
//        // создаем новый итем и сразу вешаем его на наш базовый
//        QTreeWidgetItem *item=new QTreeWidgetItem(topLevelItem);
//        // укажем текст итема
//        item->setText(0,"Под итем");


}

void MainWindow::errorMessage(QString str)
{
    QMessageBox msgBox;
    msgBox.setText(str);
    msgBox.exec();
}
