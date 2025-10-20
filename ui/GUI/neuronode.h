#ifndef NEURONODE_H
#define NEURONODE_H

#include <QGraphicsItem>

class NeuroLink;

class NeuroNode : public QGraphicsItem
{
public:
    NeuroNode(QPoint position, const QString& name, QGraphicsItem *parent = nullptr);
    ~NeuroNode() = default;

    inline float getValue() const {return value;}
    inline float getError() const {return error;}
    inline QString getName() const {return name;}
    void setName(const QString& name);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;
    QRectF boundingRect() const override;

    void addLink(NeuroLink* link);
    void removeLink(NeuroLink* link);
    void updateLinks();
private:
    float value;
    float error;
    QString name;
    QSizeF size;

    QList<NeuroLink*> links;

    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
};

#endif // NEURONODE_H
