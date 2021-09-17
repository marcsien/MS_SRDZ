#ifndef LOGOWANIE_H
#define LOGOWANIE_H

#include <QDialog>

namespace Ui {
class Logowanie;
}

class Logowanie : public QDialog
{
    Q_OBJECT

public:
    explicit Logowanie(QWidget *parent = nullptr);
    ~Logowanie();
    void cleartext();
    QString getUser();
    QString getPassword();

private slots:
    void on_pushButtonAnuluj_clicked();

    void on_pushButtonZaloguj_clicked();

private:
    Ui::Logowanie *ui;
    QString userlogin;
    QString userpassword;
};

#endif // LOGOWANIE_H
