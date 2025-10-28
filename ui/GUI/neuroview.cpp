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

    auto initialInput = createNode(QPointF(0,0), INPUT);
    auto initialOutput = createNode(QPointF(200,0),OUTPUT);
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
    auto index = INPUT;
    if(numLayer==0){ // does no changes
    } else if(numLayer == neuroNetworkVisual.size()-1){
        index = OUTPUT;
    }else{
        index = HIDDEN;
    }
    auto newNode = createNode(QPointF(0,0),index);
    neuroNetworkVisual[numLayer].append(newNode);
    newNode->setPos(calculateNodePos(numLayer));
    resetLayerNodesPos(numLayer);
    if(index == INPUT){
        for(int i = 0; i < neuroNetworkVisual[numLayer+1].size(); i++){
            createLink(newNode, neuroNetworkVisual[numLayer+1][i]);
        }
    }else if(index == HIDDEN){
        for(int i = 0; i < neuroNetworkVisual[numLayer+1].size(); i++){
            createLink(newNode, neuroNetworkVisual[numLayer+1][i]);
        }
        for(int i = 0; i < neuroNetworkVisual[numLayer-1].size(); i++){
            createLink(newNode, neuroNetworkVisual[numLayer-1][i]);
        }
    }else{
        for(int i = 0; i < neuroNetworkVisual[numLayer-1].size(); i++){
            createLink(newNode, neuroNetworkVisual[numLayer-1][i]);
        }
    }
}

void NeuroView::removeNode(size_t numLayer){
    auto curNode = neuroNetworkVisual[numLayer][neuroNetworkVisual[numLayer].size()-1];
    neuroNetworkVisual[numLayer].pop_back();
    removeAllBoundedLinks(curNode);
    scene->removeItem(curNode);
    delete curNode;
    auto index = INPUT;
    if(numLayer==0){ // does no changes
    } else if(numLayer == neuroNetworkVisual.size()-1){
        index = OUTPUT;
    }else{
        index = HIDDEN;
    }
    resetLayerNodesPos(numLayer);
    switch(index){
    case INPUT: inputNodeCounter--; break;
    case HIDDEN: hiddenNodeCounter--; break;
    case OUTPUT: outputNodeCounter--; break;
    }

}

void NeuroView::removeAllBoundedLinks(NeuroNode* node){
    for (QGraphicsItem* item : scene->items()) {
        if (auto* link = dynamic_cast<NeuroLink*>(item)) {
            if(link->getNode1() == node || link->getNode2() == node){
                scene->removeItem(link);
                delete link;
            }
        }
    }
}

QPointF NeuroView::calculateNodePos(size_t curLayer){
    qreal x=static_cast<qreal>(curLayer*XSPACE), y=0;
    return QPointF(x, y);
}

void NeuroView::resetLayerNodesPos(size_t numLayer){
    //resetting Y pos for each layer node
    qreal newY = -((neuroNetworkVisual[numLayer].size()-1)*YSPACE)/2;
    for(auto node : neuroNetworkVisual[numLayer]){
        auto pos = node->pos();
        pos.setY(newY);
        node->setPos(pos);
        newY+=YSPACE;
        auto boundedLinks = node->getLinks();
        for(auto link: boundedLinks){
            link->updatePosition();
        }
    }
}

void NeuroView::addLayer(){
    for(auto node : neuroNetworkVisual[neuroNetworkVisual.size()-1]){
        removeAllBoundedLinks(node);
        auto pos = node->pos();
        pos.setX(pos.x()+XSPACE);
        node->setPos(pos);
    }
    auto newLayerNode = createNode(QPointF(0,0),HIDDEN);
    neuroNetworkVisual.insert(neuroNetworkVisual.size()-1, QVector<NeuroNode*>{newLayerNode});
    newLayerNode->setPos(calculateNodePos(neuroNetworkVisual.size()-2));
    for(int i = 0; i < neuroNetworkVisual[neuroNetworkVisual.size()-1].size(); i++){
        createLink(newLayerNode, neuroNetworkVisual[neuroNetworkVisual.size()-1][i]);
    }
    for(int i = 0; i < neuroNetworkVisual[neuroNetworkVisual.size()-3].size(); i++){
        createLink(newLayerNode, neuroNetworkVisual[neuroNetworkVisual.size()-3][i]);
    }
}

void NeuroView::removeLayer(){
    for(auto node : neuroNetworkVisual[neuroNetworkVisual.size()-2]){
        removeAllBoundedLinks(node);
        scene->removeItem(node);
        delete node;
    }
    for(auto node : neuroNetworkVisual[neuroNetworkVisual.size()-1]){
        removeAllBoundedLinks(node);
        auto pos = node->pos();
        pos.setX(pos.x()-XSPACE);
        node->setPos(pos);
    }
    neuroNetworkVisual.removeAt(neuroNetworkVisual.size()-2);
    for(int i = 0; i < neuroNetworkVisual[neuroNetworkVisual.size()-1].size(); i++){
        for(int j = 0; j < neuroNetworkVisual[neuroNetworkVisual.size()-2].size(); j++){
            createLink(neuroNetworkVisual[neuroNetworkVisual.size()-2][j], neuroNetworkVisual[neuroNetworkVisual.size()-1][i]);
        }
    }
}
