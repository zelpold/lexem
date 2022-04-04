#ifndef MAINWINDOW_H

#define MAINWINDOW_H


#include <QList>
#include <QMainWindow>
#include <QFileDialog>
#include "lexem.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void errorMessage(QString str);
private slots:
    void on_openFileButton_clicked();

    void on_lexemButton_clicked();

private:
    QString path;
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
