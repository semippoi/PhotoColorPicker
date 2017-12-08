#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFileDialog>
#include <QColorDialog>
#include <QMessageBox>

#include "AddNewLayerDialog.h"
#include "DrawingInformation.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/images/app_icon.png"));
    ui->graphicsView->setScene(&scene);
    ui->scrollArea->setGraphicView(ui->graphicsView);

    // コネクト
    connect(ui->graphicsView, SIGNAL(getColor(QColor &)), this, SLOT(displayGetColor(QColor &)));
    connect(this, SIGNAL(currentColorChanged(QColor &)), this, SLOT(displayGetColor(QColor &)));
    connect(this, SIGNAL(currentColorChanged(QColor &)), ui->graphicsView, SLOT(changeCurrentColor(QColor &)));

    current_color = Qt::black;
    ui->toolButton->setStyleSheet("background-color:#000000");

    DrawingInformation *info = new DrawingInformation();
    initDrawingInformation(info);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QColor MainWindow::getCurrentColor()
{
    return current_color;
}

void MainWindow::loadImageFile()
{
    // 画像を表示する
    QString path = QFileDialog::getOpenFileName(this, "Open", QString(), tr("Image Files (*.bmp *.png *.jpg)"));
    if (path.isEmpty()) return;
    QPixmap pixmap(path);
    //scene.addPixmap(pixmap);
    ui->graphicsView->initializeLayer(pixmap);

    statusBar()->showMessage(path);

    // 画像に合わせて伸縮する
    ui->graphicsView->setFixedSize(pixmap.width(), pixmap.height());
}

void MainWindow::initDrawingInformation(DrawingInformation *di)
{
    info = di;
}

void MainWindow::displayGetColor(QColor &color)
{
    QString css = QString("background-color:%1").arg(color.name());
    ui->toolButton->setStyleSheet(css);
    current_color = color;
}

void MainWindow::on_toolButton_clicked()
{
    QColorDialog dlg;
    if (dlg.exec() == QDialog::Accepted) {
        current_color = dlg.selectedColor();
        emit currentColorChanged(current_color);
    }
}

void MainWindow::on_toolButton_drawBrush_toggled(bool checked)
{
    if (checked) {
        ui->toolButton_fill->setChecked(false);
        ui->graphicsView->setFillTool(false);
        ui->toolButton_colorize->setChecked(false);
        ui->graphicsView->setColorizeTool(false);
    }
    ui->graphicsView->setBrushTool(checked);
}

void MainWindow::on_toolButton_fill_toggled(bool checked)
{
    if (checked) {
        ui->toolButton_drawBrush->setChecked(false);
        ui->graphicsView->setBrushTool(false);
        ui->toolButton_colorize->setChecked(false);
        ui->graphicsView->setColorizeTool(false);
    }
    ui->graphicsView->setFillTool(checked);
}

void MainWindow::on_actionLoad_Image_File_triggered()
{
    loadImageFile();
}

void MainWindow::on_toolButton_colorize_toggled(bool checked)
{
    if (checked) {
        ui->toolButton_drawBrush->setChecked(false);
        ui->graphicsView->setBrushTool(false);
        ui->toolButton_fill->setChecked(false);
        ui->graphicsView->setFillTool(false);
    }
    ui->graphicsView->setColorizeTool(checked);
}

void MainWindow::on_toolButton_layer_clear_clicked()
{
    if (QMessageBox::warning(this, "", "Does Drawing Layer erase?", QMessageBox::Yes | QMessageBox::No)) {
        ui->graphicsView->initializeLayer(QPixmap());
    }
}

void MainWindow::on_toolButton_add_layer_clicked()
{
    AddNewLayerDialog *dlg = new AddNewLayerDialog();
    if (dlg->exec() == QDialog::Accepted) {
        // TODO: ここでLayerBrowserItemを作成する
        LayerBrowserItem item;
        item.setLayerName(dlg->layerName());
        // スタック的なサムシングにぶち込む
        ui->listWidget->setItem(ui->listWidget->count(), item);
    }
}
