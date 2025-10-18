#ifndef NEUROLINK_H
#define NEUROLINK_H

#include <QGraphicsLineItem>
#include "neuronode.h"

class NeuroLink : public QGraphicsLineItem
{
public:
    NeuroLink();
    ~NeuroLink() = default;

    float getWeight();

    NeuroNode* getNode1() const;
    NeuroNode* getNode2() const;

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* widget = nullptr) override;
    QRectF boundingRect() const override;
private:
    float weight;

    NeuroNode* node1;
    NeuroNode* node2;

    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
};

#endif // NEUROLINK_H
