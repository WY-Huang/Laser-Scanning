#ifndef LASER_PARAMSETINGDLG_H
#define LASER_PARAMSETINGDLG_H

#include <QDialog>

#include "my_params.h"


namespace Ui {
class laser_paramsetingdlg;
}

class laser_paramsetingdlg : public QDialog
{
    Q_OBJECT

public:
    explicit laser_paramsetingdlg(My_params *mcs, QWidget *parent = nullptr);
    ~laser_paramsetingdlg();

    My_params *m_mcs;

    void Initparam(My_params *mcs);      //初始化

private slots:
    void on_tabWidget_currentChanged(int index);

private:
    Ui::laser_paramsetingdlg *ui;
};

#endif // LASER_PARAMSETINGDLG_H
