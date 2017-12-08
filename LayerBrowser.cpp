#include "LayerBrowser.h"

LayerBrowser::LayerBrowser(QWidget *parent)
    : QListWidget(parent)
{
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    LayerBrowserItem item;
    item.setLayerName("Layer_1");
    items.push_back(item);
    setItem(0, item);
}

//void LayerBrowser::bind(QList<LayerBrowserItem> items)
//{
//    this->items = items ? items : &my_items_;
//}

QListWidgetItem *LayerBrowser::newItem(int row)
{
    QListWidgetItem *item = new QListWidgetItem();
    item->setData(Qt::UserRole, row);
    return item;
}

void LayerBrowser::makeList()
{
    clear();
    for (int i = 0; i < items.size(); i++) {
        addItem(newItem(i));
    }
}

void LayerBrowser::makeList(QList<LayerBrowserItem> &items)
{
//    bind(0);
    my_items_ = items;
    makeList();
}

bool LayerBrowser::setItem(int row, LayerBrowserItem &item)
{
    // TODO:
    //Q_ASSERT(this->count() == items->size());
    if (row >= 0 && row <= items.size()) {
        items.push_back(item);
        insertItem(row, item.layerName());
        return true;
    }
    return false;
}

QList<LayerBrowserItem> LayerBrowser::list()
{
    return items;
}

QPixmap LayerBrowser::selectedLayer()
{
    return layer;
}
