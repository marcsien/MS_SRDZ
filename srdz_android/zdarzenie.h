#ifndef ZDARZENIE_H
#define ZDARZENIE_H

#include <QDialog>
#include <QString>

namespace Ui {
class Zdarzenie;
}

class Zdarzenie : public QDialog
{
    Q_OBJECT

public:
    explicit Zdarzenie(QWidget *parent = nullptr);
    ~Zdarzenie();
    int getType();
    QString getName();
    void puttype(QString type);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::Zdarzenie *ui;
    QString Name;
    int IDZdarz;
};

#endif // ZDARZENIE_H
