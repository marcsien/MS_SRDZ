#ifndef LOGOWANIE_H
#define LOGOWANIE_H

#include <QDialog>
#include <QString>

namespace Ui {
class Logowanie;
}

class Logowanie : public QDialog
{
    Q_OBJECT

public:
    explicit Logowanie(QWidget *parent = nullptr);
    ~Logowanie();
    QString getUser();
    QString getPasswd();
    void cleartext();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::Logowanie *ui;
    QString userlogin;
    QString userpasswd;
};

#endif // LOGOWANIE_H
