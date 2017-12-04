#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFileDialog>
#include <QColorDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(&scene);

    connect(ui->graphicsView, SIGNAL(getColor(QColor &)), this, SLOT(displayGetColor(QColor &)));
    connect(this, SIGNAL(currentColorChanged(QColor &)), this, SLOT(displayGetColor(QColor &)));

    current_color = Qt::black;
    ui->toolButton->setStyleSheet("background-color:#000000");
}

MainWindow::~MainWindow()
{
    delete ui;
}

QColor MainWindow::getCurrentColor()
{
    return current_color;
}

void MainWindow::on_pushButton_clicked()
{
    // ‰æ‘œ‚ð•\Ž¦‚·‚é
    QString path = QFileDialog::getOpenFileName(this, "Open", QString(), tr("Image Files (*.bmp *.png *.jpg)"));
    if (path.isEmpty()) return;
    QPixmap pixmap(path);
    scene.addPixmap(pixmap);
    ui->graphicsView->initializeLayer(pixmap);

    ui->lineEdit->setText(path);

    // ‰æ‘œ‚É‡‚í‚¹‚ÄLk‚·‚é
    ui->graphicsView->setFixedSize(pixmap.width() + 50, pixmap.height() + 50);
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
