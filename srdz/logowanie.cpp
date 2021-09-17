#include "logowanie.h"
#include "ui_logowanie.h"
#include "QMessageBox"


Logowanie::Logowanie(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Logowanie)
{
    ui->setupUi(this);
}

Logowanie::~Logowanie()
{
    delete ui;
}


void Logowanie::cleartext()
{
    ui->lineEditLogin->clear();
    ui->lineEditPassword->clear();
}

QString Logowanie::getUser()
{
    return userlogin;
}

QString Logowanie::getPassword()
{
    return userpassword;
}

void Logowanie::on_pushButtonAnuluj_clicked()
{
    this->close();
}

void Logowanie::on_pushButtonZaloguj_clicked()
{
    this->userlogin = ui->lineEditLogin->text();
    this->userpassword = ui->lineEditPassword->text();
    //tutaj wywolanie metody mainwindow i dopiero close ??
    this->close();
}
