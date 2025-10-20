#include "neuroview.h"

NeuroView::NeuroView(QWidget *parent)
    : QGraphicsView(parent),
    inputNodeCounter(0),
    hiddenNodeCounter(0),
    outputNodeCounter(0),
    neuroNetworkVisual({})
{
    scene = new QGraphicsScene(this);
    setScene(scene);

    setRenderHint(QPainter::Antialiasing);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    //setSceneRect(-1000, -1000, 2000, 2000);
    setDragMode(QGraphicsView::RubberBandDrag);

    auto initialInput = createNode(QPointF(50,50), INPUT);
    auto initialOutput = createNode(QPointF(250,50),OUTPUT);
    createLink(initialInput, initialOutput);
    QVector<NeuroNode*> firstLayer = {initialInput};
    QVector<NeuroNode*> lastLayer = {initialOutput};
    neuroNetworkVisual.append(firstLayer);
    neuroNetworkVisual.append(lastLayer);
}

NeuroNode* NeuroView::createNode(const QPointF &pos, LayerType lType)
{
    switch(lType){
    case INPUT: {
        NeuroNode *nNode = new NeuroNode(pos.toPoint(), QString("I%1").arg(++inputNodeCounter));
        scene->addItem(nNode);
        return nNode;
    };
    case HIDDEN: {
        NeuroNode *nNode = new NeuroNode(pos.toPoint(), QString("N%1").arg(++hiddenNodeCounter));
        scene->addItem(nNode);
        return nNode;
    };
    case OUTPUT: {
        NeuroNode *nNode = new NeuroNode(pos.toPoint(), QString("O%1").arg(++outputNodeCounter));
        scene->addItem(nNode);
        return nNode;
    };
    }
    return nullptr;
}

void NeuroView::createLink(NeuroNode *from, NeuroNode *to)
{
    NeuroLink* link = new NeuroLink(from, to);
    scene->addItem(link);
    from->addLink(link);
    to->addLink(link);
}

void NeuroView::clear(){
    inputNodeCounter = 0;
    hiddenNodeCounter = 0;
    outputNodeCounter = 0;
    const auto items = scene->items();
    for (QGraphicsItem* item : items) {
        if (dynamic_cast<NeuroLink*>(item)) {
            scene->removeItem(item);
            delete item;
        }
    }
    scene->clear();
}


void NeuroView::addNode(size_t numLayer){
    // test code________________________________________________________________________________
    float initPos = -450;
    for(int i = 0; i < 10; i++){
        auto testInputPlusOne = createNode(QPointF(50,initPos),INPUT);
        createLink(testInputPlusOne, neuroNetworkVisual[neuroNetworkVisual.size()-1][neuroNetworkVisual[neuroNetworkVisual.size()-1].size()-1]);
        neuroNetworkVisual[0].append(testInputPlusOne);
        initPos+=100;
    }
    // test code________________________________________________________________________________
}

void NeuroView::removeNode(size_t numLayer){

}

void NeuroView::addLayer(){

}

void NeuroView::removeLayer(){

}
