#ifndef ADDNEWLAYERDIALOG_H
#define ADDNEWLAYERDIALOG_H

#include <QDialog>

namespace Ui {
class AddNewLayerDialog;
}

class AddNewLayerDialog : public QDialog
{
    Q_OBJECT
private:
    QString layer_name;

public:
    AddNewLayerDialog(QWidget *parent = 0);
    ~AddNewLayerDialog();
    QString layerName();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::AddNewLayerDialog *ui;
};

#endif // ADDNEWLAYERDIALOG_H
