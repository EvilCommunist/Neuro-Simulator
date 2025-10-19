#ifndef NEUROLINK_H
#define NEUROLINK_H

#include <QGraphicsLineItem>
#include "neuronode.h"

class NeuroLink : public QGraphicsLineItem
{
public:
    NeuroLink(NeuroNode* node1, NeuroNode* node2, QGraphicsItem* parent);
    ~NeuroLink();

    float getWeight();

    NeuroNode* getNode1() const;
    NeuroNode* getNode2() const;
    void updatePosition();

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* widget = nullptr) override;
    // QRectF boundingRect() const override;
private:
    float weight;

    NeuroNode* node1;
    NeuroNode* node2;

    void updateLine();
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
};

#endif // NEUROLINK_H
