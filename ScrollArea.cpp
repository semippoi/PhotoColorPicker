#include "ScrollArea.h"
#include "ColorPickerView.h"

#include <QTimeLine>

struct ScrollArea::Private {
    ColorPickerView *cv;
};

ScrollArea::ScrollArea(QWidget *parent) : QScrollArea(parent)
{
    pv = new Private();
}

ScrollArea::~ScrollArea()
{
    delete pv;
}

void ScrollArea::setGraphicView(ColorPickerView *cv)
{
    pv->cv = cv;
}

void ScrollArea::paintEvent(QPaintEvent *event)
{

}

void ScrollArea::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;
    _numScheduledScalings += numSteps;
    if (_numScheduledScalings * numSteps < 0) {
        _numScheduledScalings = numSteps;
    }

    QTimeLine *anim = new QTimeLine(350, this);
    anim->setUpdateInterval(20);

    connect(anim, SIGNAL(valueChanged(qreal)), SLOT(scalingTime(qreal)));
    connect(anim, SIGNAL(finished()), SLOT(animFinished()));
    anim->start();

    update();
}

void ScrollArea::animFinished()
{
    if (_numScheduledScalings > 0) {
        _numScheduledScalings--;
    } else {
        _numScheduledScalings++;
        sender()->~QObject();
    }
}

void ScrollArea::scalingTime(qreal x)
{
    qreal factor = 1.0 + qreal(_numScheduledScalings) / 300.0;
    pv->cv->scale(factor, factor);
}
