#include "neuroview.h"
#include <QWheelEvent>

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

    resetSceneSize();
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

    resetSceneSize();
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

    resetSceneSize();
}

void NeuroView::removeLayer(){
    for(auto node : neuroNetworkVisual[neuroNetworkVisual.size()-2]){
        removeAllBoundedLinks(node);
        scene->removeItem(node);
        this->hiddenNodeCounter --;
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

    resetSceneSize();
}


void NeuroView::resetSceneSize(){
    QRectF itemsRect = scene->itemsBoundingRect();
    QRectF paddedRect = itemsRect.adjusted(-50, -50, 50, 50);
    scene->setSceneRect(paddedRect);

    this->fitInView(paddedRect, Qt::KeepAspectRatio);
}

void NeuroView::replaceWeights(QVector<QVector<QVector<float>>> weights){
    for(int i = 0; i < weights.size(); i ++){
        for(int j = 0; j < weights[i].size(); j++){
            auto links = neuroNetworkVisual[i][j]->getLinks();
            int index = 0;
            for(auto link: links){
                if(neuroNetworkVisual[i][j] == link->getNode1()){
                    if(neuroNetworkVisual[i+1].contains(link->getNode2())){
                        link->setWeight(weights[i][j][index]);
                        index++;
                    }
                }
                else if(neuroNetworkVisual[i][j] == link->getNode2()){
                    if(neuroNetworkVisual[i+1].contains(link->getNode1())){
                        link->setWeight(weights[i][j][index]);
                        index++;
                    }
                }
            }
        }
    }
}

void NeuroView::setNeuroneValues(QVector<QVector<QVector<float>>> values){
    const int ERROR_INDEX = 1, VALUE_INDEX = 0;
    for(int i = 0; i < values.size(); i++){
        for(int j = 0; j < values[i].size(); j++){
            neuroNetworkVisual[i][j]->setValue(values[i][j][ERROR_INDEX]);
            neuroNetworkVisual[i][j]->setError(values[i][j][VALUE_INDEX]);
        }
    }
}

void NeuroView::prepeare(){
    while(neuroNetworkVisual.size() > 2){
        this->removeLayer();
    }
    this->hiddenNodeCounter = 0;
    while(neuroNetworkVisual[INITIAL_INPUT].size() > 1){
        this->removeNode(INITIAL_INPUT);
    }
    while(neuroNetworkVisual[INITIAL_OUTPUT].size() > 1){
        this->removeNode(INITIAL_OUTPUT);
    }
    this->inputNodeCounter = 0;
    this->outputNodeCounter = 0;
}


//=---------------------------------_MANIPULATIONS_-------------------------------------=//
void NeuroView::wheelEvent(QWheelEvent *event){
    qreal factor = 1.1;
    if (event->angleDelta().y() > 0) {
        scale(factor, factor);
    } else {
        scale(1.0 / factor, 1.0 / factor);
    }
}
//=---------------------------------_MANIPULATIONS_-------------------------------------=//
