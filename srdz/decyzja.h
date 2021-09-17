#ifndef DECYZJA_H
#define DECYZJA_H

#include <QDialog>

namespace Ui {
class Decyzja;
}

class Decyzja : public QDialog
{
    Q_OBJECT

public:
    explicit Decyzja(QWidget *parent = nullptr);
    ~Decyzja();
    QString gettext();
    void writeid(int id);
    void writename(QString name);
    void writetype(QString type);
    void writetime(QString time);
    void writeplace(QString place);
    void writeuser(QString user);
    void setdescription(QString desc);


private:
    Ui::Decyzja *ui;
};

#endif // DECYZJA_H
