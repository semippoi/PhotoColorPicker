#ifndef DRAWINGINFORMATION_H
#define DRAWINGINFORMATION_H

#include "LayerBrowserItem.h"

struct DrawingInformation
{
    QList<LayerBrowserItem> *item_list;
public:
    DrawingInformation();
    QList<LayerBrowserItem> *itemList();
    void setList(LayerBrowserItem &item);
};

#endif // DRAWINGINFORMATION_H
