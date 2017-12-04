#include "ColorPickerView.h"

#include <QGraphicsItem>

struct ColorPickerView::Private {
    QPixmap base_pixmap;
    QPixmap layer;
    bool transparent;
    double right;
    QColor current_color;
    std::vector<QPoint> fill_point;    // 塗りつぶすpixel保存用のスタック
    Private(){
        transparent = true;
        fill_point.clear();
        current_color = Qt::black;
    }
};

ColorPickerView::ColorPickerView(QWidget *parent) : QGraphicsView(parent)
{
    pv = new Private();
    invalidatePixmap();
}



void ColorPickerView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() & Qt::RightButton) {
        // 画像の色抽出
        QColor rgb = pickUpColor(pv->layer, event->pos());
        if (rgb.alpha() == 0) {
            rgb = pickUpColor(pv->base_pixmap, event->pos());
            pv->transparent = true;
        } else {
            pv->transparent = false;
        }
        pv->current_color = rgb;

        emit getColor(rgb);

    } else if (event->button() & Qt::LeftButton) {
        // 塗りつぶし機能
        //pv->layer = QPixmap(pv->base_pixmap.width(), pv->base_pixmap.height());

        pv->right = this->width();

        std::vector<QPoint> stack;
        stack.push_back(event->pos());
        pv->fill_point.push_back(event->pos());
        std::vector<QPoint>::iterator it1 = stack.begin();

        int xleft = 0, xright = 0;

        QPixmap compare;
        if (pv->transparent) {
            compare = pv->base_pixmap;
        } else {
            compare = pv->layer;
        }

        while (it1 != stack.end()) {
            QPoint p = *it1;

            // 左の境界を探す
            for (int i = 1; i > 0 && i < p.x() ; i++) {
                QPoint q = QPoint(p.x() - i, p.y());
                if (pickUpColor(compare, p) != pickUpColor(compare, q)) {
                    xleft = p.x() - i + 1;
                    break;
                } else {
                    pv->fill_point.push_back(q);
                }
            }

            // 右の境界を探す
            for (int j = 1; j > p.x() && j < pv->right ; j++) {
                QPoint q = QPoint(p.x() - j, p.y());
                if (pickUpColor(compare, p) != pickUpColor(compare, q)) {
                    xright = p.x() - j - 1;
                    break;
                } else {
                    pv->fill_point.push_back(q);
                }
            }

            // (xleft, Y-1)から(xright, Y-1)を走査して境界の一個前のピクセルをスタックに積む
            for (int k = xleft; k < xright; k++) {
                QPoint q = QPoint(k, p.y() - 1);
                if (pickUpColor(compare, p) != pickUpColor(compare, q)) {
                    stack.push_back(QPoint(k - 1, p.y() - 1));
                } else {
                    pv->fill_point.push_back(q);
                }
            }

            // (xleft, Y+1)から(xright, Y+1)を走査して境界の一個前のピクセルをスタックに積む
            for (int l = xleft; l < xright; l++) {
                QPoint q = QPoint(l, p.y() + 1);
                if (pickUpColor(compare, p) != pickUpColor(compare, q)) {
                    stack.push_back(QPoint(l - 1, p.y() + 1));
                } else {
                    pv->fill_point.push_back(q);
                }
            }

            ++it1;
        }

        update();
//        while (it2 != pv->fill_point.end()) {
//            QPoint fp = *it2;
//            repaint();
//            ++it2;
//        }
    }
}

void ColorPickerView::paintEvent(QPaintEvent *)
{
    QPainter painter(viewport());

    std::vector<QPoint>::iterator it = pv->fill_point.begin();
    while (it != pv->fill_point.end()) {
        QPoint fp = *it;
        pv->layer.fill(pv->current_color);
        ++it;
    }

    painter.drawPixmap(0, 0, pv->base_pixmap);
    painter.drawPixmap(0, 0, pv->layer);

    pv->fill_point.clear();
    viewport()->update();
}

// 色の抽出
QColor ColorPickerView::pickUpColor(QPixmap pixmap, QPoint pos)
{
    QImage img = pixmap.toImage();
    QColor rgb = img.pixelColor(pos.x(), pos.y());

    return rgb;
}

void ColorPickerView::invalidatePixmap()
{
    pv->base_pixmap = QPixmap();
    pv->layer = QPixmap();
}

void ColorPickerView::changeCurrentColor(QColor &color)
{
    pv->current_color = color;
}

void ColorPickerView::initializeLayer(const QPixmap &pixmap)
{
    invalidatePixmap();
    pv->base_pixmap = pixmap;
    pv->layer = QPixmap(pixmap.width(), pixmap.height());
    pv->layer.fill(QColor(0, 0, 0, 0));
}

