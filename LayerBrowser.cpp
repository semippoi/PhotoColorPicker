#include "LayerBrowser.h"

#include <QStyledItemDelegate>

class LayerBrowserItemDelegate : public QStyledItemDelegate
{
public:
    LayerBrowserItemDelegate(LayerBrowser *parent)
        : QStyledItemDelegate(parent)
    {

    }

    ~LayerBrowserItemDelegate()
    {
    }

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        LayerBrowser *lb = qobject_cast<LayerBrowser *>(parent());
        return QSize(20, 40);
    }

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        LayerBrowser *lb = qobject_cast<LayerBrowser *>(parent());

        QStyleOptionViewItemV4 opt = option;
        initStyleOption(&opt, index);

        const int icon_size = 30;
        const int item_height = 40;

        int sel = lb->currentRow();

        int w = opt.rect.width();
        int h = opt.rect.height();
        int row = index.row();

        bool selected = (opt.state & QStyle::State_Selected) && (row == sel);

        QPixmap pixmap(w, h);
        {
            QColor bgcolor;
            if (selected) {
                bgcolor = QColor(119, 119, 119);
            } else {
                bgcolor = QColor(220, 220, 220);
            }
            pixmap.fill(bgcolor);
        }

        if (row >= 0 && row < lb->items.size()) {
            LayerBrowserItem const &item = lb->items.at(row);

            QPainter p(&pixmap);

            QRect outer = QRect(0, 0, lb->width(),item_height);
            p.drawRect(outer);

            QRect icon_rect = QRect(0, 5, icon_size, icon_size);
            if (item.display) {
                // レイヤー表示中のアイコン
                QImage icon = QImage(":/images/eye_icon.png");
                p.drawImage(icon_rect, icon);
            } else {
                // レイヤー非表示中のアイコン
                p.drawImage(icon_rect, QImage(":/images/grayeye_icon.png"));
            }
            QRect text_rc = QRect(icon_size, 0, w, item_height);
            p.setPen(QColor(255, 255, 255));
            p.drawText(text_rc, Qt::TextSingleLine | Qt::AlignLeft | Qt::AlignVCenter, item.layer_name);
            //p.drawPixmap(0, 0, w/4, h/4, item.layer);

        }
        painter->drawPixmap(opt.rect.x(), opt.rect.y(), pixmap);
    }
};

LayerBrowser::LayerBrowser(QWidget *parent)
    : QListWidget(parent)
{
    setItemDelegate(new LayerBrowserItemDelegate(this));

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
