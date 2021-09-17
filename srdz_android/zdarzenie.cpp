#include "zdarzenie.h"
#include "ui_zdarzenie.h"
#include <QDebug>

Zdarzenie::Zdarzenie(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Zdarzenie)
{
    ui->setupUi(this);
    ui->label->setAlignment(Qt::AlignCenter);
    ui->label_2->setAlignment(Qt::AlignCenter);
    setWindowState(Qt::WindowMaximized);
}


Zdarzenie::~Zdarzenie()
{
    delete ui;
}

int Zdarzenie::getType()
{
    return IDZdarz;
}

QString Zdarzenie::getName()
{
    return Name;
}

void Zdarzenie::puttype(QString type)
{
    ui->comboBoxTyp->addItem(type);
    qDebug()<<ui->comboBoxTyp->count();
}

void Zdarzenie::on_buttonBox_accepted()
{
    this->Name = ui->lineEditNazwa->text();
    this->IDZdarz = ui->comboBoxTyp->currentIndex();
    this->close();
}

void Zdarzenie::on_buttonBox_rejected()
{
    close();
}
