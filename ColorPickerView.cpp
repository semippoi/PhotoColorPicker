#include "ColorPickerView.h"

#include <QGraphicsItem>
#include <iostream>
#include <algorithm>
#include <vector>

struct ColorPickerView::Private {
    struct Tool{
        bool brush_tool;
        bool fill_tool;
        bool colorize_tool;
    } t;
    QPixmap base_pixmap;
    QPixmap layer;
    QImage drawing_layer;

    bool transparent;
    double right;
    QColor current_color;

    std::vector<QPoint> fill_point;    // �h��Ԃ�pixel�ۑ��p�̃X�^�b�N
    std::vector<RegionFiller> target_stack;

    QPoint lastPoint;
    Private(){
        t.brush_tool = false;
        t.fill_tool = false;
        t.colorize_tool = false;
        transparent = true;
        fill_point.clear();
        current_color = Qt::black;
        target_stack.clear();
    }
};

ColorPickerView::ColorPickerView(QWidget *parent) : QGraphicsView(parent)
{
    pv = new Private();
    invalidatePixmap();
    pv->layer = QPixmap(500, 500);
    pv->drawing_layer = pv->layer.toImage();

    update();
}



void ColorPickerView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (pv->t.brush_tool) {
            // �t���[�n���h�`��
            pv->lastPoint = event->pos();
            pv->drawing_layer = pv->layer.toImage();
//            QPainter painter(&pv->drawing_layer);
//            prepare_draw_brush(&painter, event->pos());
        } else if (pv->t.fill_tool) {
            // �h��Ԃ�
            fill_color(event->pos());
        } else if (pv->t.colorize_tool) {
            // �摜�̐F���o
            QColor rgb = pickUpColor(pv->layer, event->pos());
            if (rgb.alpha() == 0) {
                rgb = pickUpColor(pv->base_pixmap, event->pos());
                pv->transparent = true;
            } else {
                pv->transparent = false;
            }
            pv->current_color = rgb;

            emit getColor(rgb);
        } else {
            qDebug("ClickPos : ( %d, %d)", event->pos().x(), event->pos().y());
        }
    }
}

void ColorPickerView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->button() & Qt::LeftButton && pv->t.brush_tool) {
        drawLineTo(event->pos());
    }
}

void ColorPickerView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && pv->t.brush_tool) {
        drawLineTo(event->pos());
    }
}

void ColorPickerView::paintEvent(QPaintEvent *)
{
    QPainter painter(viewport());

    QImage img = pv->layer.toImage();

    painter.drawPixmap(0, 0, pv->base_pixmap);
    painter.drawImage(0, 0, img);


    //painter.drawPixmap(0, 0, pv->layer);

    //pv->fill_point.clear();
}

// �F�̒��o
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

// �̈�h��Ԃ�
void ColorPickerView::fill_color(QPoint point)
{
    // �E�[�̌���
    pv->right = pv->base_pixmap.width();
    if (pv->base_pixmap.isNull()){
        pv->right = 500;
    }

    int xleft = point.x(), xright = point.x();
    int yparent;

    pv->fill_point.push_back(point);
    pv->target_stack.push_back(RegionFiller(point.x(), xright, point.y(), point.y()));


    QPixmap compare;
    if (pv->transparent) {
        compare = pv->base_pixmap;
    } else {
        compare = pv->layer;
    }
    QColor target_color = pickUpColor(compare, point);

    while (!pv->target_stack.empty()) {
        RegionFiller region_filler = pv->target_stack.back();
        pv->target_stack.pop_back();

        xleft = region_filler.xleft;
        xright = region_filler.xright;
        int y = region_filler.y;
        yparent = region_filler.oy;

        int xleft_save = xleft - 1;
        if (xleft_save < 0) xleft_save = 0;
        int xright_save = xright + 1;
        if (xright_save > pv->right) xright_save = pv->right;

        // ���̋��E��T��
        while (xleft >= 0) {
            QPoint q = QPoint(xleft, y);
            if (pickUpColor(compare, q) != target_color) {
                break;
            } else {
                pv->fill_point.push_back(q);
                if (xleft == 0) break;
            }
            xleft--;
        }

        // �E�̋��E��T��
        while (xright < pv->right) {
            QPoint q = QPoint(xright, y);
            if (pickUpColor(compare, q) != target_color) {
                break;
            } else {
                pv->fill_point.push_back(q);
                if (xright == pv->right - 1) break;
            }
            xright++;
        }

        // xleft����xright�܂ł̐�����`�悷��
//        std::vector<QPoint>::iterator it = pv->fill_point.begin();
        pv->drawing_layer = pv->layer.toImage();

        for (int i = xleft; i < xright; i++) {
            pv->drawing_layer.setPixelColor(i, y, pv->current_color);
        }
//        while (it != pv->fill_point.end()) {
//            QPoint fp = *it;
//            pv->drawing_layer.setPixelColor(fp, pv->current_color);
//            it++;
//        }
        pv->layer.convertFromImage(pv->drawing_layer);
//        pv->fill_point.clear();


        // (xleft, Y-1)����(xright, Y-1)�𑖍����ċ��E�̉E�[�̃s�N�Z�����X�^�b�N�ɐς�
        if (y - 1 >= 0) {
            if (y - 1 == yparent) {
                scanLine(compare, xleft, xleft_save, y - 1, y, target_color);
                scanLine(compare, xright_save, xright, y - 1, y, target_color);
            } else {
                scanLine(compare, xleft, xright, y - 1, y, target_color);
            }
        }

        // (xleft, Y+1)����(xright, Y+1)�𑖍����ċ��E�̈�O�̃s�N�Z�����X�^�b�N�ɐς�
        if (y + 1 < pv->base_pixmap.height()) {
            if (y + 1 == yparent) {
                scanLine(compare, xleft, xleft_save, y + 1, y, target_color);
                scanLine(compare, xright_save, xright, y + 1, y, target_color);
            } else {
                scanLine(compare, xleft, xright, y + 1, y, target_color);
            }
        }
        viewport()->update();
    }
}

//void ColorPickerView::prepare_draw_brush(QPainter *painter, QPoint point)
//{
//    painter->setPen(QPen(pv->current_color, 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

//}

void ColorPickerView::drawLineTo(QPoint point)
{
    QPainter painter(&pv->drawing_layer);
    painter.setPen(QPen(pv->current_color, 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(pv->lastPoint, point);


    pv->layer.convertFromImage(pv->drawing_layer);

    viewport()->update();
    pv->lastPoint = point;
}

void ColorPickerView::scanLine(QPixmap compare, int xleft, int xright, int y, int yparent, QColor col)
{
    while (xleft < xright) {
        // ��̈�F���΂�
        for (; xleft < xright; xleft++) {
            if (pickUpColor(compare, QPoint(xleft, y)) == col) break;
        }
        if (pickUpColor(compare, QPoint(xleft, y)) != col) break;
        int left = xleft;

        // �̈�F���΂�
        for (; xleft < xright; xleft++) {
            if (pickUpColor(compare, QPoint(xleft, y)) != col) break;
        }

        //auto it = std::find(pv->target_stack.begin(), pv->target_stack.end(), RegionFiller(left, xleft - 1, y, yparent));
        if (pickUpColor(pv->layer, QPoint(xleft - 1, y)) != pv->current_color) {
            pv->target_stack.emplace_back(RegionFiller(left, xleft - 1, y, yparent));
        }
    }
}

void ColorPickerView::changeCurrentColor(QColor &color)
{
    pv->current_color = color;
}

void ColorPickerView::initializeLayer(const QPixmap &pixmap)
{
    if (!pixmap.isNull()) {
        invalidatePixmap();
        pv->base_pixmap = pixmap;
    }
    pv->layer = QPixmap(pixmap.width(), pixmap.height());
    pv->layer.fill(QColor(0, 0, 0, 0));
    pv->drawing_layer.fill(QColor(0, 0, 0, 0));
}

void ColorPickerView::setBrushTool(bool checked)
{
    pv->t.brush_tool = checked;
}

void ColorPickerView::setFillTool(bool checked)
{
    pv->t.fill_tool = checked;
}

void ColorPickerView::setColorizeTool(bool checked)
{
    pv->t.colorize_tool = checked;
}

