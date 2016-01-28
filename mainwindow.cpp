#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHostAddress>
#include <QDebug>
#include <QFile>
#include <QByteArray>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    qDebug() << "socket created";

    connect(socket, SIGNAL(connected()), this, SLOT(socketConnected()));
    socket->connectToHost(QHostAddress::LocalHost, qint16(1337), QIODevice::ReadWrite);
}

MainWindow::~MainWindow()
{
    delete ui;
    socket->close();
    delete socket;
}

void MainWindow::socketConnected()
{
    qDebug() << "socked connected";

    connect(socket, SIGNAL(readyRead()), this, SLOT(dataAvalaible()));

    //socket->write("some chars");
    QFile file("test.xml");
    if (!file.open(QIODevice::ReadOnly)) qDebug() << "Error - file did not open";
    QByteArray bytes = file.readAll();
    socket->write(bytes);
    socket->flush();
    qDebug() << "data send";
}

void MainWindow::dataAvalaible()
{
    qint64 size = socket->bytesAvailable();
    qDebug() << size;
    if (size <=0 || size >=60000)
    {
        qDebug() << QString("Bad size == " + size);
    }
    QByteArray arr = socket->read(size);
    QString receivedData = QString(arr);
    qDebug() << "receivedData: " << receivedData;
}
