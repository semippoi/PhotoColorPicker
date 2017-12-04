#include "ColorPickerView.h"

#include <QGraphicsItem>

struct ColorPickerView::Private {
    QPixmap base_pixmap;
    QPixmap layer;
    double right;
    QColor current_color;
    Private(){

    }
};

ColorPickerView::ColorPickerView(QWidget *parent) : QGraphicsView(parent)
{
    pv = new Private();
}



void ColorPickerView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() & Qt::RightButton) {
        // 画像の色抽出
        QColor rgb = pickUpColor(event->pos());

        emit getColor(rgb);

    } else if (event->button() & Qt::LeftButton) {
        // 塗りつぶし機能
        pv->layer = QPixmap(pv->base_pixmap.width(), pv->base_pixmap.height());
        QPainter painter(this);

        pv->right = this->width();

        std::vector<QPoint> stack;
        stack.push_back(event->pos());
        std::vector<QPoint>::iterator it1 = stack.begin();

        // 塗りつぶすpixel保存用のスタックとイテレータ
        std::vector<QPoint> fill_point;
        std::vector<QPoint>::iterator it2 = fill_point.begin();

        int xleft = 0, xright = 0;

        while (it1 != stack.end()) {
            QPoint p = *it1;

            // 左の境界を探す
            for (int i = 1; i > 0 && i < p.x() ; i++) {
                QPoint q = QPoint(p.x() - i, p.y());
                if (pickUpColor(p) != pickUpColor(q)) {
                    xleft = p.x() - i + 1;
                    break;
                } else {
                    fill_point.push_back(q);
                }
            }

            // 右の境界を探す
            for (int j = 1; j > p.x() && j < pv->right ; j++) {
                QPoint q = QPoint(p.x() - j, p.y());
                if (pickUpColor(p) != pickUpColor(q)) {
                    xright = p.x() - j - 1;
                    break;
                } else {
                    fill_point.push_back(q);
                }
            }

            // (xleft, Y-1)から(xright, Y-1)を走査して境界の一個前のピクセルをスタックに積む
            for (int k = xleft; k < xright; k++) {
                QPoint q = QPoint(k, p.y() - 1);
                if (pickUpColor(p) != pickUpColor(q)) {
                    stack.push_back(QPoint(k - 1, p.y() - 1));
                } else {
                    fill_point.push_back(q);
                }
            }

            // (xleft, Y+1)から(xright, Y+1)を走査して境界の一個前のピクセルをスタックに積む
            for (int l = xleft; l < xright; l++) {
                QPoint q = QPoint(l, p.y() + 1);
                if (pickUpColor(p) != pickUpColor(q)) {
                    stack.push_back(QPoint(l - 1, p.y() + 1));
                } else {
                    fill_point.push_back(q);
                }
            }

            ++it1;
        }

        while (it2 != fill_point.end()) {
            QPoint fp = *it2;
            //pv->layer.fill(fp, parent()->);
            ++it2;
        }
    }
}

// 色の抽出
QColor ColorPickerView::pickUpColor(QPoint pos)
{
    QPixmap pixmap = this->grab();
    QImage img = pixmap.toImage();
    QColor rgb = img.pixelColor(pos.x(), pos.y());

    return rgb;
}

void ColorPickerView::invalidatePixmap()
{
    pv->base_pixmap = QPixmap();
    pv->layer = QPixmap();
}

void ColorPickerView::initializeLayer(const QPixmap &pixmap)
{
    invalidatePixmap();
    pv->layer = QPixmap(pixmap.width(), pixmap.height());
}
