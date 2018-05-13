#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    serial = new QSerialPort(this);
	//Konfiguracja portu szeregowego. Dla Windowsa inny portname
	//Default Baud9600, 8bit danych, bez parzystosci,1 bit stopu
    serial->setPortName("/dev/ttyACM0");
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    serial->open(QIODevice::ReadWrite);

    if(serial->isOpen())
    {
        qDebug()<<"POLACZONO";
    }
    qDebug()<<serial->isOpen();
    qDebug()<<serial->isReadable();

    connect(serial, SIGNAL(readyRead()), this, SLOT(readFromSerial()));

    ui->setupUi(this);
}
void MainWindow::readFromSerial()
{
    QByteArray frame;
    frame = serial->readAll();
    QString info = frame;
    qDebug()<<info;

	
    bufor.append(frame);
    if(bufor.contains("s"))
    {
        if(bufor.contains("e"))
        {
            int p1 = bufor.indexOf("s");
            int p2 = bufor.indexOf("e");
            if(p2>p1)
            {
                //get EMG data from A0, A1
				//dane rozdzielane " ; "
                QString b2 = bufor.mid(p1+2,(p2-p1-3));
                QStringList l = b2.split(";");
                qDebug()<<"A0 = "<<l[0];
                qDebug()<<"A1 = "<<l[1];
                ui->textEdit->append(l[0]);
                QString val = l[0];

                if(val.toInt()>300)
                {
                    ui->textEdit_2->append("MORĄG RULEZZ!!!");
                }
                else
                {
                    ui->textEdit_2->append("A Jednak NIE :))");
                }

                bufor.remove(0,p2+1);
                qDebug()<<b2;
            }
            else
            {
                bufor.clear();
                qDebug()<<bufor;
            }
        }
    }
 }

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ui->textEdit->append(lineA0);
    ui->textEdit_2->append(lineA1);
    lineA0.clear();
    lineA1.clear();
}
