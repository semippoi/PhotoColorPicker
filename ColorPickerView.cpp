#include "ColorPickerView.h"

#include <QGraphicsItem>

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
    std::list<QPoint> target_stack;
    std::list<QPoint> stack;

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
}



void ColorPickerView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() & Qt::LeftButton) {
        if (pv->t.brush_tool) {
            // �t���[�n���h�`��
            pv->lastPoint = event->pos();
            pv->drawing_layer = pv->layer.toImage();
            QPainter painter(&pv->drawing_layer);
            prepare_draw_brush(&painter, event->pos());
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
        }
    }
}

void ColorPickerView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->button() & Qt::LeftButton && pv->t.brush_tool) {
        drawLineTo(event->pos());
        viewport()->update();
    }
}

void ColorPickerView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() & Qt::LeftButton && pv->t.brush_tool) {
        drawLineTo(event->pos());
        viewport()->update();
    }
}

void ColorPickerView::paintEvent(QPaintEvent *)
{
    QPainter painter(viewport());

    QImage img = pv->layer.toImage();

    painter.drawPixmap(0, 0, pv->base_pixmap);
    painter.drawImage(0, 0, img);


    //painter.drawPixmap(0, 0, pv->layer);

    pv->fill_point.clear();
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
    pv->right = pv->base_pixmap.width();

    pv->fill_point.push_back(point);
    auto it1 = pv->target_stack.begin();
    it1 = pv->target_stack.insert(it1, point);

    int xleft = 0, xright = 0;

    QPixmap compare;
    if (pv->transparent) {
        compare = pv->base_pixmap;
    } else {
        compare = pv->layer;
    }
    QColor target_color = pickUpColor(compare, point);

    while (it1 != pv->target_stack.end()) {
        QPoint p = *it1;

        if (pickUpColor(pv->layer, p) == pv->current_color) {
            ++it1;
            continue;
        }

        // ���̋��E��T��
        for (int i = 1; i < p.x(); i++) {
            QPoint q = QPoint(p.x() - i, p.y());
            if (pickUpColor(compare, p) != pickUpColor(compare, q)) {
                xleft = p.x() - i + 1;
                break;
            } else {
                pv->fill_point.push_back(q);
            }
        }

        // �E�̋��E��T��
        for (int j = 1; j < pv->right ; j++) {
            QPoint q = QPoint(p.x() + j, p.y());
            if (pickUpColor(compare, p) != pickUpColor(compare, q)) {
                xright = p.x() + j - 1;
                break;
            } else {
                pv->fill_point.push_back(q);
            }
        }

        // xleft����xright�܂ł̐�����`�悷��
        std::vector<QPoint>::iterator it2 = pv->fill_point.begin();
        pv->drawing_layer = pv->layer.toImage();
        while (it2 != pv->fill_point.end()) {
            QPoint fp = *it2;
            pv->drawing_layer.setPixelColor(fp, pv->current_color);
            it2++;
        }
        pv->layer.convertFromImage(pv->drawing_layer);
        pv->fill_point.clear();


        // (xleft, Y-1)����(xright, Y-1)�𑖍����ċ��E�̉E�[�̃s�N�Z�����X�^�b�N�ɐς�
        if (p.y() - 1 >= 0) {
            scanLine(compare, xleft, xright, p.y() - 1, target_color);
        }

        // (xleft, Y+1)����(xright, Y+1)�𑖍����ċ��E�̈�O�̃s�N�Z�����X�^�b�N�ɐς�
        if (p.y() + 1 <= pv->base_pixmap.height()) {
            scanLine(compare, xleft, xright, p.y() + 1, target_color);
        }

//        pv->stack.clear();
//        auto it_ = pv->stack.begin();
//        auto ite = pv->target_stack.begin();
//        while (it_ != pv->stack.end()) {
//            pv->target_stack.emplace_back(*it_);
//            ++it_;
//            ++ite;
//        }

        ++it1;
    }

//    std::vector<QPoint>::iterator it2 = pv->fill_point.begin();
//    pv->drawing_layer = pv->layer.toImage();
//    while (it2 != pv->fill_point.end()) {
//        QPoint fp = *it2;
//        pv->drawing_layer.setPixelColor(fp, pv->current_color);
//        it2++;
//    }
//    pv->layer.convertFromImage(pv->drawing_layer);

    viewport()->update();
}

void ColorPickerView::prepare_draw_brush(QPainter *painter, QPoint point)
{
    painter->setPen(QPen(pv->current_color, 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

}

void ColorPickerView::drawLineTo(QPoint point)
{
    QPainter painter(&pv->drawing_layer);
    painter.drawLine(pv->lastPoint, point);

    pv->lastPoint = point;
}

void ColorPickerView::scanLine(QPixmap compare, int xleft, int xright, int y, QColor col)
{
    while (xleft <= xright) {
        // ��̈�F���΂�
        for (; xleft <= xright; xleft++) {
            if (pickUpColor(compare, QPoint(xleft, y)) == col) break;
        }
        if (pickUpColor(compare, QPoint(xleft, y)) != col) break;

        // �̈�F���΂�
        for (; xleft <= xright; xleft++) {
            if (pickUpColor(compare, QPoint(xleft, y)) != col) break;
        }
//        pv->stack.push_back(QPoint(xleft - 1, y));
        pv->target_stack.emplace_back(QPoint(xleft - 1, y));
    }
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
    pv->drawing_layer = pv->layer.toImage();
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

