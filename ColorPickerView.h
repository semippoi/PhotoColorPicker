#ifndef COLORPICKERVIEW_H
#define COLORPICKERVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QMouseEvent>

#include "MainWindow.h"

class ColorPickerView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ColorPickerView(QWidget *parent = 0);

    void initializeLayer(QPixmap const &pixmap);

    virtual void mousePressEvent(QMouseEvent *event);
    virtual void paintEvent(QPaintEvent *);
private:
    QColor pickUpColor(QPixmap pixmap, QPoint pos);
    void invalidatePixmap();

protected:
    struct Private;
    Private *pv;

signals:
    void getColor(QColor &);
public slots:
    void changeCurrentColor(QColor &color);
public slots:
};

#endif // COLORPICKERVIEW_H
