#ifndef NEURONODE_H
#define NEURONODE_H

#include <QGraphicsItem>

class NeuroLink;

class NeuroNode : public QGraphicsItem
{
public:
    NeuroNode();
    ~NeuroNode() = default;

    float getValue();
    float getError();
    QString getName();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;
private:
    float value;
    float error;
    QString name;

    QList<NeuroLink*> links;

    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
};

#endif // NEURONODE_H
