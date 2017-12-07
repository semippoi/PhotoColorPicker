#ifndef SCROLLAREA_H
#define SCROLLAREA_H

#include <QWidget>
#include <QScrollArea>

class ColorPickerView;

class ScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    ScrollArea(QWidget *parent);
    ~ScrollArea();
private:
    double _numScheduledScalings;
    struct Private;
    Private *pv;
public:
    void setGraphicView(ColorPickerView *cv);

    virtual void paintEvent(QPaintEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
private slots:
    void animFinished();
    void scalingTime(qreal x);

};

#endif // SCROLLAREA_H
