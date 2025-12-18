#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <cstdio>
#include <fstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    checkServiceStatus(); // Проверяем статус сервиса при запуске
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_start_clicked()
{
    system("doas service ipfw start");
    checkServiceStatus();
}

void MainWindow::on_pushButton_stop_clicked()
{
    system("doas service ipfw stop");
    checkServiceStatus();
}

void MainWindow::checkServiceStatus()
{
    // Выполняем команду и сохраняем её вывод в файл
    system("doas service ipfw status > /tmp/ipfw_status_output.txt 2>&1");

    // Читаем содержимое файла
    std::ifstream file("/tmp/ipfw_status_output.txt");
    if (!file.is_open()) {
        qDebug() << "Не удалось открыть файл для чтения";
        return;
    }

    std::string line;
    std::string output;

    // Читаем все строки из файла
    while (std::getline(file, line)) {
        output += line + "\n";
    }

    file.close();

    // Выводим результат в консоль для отладки
    qDebug() << "Output: " << QString::fromStdString(output);

    // Проверяем, содержится ли в выводе строка "ipfw is enabled"
    if (QString::fromStdString(output).contains("ipfw is enabled", Qt::CaseInsensitive)) {
        ui->checkBox->setChecked(true);  // Если сервис включен, отмечаем чекбокс
    } else {
        ui->checkBox->setChecked(false); // Иначе, снимаем отметку с чекбокса
    }
}

