#include "neuronode.h"
#include "neurolink.h"
#include <QPen>
#include <QPainter>
#include <QStyleOptionGraphicsItem>


NeuroNode::NeuroNode(QGraphicsItem *parent)
    : QGraphicsItem(parent),
    size(70, 70){
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
}

QRectF NeuroNode::boundingRect() const{
    return QRectF(0, 0, size.width(), size.height());
}

void NeuroNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->save();

    QRectF rect = boundingRect();
    qreal size = rect.width();
    QRadialGradient gradient(0, 0, size/2);
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, Qt::lightGray);

    QBrush circleBrush(gradient);
    QPen circlePen(Qt::darkGray, 2);

    if (option->state & QStyle::State_Selected) {
        circlePen.setColor(Qt::blue);
        circlePen.setStyle(Qt::DashLine);
        painter->setPen(QPen(Qt::blue, 1, Qt::DashLine));
        painter->setBrush(Qt::NoBrush);
        painter->drawEllipse(-size/2 - 3, -size/2 - 3, size + 6, size + 6);
    }
    painter->setBrush(circleBrush);
    painter->setPen(circlePen);
    painter->drawEllipse(-size/2, -size/2, size, size);
    painter->setPen(Qt::black);
    QFont font = painter->font();
    font.setPointSize(9);
    font.setBold(true);
    painter->setFont(font);
    QRectF textRect(-size/2, -size/2, size, size);
    painter->drawText(textRect, Qt::AlignCenter, getName());
    painter->restore();
}


QVariant NeuroNode::itemChange(GraphicsItemChange change, const QVariant &value){
    if (change == ItemPositionChange && scene()) {
        QPointF newPos = value.toPointF();
        updateLinks();
        return newPos;
    }
    return QGraphicsItem::itemChange(change, value);
}

void NeuroNode::updateLinks()
{
    for (NeuroLink* link : links) {
        link->updatePosition();
    }
}
void NeuroNode::addLink(NeuroLink* link) { links.append(link); }
void NeuroNode::removeLink(NeuroLink* link) {
    if (!scene()) return;
    if(links.empty())
        return;
    else links.removeAll(link);
}

void NeuroNode::setName (const QString &name) {this->name=name; update();}
