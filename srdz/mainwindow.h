#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dbpolaczenie.h"
#include "QMessageBox"
#include "QSqlQueryModel"
#include "logowanie.h"
#include <QUdpSocket>
#include <QDebug>
#include <QStringList>

namespace Ui {
class MainWindow;
}

class QSqlQueryModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void logincheck();
    void permitscheck(QString permition);
    void showselectintable(QString query);
    void checkforalert();
    int checkloginid();
    QString checkrecord(QString query,int row);/////////////////////////////
    bool isEmptySelect(QString query);//////////////////////////////////////

private slots:
    void on_actionPo_cz_z_baz_danych_triggered();
    void on_actionZako_cz_triggered();
    void on_actionO_Qt_triggered();
    void on_actionLogowanie_triggered();
    void autostart();

    void on_Wyszukaj_clicked();

    void on_pushButton_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_4_clicked();

    void readPendingDatagrams();



private:
    Ui::MainWindow *ui;
    DBPolaczenie mDBPolaczenie;
    QSqlQueryModel *mModel;
    QString passwd = "";
    QString login = "";
    bool logged = false;
    bool edit = false;
    bool preview = false;
    bool add = false;
    QHostAddress adres = QHostAddress("192.168.1.3");//komp - 4
    QHostAddress adressender = QHostAddress("192.168.1.3");//xiaomi -3
    quint16 port = 30001;
    QUdpSocket *m_socket;
    void init();
    void initNetwork();
    QStringList m_spam;
    void sendnumber(int i, QHostAddress adresss);
    void sendstring(QString str,QHostAddress adresss);
    int addplace(QString wspol);
    int clientlogin(QString login,QString password);
    int addevent(QString NazwaZdarzenia,int IDTypu,QString czas,int IDMiejsca,int IDLogin);
    int addtype(QString nazwa);
    QString readtype();
};

#endif // MAINWINDOW_H
