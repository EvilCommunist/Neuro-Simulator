#ifndef NEUROVIEW_H
#define NEUROVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include "neurolink.h"
#include "neuronode.h"

enum LayerType{
    INPUT,
    HIDDEN,
    OUTPUT
};

class NeuroView : public QGraphicsView
{
    Q_OBJECT
private:
    QGraphicsScene *scene;

    uint8_t inputNodeCounter;
    uint8_t outputNodeCounter;
    uint32_t hiddenNodeCounter;

    void clear();

    NeuroNode* createNode(const QPointF &pos, LayerType lType);
    void createLink(NeuroNode *from, NeuroNode *to);
public:
    explicit NeuroView(QWidget *parent = nullptr);
    ~NeuroView() = default;
};

#endif // NEUROVIEW_H
