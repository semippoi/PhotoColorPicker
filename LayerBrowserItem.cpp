#include "LayerBrowserItem.h"

LayerBrowserItem::LayerBrowserItem()
{
    display = true;
    opacity = 255;
}

void LayerBrowserItem::render(QPainter *painter)
{
    // TODO: Item�Ƃ��ĕ`�悳�����e��
    // ���C���[�{�́A���C���[���A���C���[���A�C�R���i�ځj�A�����x�i%�\���j
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

