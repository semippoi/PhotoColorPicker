#include "LayerBrowserItem.h"

LayerBrowserItem::LayerBrowserItem()
{
    display = true;
    opacity = 255;
}

void LayerBrowserItem::render(QPainter *painter)
{
    // TODO: Itemとして描画される内容物
    // レイヤー本体、レイヤー名、レイヤー可視アイコン（目）、透明度（%表示）
}

void LayerBrowserItem::setLayerBrowserItem(QString name, QPixmap pixmap)
{
    layer_name = name;
    layer = pixmap;
}

void LayerBrowserItem::setOpacity(double op)
{
    opacity = op;
}

void LayerBrowserItem::setDisplay(bool dp)
{
    display = dp;
}

void LayerBrowserItem::setLayerName(QString name)
{
    layer_name = name;
}

QString LayerBrowserItem::layerName()
{
    return layer_name;
}

