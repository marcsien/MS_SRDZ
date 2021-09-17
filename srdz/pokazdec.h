#ifndef POKAZDEC_H
#define POKAZDEC_H

#include <QDialog>

namespace Ui {
class Pokazdec;
}

class Pokazdec : public QDialog
{
    Q_OBJECT

public:
    explicit Pokazdec(QWidget *parent = nullptr);
    ~Pokazdec();
    void writeid(int id);
    void writename(QString name);
    void writetype(QString type);
    void writetime(QString time);
    void writeplace(QString place);
    void writeuser(QString user);
    void setdescription(QString desc);
    void writedectime(QString time2);
    void writedispatcher(QString disp);

private:
    Ui::Pokazdec *ui;
};

#endif // POKAZDEC_H
