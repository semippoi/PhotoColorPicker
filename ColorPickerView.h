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
private:
    QColor pickUpColor(QPoint pos);
    void invalidatePixmap();

protected:
    struct Private;
    Private *pv;

signals:
    void getColor(QColor &);
public slots:
};

#endif // COLORPICKERVIEW_H
