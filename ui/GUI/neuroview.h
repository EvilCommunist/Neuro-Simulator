#ifndef NEUROVIEW_H
#define NEUROVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include "neurolink.h"
#include "neuronode.h"
#include <QVector>

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

    QVector<QVector<NeuroNode*>> neuroNetworkVisual;

    void clear();

    NeuroNode* createNode(const QPointF &pos, LayerType lType);
    void createLink(NeuroNode *from, NeuroNode *to);

    void removeAllBoundedLinks(NeuroNode* node);
    QPointF calculateNodePos(size_t curLayer);
public:
    explicit NeuroView(QWidget *parent = nullptr);
    ~NeuroView() = default;

    // appends|pops last node in layer or layer in NN
    void addNode(size_t numLayer);
    void removeNode(size_t numLayer);
    void addLayer();
    void removeLayer();

    inline size_t getNeuroLen(){return this->neuroNetworkVisual.size();}
};

#endif // NEUROVIEW_H
