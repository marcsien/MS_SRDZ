#include "pokazdec.h"
#include "ui_pokazdec.h"

Pokazdec::Pokazdec(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Pokazdec)
{
    ui->setupUi(this);
}

Pokazdec::~Pokazdec()
{
    delete ui;
}

void Pokazdec::writeid(int id)
{
    ui->lineEditId->setText(QString("%1").arg(id));
}

void Pokazdec::writename(QString name)
{
    ui->lineEditNazwa->setText(name);
}

void Pokazdec::writetype(QString type)
{
    ui->lineEditTyp->setText(type);
}

void Pokazdec::writetime(QString time)
{
    ui->lineEditCzas->setText(time);
}

void Pokazdec::writeplace(QString place)
{
    ui->lineEditMiejsce->setText(place);
}

void Pokazdec::writeuser(QString user)
{
    ui->lineEditUzytkownik->setText(user);
}

void Pokazdec::setdescription(QString desc)
{
    ui->textEdit->setDisabled(1);
    ui->textEdit->setText(desc);
}

void Pokazdec::writedectime(QString time2)
{
    ui->lineEditCzasObslugi->setText(time2);
}

void Pokazdec::writedispatcher(QString disp)
{
    ui->lineEditDyspozytor->setText(disp);
}
