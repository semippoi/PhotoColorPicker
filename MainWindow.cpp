#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(&scene);

    connect(ui->graphicsView, SIGNAL(getColor(QColor &)), this, SLOT(displayGetColor(QColor &)));
    current_color = Qt::black;
    ui->toolButton->setStyleSheet("background-color:#000000");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    // �摜��\������
    QString path = QFileDialog::getOpenFileName(this, "Open", QString(), tr("Image Files (*.bmp *.png *.jpg)"));
    if (path.isEmpty()) return;
    QPixmap pixmap(path);
    scene.addPixmap(pixmap);
    ui->graphicsView->initializeLayer(pixmap);

    ui->lineEdit->setText(path);

    // �摜�ɍ��킹�ĐL�k����
    ui->graphicsView->setFixedSize(pixmap.width() + 50, pixmap.height() + 50);
}

void MainWindow::displayGetColor(QColor &color)
{
    QString css = QString("background-color:%1").arg(color.name());
    ui->toolButton->setStyleSheet(css);
}
