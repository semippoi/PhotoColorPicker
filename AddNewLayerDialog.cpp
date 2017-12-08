#include "AddNewLayerDialog.h"
#include "ui_AddNewLayerDialog.h"

#include <QMessageBox>

AddNewLayerDialog::AddNewLayerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddNewLayerDialog)
{
    ui->setupUi(this);
}

AddNewLayerDialog::~AddNewLayerDialog()
{
    delete ui;
}

QString AddNewLayerDialog::layerName()
{
    return layer_name;
}

void AddNewLayerDialog::on_buttonBox_accepted()
{
    layer_name = ui->lineEdit->text();
    if (layer_name.isEmpty()) {
        QMessageBox::warning(this, "Add New Layer", "New layer name is Empty!");
        //QDialog::reject();
    } else {
        QDialog::accept();
    }
}

void AddNewLayerDialog::on_buttonBox_rejected()
{
    QDialog::reject();
}
