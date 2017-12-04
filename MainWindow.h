#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QGraphicsScene scene;
    QColor current_color;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QColor getCurrentColor();

private slots:
    void on_pushButton_clicked();
    void displayGetColor(QColor &color);

    void on_toolButton_clicked();

signals:
    void currentColorChanged(QColor &color);
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
