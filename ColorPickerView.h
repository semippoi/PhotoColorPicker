#ifndef COLORPICKERVIEW_H
#define COLORPICKERVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QMouseEvent>
#include <list>

#include "MainWindow.h"

struct RegionFiller{
    int xleft; // 領域左端のX座標
    int xright;  // 領域右端のX座標
    int y; // 領域のY座標
    int oy; // 親ラインのY座標
    RegionFiller(int xleft, int xright, int y, int oy)
        : xleft(xleft)
        , xright(xright)
        , y(y)
        , oy(oy)
    {
    }
    bool operator == (const RegionFiller &a) const
    {
        if (xleft == a.xleft && xright == a.xright && y == a.y && oy == a.oy) {
            return true;
        } else {
            return false;
        }
    }
};

class ColorPickerView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ColorPickerView(QWidget *parent = 0);

    void initializeLayer(QPixmap const &pixmap);

    void setBrushTool(bool checked);
    void setFillTool(bool checked);
    void setColorizeTool(bool checked);

    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void paintEvent(QPaintEvent *);
private:
    QColor pickUpColor(QPixmap pixmap, QPoint pos);
    void invalidatePixmap();

    void fill_color(QPoint point);
    void prepare_draw_brush(QPainter *painter, QPoint point);
    void drawLineTo(QPoint point);
    void scanLine(QPixmap compare, int xleft, int xright, int y, int yparent, QColor col);

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
