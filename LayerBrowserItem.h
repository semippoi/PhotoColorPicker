#ifndef LAYERBROWSERITEM_H
#define LAYERBROWSERITEM_H

#include <QSharedPointer>
#include <QPainter>

struct LayerBrowserItem
{
    QString layer_name;    // ���C���[��
    QPixmap layer;         // ���C���[�{��
    bool display;          // �\���E��\���̐ݒ�
    double opacity;        // �����x

    LayerBrowserItem();

    void render(QPainter *painter);
    void setLayerBrowserItem(QString name, QPixmap pixmap);
    void setOpacity(double op);
    void setDisplay(bool dp);
    void setLayerName(QString name);
    QString layerName();
};

#endif // LAYERBROWSERITEM_H
