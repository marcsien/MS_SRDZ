#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QString"
#include "ctime"
#include "logowanie.h"
#include "qmessagebox.h"
#include "zdarzenie.h"
#include <QDateTime>
#include <QDebug>
#include <QInputDialog>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QThread>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label->setAlignment(Qt::AlignCenter);
    init();
    initNetwork();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonLokalizuj_clicked()
{

    if(logged)
    {
        QString miej = QString("%1.%2 S, %3.%4 E, %5.%6m")
                .arg(( std::rand() % 100 ) + 10)
                .arg(( std::rand() % 8888 ) + 1111)
                .arg(( std::rand() % 100 ) + 10)
                .arg(( std::rand() % 8888 ) + 1111)
                .arg(( std::rand() % 99 ))
                .arg(( std::rand() % 100 ) + 10);

        ui->label->setText(miej);
        QString now = QString("MIEJSCE/%1").arg(miej);
        int res = sendfast(now).toInt();
        IDMiejsca = res;

    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Należy się zalogować !");
        msgBox.setWindowTitle("Błąd");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
    }
}

void MainWindow::on_pushButton_clicked()
{
    //close();
    QString lo = sendfast("T");
}

void MainWindow::on_pushButtonZaloguj_clicked()
{

    if(connected)
    {
        Logowanie *dlglogin = new Logowanie(this);
        if(dlglogin->exec() == QDialog::Accepted)
        {
            login = dlglogin->getUser();
            haslo = dlglogin->getPasswd();
            int id = logincheck();
            if (id)
            {
                IDLogin=id;
                logged = true;
                QMessageBox msgBox;
                msgBox.setText("Zalogowano !");
                msgBox.setWindowTitle("Informacja");
                msgBox.setIcon(QMessageBox::Information);
                msgBox.exec();
            }
            else if ( id == 0)
            {
               logged = false;
               IDLogin = 0;
               QMessageBox msgBox;
               msgBox.setText("Niepoprawne dane logowania !");
               msgBox.setWindowTitle("Błąd");
               msgBox.setIcon(QMessageBox::Warning);
               msgBox.exec();
            }
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setText("Anulowano !");
            msgBox.setWindowTitle("Błąd");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
        }

        delete dlglogin;

    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Należy się najpierw połączyć z serwerem !");
        msgBox.setWindowTitle("Błąd");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
    }
}

int MainWindow::logincheck()
{
        QString log = QString("LOGIN/%1/%2").arg(login).arg(haslo);
        QString odb = sendfast(log);
        int returnvalue = odb.toInt();
        return returnvalue;
}


void MainWindow::dodajzdarzenie(QString NazwaZdarzenia,int IDTypu,QString CzasZdarzenia,int IDMiejsca,int IDUzytkownika)
{
    QString zdarz = QString("ZDARZ/%1/%2/%3/%4/%5")
            .arg(NazwaZdarzenia)
            .arg(IDTypu)
            .arg(CzasZdarzenia)
            .arg(IDMiejsca)
            .arg(IDUzytkownika);
    QString ans = sendfast(zdarz);
}

void MainWindow::on_pushButtonZdarzenie_clicked()
{
    if(logged)
    {
        on_pushButtonLokalizuj_clicked();
        int IDTypu = 0;
        QString NazwaZdarzenia = "";
        Zdarzenie *dlgzdarz = new Zdarzenie(this);
        addtypes(dlgzdarz);
        QDateTime datetime(QDateTime::currentDateTime());
        QString czas = datetime.toString("yyyy-MM-dd HH:mm:ss");
        if(dlgzdarz->exec() == QDialog::Accepted)
        {
            IDTypu = dlgzdarz->getType() + 1;
            NazwaZdarzenia = dlgzdarz->getName();
            dodajzdarzenie(NazwaZdarzenia,IDTypu,czas,IDMiejsca,IDLogin);
        }
        else {
            QMessageBox msgBox;
            msgBox.setText("Anulowano !");
            msgBox.setWindowTitle("Błąd");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
        }
        delete dlgzdarz;



    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Należy się zalogować !");
        msgBox.setWindowTitle("Błąd");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
    }
}

QString MainWindow::readPendingDatagrams()
{


//        QByteArray datagram1;
//        //datagram1.resize(m_socket->pendingDatagramSize());
//        qDebug()<<m_socket->pendingDatagramSize();
//        if (m_socket->pendingDatagramSize()==1)
//        {
//            on_pushButtonIP_clicked();
//            if (connected == true)
//            {
//                int delay = 50;
//                logged = true;
//                connected = true;
//                m_socket->readDatagram(datagram1.data(), datagram1.size(),
//                                        &adres, &port);
//                QString qstring = QString::fromStdString(datagram1.toStdString());
//                QStringList pieces = qstring.split("/");
//                if (pieces.value(0) == "TEST")
//                {
//                    delay = pieces.value(1).toInt();
//                    for (int i =1 ; i<=100 ;i++)
//                    {
//                        on_pushButtonLokalizuj_clicked();
//                        int IDTypu = std::rand() % 5 + 1;
//                        QString NazwaZdarzenia = QString("ZdarzenieTestowe_%1").arg(i);
//                        QDateTime datetime(QDateTime::currentDateTime());
//                        QString czas = datetime.toString("yyyy-MM-dd HH:mm:ss");
//                        dodajzdarzenie(NazwaZdarzenia,IDTypu,czas,IDMiejsca,IDLogin);
//                        QThread::msleep(delay);
//                    }
//                }
//            }
//            else
//            {
//                QMessageBox msgBox;
//                msgBox.setText("nie połączono test nieudany");
//                msgBox.setWindowTitle("Nie połączono");
//                msgBox.setIcon(QMessageBox::Warning);
//                msgBox.exec();
//            }
//        }



    if (m_socket->waitForReadyRead(50))
    {
        QByteArray datagram1;
        datagram1.resize(m_socket->pendingDatagramSize());
        m_socket->readDatagram(datagram1.data(), datagram1.size(),
                                &adres, &port);
        QString qstring = QString::fromStdString(datagram1.toStdString());

        if(qstring == "T")
        {
            QFile file("C:/Users/ms/Desktop/test.txt");
            QFile file1("C:/Users/ms/Desktop/test1.txt");
            if(file.open(QIODevice::WriteOnly | QIODevice::Text) &&file1.open(QIODevice::WriteOnly | QIODevice::Text))
            {
            QTextStream out(&file);
            QTextStream out1(&file1);

            for (int i =1 ; i<=1000 ;i++)
            {
            on_pushButtonLokalizuj_clicked();
            int IDTypu = std::rand() % 5 + 1;
            QString NazwaZdarzenia = QString("ZdarzenieTestowe_%1").arg(i);
            QDateTime datetime(QDateTime::currentDateTime());
            QString czas = datetime.toString("yyyy-MM-dd HH:mm:ss");
            qint64 timex = datetime.currentMSecsSinceEpoch();
            out << timex << endl;
            dodajzdarzenie(NazwaZdarzenia,IDTypu,czas,IDMiejsca,IDLogin);
            QDateTime datetime1(QDateTime::currentDateTime());
            qint64 timex1 = datetime.currentMSecsSinceEpoch();
            out1 <<timex1<<endl;







            }
            file.close();
            file1.close();
            return "";
            }

        }
        else
        {
            qDebug()<<qstring;
            return qstring;
        }
    }
    else
    {
        return "uplynalczas";
    }

}

void MainWindow::init()
{
    m_socket=NULL;
    m_spam << "10.1.16.49"
           << "10.1.16.40";
}

void MainWindow::initNetwork()
{
    if (m_socket) {
        disconnect (m_socket,0,0,0);
        delete m_socket;
        m_socket;

    }
    m_socket=new QUdpSocket(this);

//    connect(m_socket,SIGNAL(readyRead()),
//            this, SLOT(readPendingDatagrams()));
    if (m_socket->bind(QHostAddress::Any,
                       port)){
        qWarning()<<"Błąd bindowania na porcie:"<<port;

    }
    else {
        connect(m_socket,SIGNAL(readyRead()),
                this,SLOT(readPendingDatagrams()));
    }

}


QString MainWindow::sendfast(QString message)
{
    if(m_socket)
    {
        if (m_socket->isReadable())
        {
            QByteArray datagram99;
            m_socket->readDatagram(datagram99.data(), datagram99.size(),
                                   &adres, &port);
        }

        int cnt = m_socket->writeDatagram(
                    message.toLatin1(),
                    adres,
                    port);


        return readPendingDatagrams();
//        if (m_socket->waitForReadyRead(50))
//        {
//            QByteArray datagram1;
//            datagram1.resize(m_socket->pendingDatagramSize());
//            m_socket->readDatagram(datagram1.data(), datagram1.size(),
//                                    &adres, &port);
//            QString qstring = QString::fromStdString(datagram1.toStdString());

//            return qstring;
//        }
//        else {
//            return "";
//        }
    }
}

void MainWindow::addtypes(Zdarzenie *dlgzdarz)
{
    QString typy = sendfast("READTYP");
    QStringList pieces = typy.split("/");
    for (int t = 0;t<pieces.count();t++)
    {
        dlgzdarz->puttype(pieces.value(t));
    }
}

void MainWindow::on_pushButtonIP_clicked()
{
    bool ok;
    QHostAddress address;
    QString hostname = "ms-Komputer";
    QString hostIP;
    QList<QHostAddress> hostList = QHostInfo::fromName(hostname).addresses();
    foreach (const QHostAddress& address, hostList) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address.isLoopback() == false) {
             hostIP = address.toString();
        }
    }

    if(!hostIP.isNull())
    {
        adres.setAddress(hostIP);
        if(sendfast("CON")=="OK")
        {
            connected = true;
            QMessageBox msgBox;
            msgBox.setText("Połączono !");
            msgBox.setWindowTitle("Połączono");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
        }
    }
    else
    {
        for (int i = 1;i<=255;i++)
        {
            adres.setAddress(QString("192.168.1.%1").arg(i));

            if(sendfast("CON")=="OK")
            {
                connected = true;
                QMessageBox msgBox;
                msgBox.setText("Połączono !");
                msgBox.setWindowTitle("Połączono");
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.exec();
                break;
            }
        }


        if(connected==false)
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Brak połączenia", "Brak połączenia, czy chcesz wprowadzić adres ręcznie?",
                                          QMessageBox::Yes|QMessageBox::No);
            if (reply == QMessageBox::Yes)
            {
                QString text = QInputDialog::getText(this,"Podaj adres IP","Adres IP Serwera:",QLineEdit::Normal,"192.168.1.1",&ok);
                if (ok && !text.isEmpty())
                    adres.setAddress(text);
                if(sendfast("CON")=="OK")
                {
                    connected = true;
                    QMessageBox msgBox;
                    msgBox.setText("Połączono !");
                    msgBox.setWindowTitle("Połączono");
                    msgBox.setIcon(QMessageBox::Warning);
                    msgBox.exec();
                }
                else
                {
                    QMessageBox msgBox;
                    msgBox.setText("Błąd połączenia");
                    msgBox.setWindowTitle("Błąd połączenia");
                    msgBox.setIcon(QMessageBox::Warning);
                    msgBox.exec();
                }
            }
        }

    }
}

void MainWindow::on_pushButton_2_clicked()
{
    if(logged)
    {
        bool ok;
        QString text = QInputDialog::getText(this,"Podaj nazwę typu",
                                             tr("Nazwa typu:"), QLineEdit::Normal,"Tutaj wpisz nową nazwę", &ok);
        if (ok && !text.isEmpty())
        {
            sendfast("ADDTYP/"+text);
        }
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Należy się zalogować !");
        msgBox.setWindowTitle("Błąd");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
    }
}
