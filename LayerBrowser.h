#ifndef LAYERBROWSER_H
#define LAYERBROWSER_H

#include <QWidget>
#include <QListWidget>
#include "LayerBrowserItem.h"

class LayerBrowser : public QListWidget
{
    Q_OBJECT
private:
    QList<LayerBrowserItem> my_items_;
    QList<LayerBrowserItem> items;
    QPixmap layer;
    QListWidgetItem *newItem(int row);
public:
    LayerBrowser(QWidget *parent);

//    void bind(QList<LayerBrowserItem> items);
    void makeList();
    void makeList(QList<LayerBrowserItem> &items);
    bool setItem(int row, LayerBrowserItem &item);
    QList<LayerBrowserItem> list();
    QPixmap selectedLayer();
};

#endif // LAYERBROWSER_H
