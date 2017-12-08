#ifndef LAYERBROWSERITEM_H
#define LAYERBROWSERITEM_H

#include <QSharedPointer>
#include <QPainter>

struct LayerBrowserItem
{
    QString layer_name;    // レイヤー名
    QPixmap layer;         // レイヤー本体
    bool display;          // 表示・非表示の設定
    double opacity;        // 透明度

    LayerBrowserItem();

    void render(QPainter *painter);
    void setLayerBrowserItem(QString name, QPixmap pixmap);
    void setOpacity(double op);
    void setDisplay(bool dp);
    void setLayerName(QString name);
    QString layerName();
};

#endif // LAYERBROWSERITEM_H
