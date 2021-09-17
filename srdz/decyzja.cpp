#include "decyzja.h"
#include "ui_decyzja.h"

Decyzja::Decyzja(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Decyzja)
{
    ui->setupUi(this);
}

Decyzja::~Decyzja()
{
    delete ui;
}

QString Decyzja::gettext()
{
    QString txt = ui->textEdit->toPlainText();
    return txt;
}

void Decyzja::writeid(int id)
{
    ui->lineEditId->setText(QString("%1").arg(id));
}

void Decyzja::writename(QString name)
{
    ui->lineEditNazwa->setText(name);
}

void Decyzja::writetype(QString type)
{
    ui->lineEditTyp->setText(type);
}

void Decyzja::writetime(QString time)
{
    ui->lineEditCzas->setText(time);
}

void Decyzja::writeplace(QString place)
{
    ui->lineEditMiejsce->setText(place);
}

void Decyzja::writeuser(QString user)
{
    ui->lineEditUzytkownik->setText(user);
}

void Decyzja::setdescription(QString desc)
{
    ui->textEdit->setDisabled(1);
    ui->textEdit->setText(desc);
}
