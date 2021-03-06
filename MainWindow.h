#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

#include "DrawingInformation.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QGraphicsScene scene;
    QColor current_color;
    DrawingInformation *info;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QColor getCurrentColor();

private:
    void loadImageFile();
    void initDrawingInformation(DrawingInformation *di);

private slots:
    void displayGetColor(QColor &color);

    void on_toolButton_clicked();
    void on_toolButton_drawBrush_toggled(bool checked);
    void on_toolButton_fill_toggled(bool checked);
    void on_actionLoad_Image_File_triggered();
    void on_toolButton_colorize_toggled(bool checked);
    void on_toolButton_layer_clear_clicked();
    void on_toolButton_add_layer_clicked();

signals:
    void currentColorChanged(QColor &color);
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
