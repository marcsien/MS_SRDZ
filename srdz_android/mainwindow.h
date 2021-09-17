#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QUdpSocket>
#include <QDebug>
#include <QStringList>
#include <zdarzenie.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonLokalizuj_clicked();

    void on_pushButton_clicked();

    void on_pushButtonZaloguj_clicked();

    void on_pushButtonZdarzenie_clicked();

    QString readPendingDatagrams();

    void on_pushButtonIP_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QString login = "";
    QString haslo = "";
    bool logged = false;
    int IDLogin = 1;
    int logincheck();
    void dodajzdarzenie(QString NazwaZdarzenia,int IDTypu,QString CzasZdarzenia,int IDMiejsca,int IDUzytkownika);
    QHostAddress adres;
    quint16 port = 30001;
    QUdpSocket *m_socket;
    void init();
    void initNetwork();
    QStringList m_spam;
    QString sendfast(QString message);
    int IDMiejsca;
    bool connected = false;
    void addtypes(Zdarzenie *dlgzdarz);

};

#endif // MAINWINDOW_H
