#ifndef NEUROVIEW_H
#define NEUROVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>

class NeuroView : public QGraphicsView
{
    Q_OBJECT
private:
    QGraphicsScene *scene;

    uint8_t inputNodeCounter;
    uint8_t outputNodeCounter;
    uint32_t hiddenNodeCounter;
public:
    NeuroView();
};

#endif // NEUROVIEW_H
