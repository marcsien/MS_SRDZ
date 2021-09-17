#include "logowanie.h"
#include "ui_logowanie.h"

Logowanie::Logowanie(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Logowanie)
{
    ui->setupUi(this);
    ui->label->setAlignment(Qt::AlignCenter);
    ui->label_2->setAlignment(Qt::AlignCenter);
    setWindowState(Qt::WindowMaximized);
}

Logowanie::~Logowanie()
{
    delete ui;
}

QString Logowanie::getUser()
{
    return userlogin;
}

QString Logowanie::getPasswd()
{
    return userpasswd;
}

void Logowanie::cleartext()
{
    ui->lineEditLogin->clear();
    ui->lineEditHaslo->clear();
}

void Logowanie::on_buttonBox_accepted()
{
    this->userlogin = ui->lineEditLogin->text();
    this->userpasswd = ui->lineEditHaslo->text();
    this->close();
}

void Logowanie::on_buttonBox_rejected()
{
    this->close();
}
