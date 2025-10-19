#include "neurolink.h"
#include <QPen>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

NeuroLink::NeuroLink(NeuroNode* node1, NeuroNode* node2, QGraphicsItem* parent)
    : QGraphicsLineItem(parent), node1(node1), node2(node2)
{
    setFlag(QGraphicsItem::ItemIsSelectable);
    setZValue(-1);
    setAcceptHoverEvents(true);

    setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap));

    if (node1){
        node1->addLink(this);
    }
    if (node2){
        node2->addLink(this);
    }

    updatePosition();
    weight = 0;
}

NeuroLink::~NeuroLink(){
    if (node1!=nullptr) node1->removeLink(this);
    if (node2!=nullptr) node2->removeLink(this);
}

// QRectF NeuroLink::boundingRect() const{

// }

void NeuroLink::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
        QPen pen;
        pen.setColor(Qt::black);
        pen.setWidth(2);
        painter->setPen(pen);
        painter->drawLine(line());

    if (option->state & QStyle::State_Selected) {
        QString metric = QString("Weight: %1")
        .arg(this->weight);

        QFontMetrics fm(painter->font());
        QRect textRect = fm.boundingRect(QRect(), Qt::AlignCenter, metric);
        QPointF center = (line().p1() + line().p2()) / 2;
        textRect.moveCenter(center.toPoint());

        painter->setPen(Qt::black);
        painter->setBrush(Qt::white);
        painter->drawRect(textRect.adjusted(-5, -5, 5, 5));
        painter->drawText(textRect, Qt::AlignCenter, metric);
    } else if (option->state & QStyle::State_MouseOver) {
        QString metric = QString("Weight: %1")
        .arg(this->weight);
        QFontMetrics fm(painter->font());
        QRect textRect = fm.boundingRect(QRect(), Qt::AlignCenter, metric);
        QPointF center = (line().p1() + line().p2()) / 2;
        textRect.moveCenter(center.toPoint());

        painter->setPen(Qt::black);
        painter->setBrush(Qt::white);
        painter->drawRect(textRect.adjusted(-5, -5, 5, 5));
        painter->drawText(textRect, Qt::AlignCenter, metric);
    }
}

void NeuroLink::updatePosition()
{
    prepareGeometryChange();
    updateLine();
}

void NeuroLink::updateLine()
{
    if (!node1 || !node2) return;

    prepareGeometryChange();
    QLineF newLine(node1->pos(), node2->pos());
    setLine(newLine);
}

QVariant NeuroLink::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemSelectedHasChanged) {
        QPen p = pen();
        p.setColor(isSelected() ? Qt::blue : Qt::black);
        p.setStyle(isSelected() ? Qt::DashLine : Qt::SolidLine);
        setPen(p);
    }
    return QGraphicsLineItem::itemChange(change, value);
}


NeuroNode* NeuroLink::getNode1() const { return node1; }
NeuroNode* NeuroLink::getNode2() const { return node2; }
