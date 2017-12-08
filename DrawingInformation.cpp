#include "DrawingInformation.h"

DrawingInformation::DrawingInformation()
{

}

QList<LayerBrowserItem> *DrawingInformation::itemList()
{
    return item_list;
}

void DrawingInformation::setList(LayerBrowserItem &item)
{
    item_list->push_back(item);
}
