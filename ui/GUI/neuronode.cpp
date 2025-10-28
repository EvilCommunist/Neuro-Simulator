#include "neuronode.h"
#include "neurolink.h"
#include <QPen>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

// aliases for layer types
#define INP 'I'
#define OUT 'O'
#define HID 'N'



NeuroNode::NeuroNode(QPoint position, const QString& name, QGraphicsItem *parent)
    : QGraphicsItem(parent), value(0), error(0), name(name), size(70, 70){
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setName(name);
    setPos(position);
}

QRectF NeuroNode::boundingRect() const{
    return QRectF(-size.width()/2, -size.height()/2, size.width(), size.height());
}

void NeuroNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->save();

    QRectF rect = boundingRect();
    qreal size = rect.width();
    QRadialGradient gradient(0, 0, size/2);
    gradient.setColorAt(0, Qt::white);
    switch(name[0].toLatin1()){
    case INP: gradient.setColorAt(1, Qt::red); break;
    case HID: gradient.setColorAt(1, Qt::yellow); break;
    case OUT: gradient.setColorAt(1, Qt::green); break;
    default: gradient.setColorAt(1, Qt::lightGray); break; // in case it needed
    }

    QBrush circleBrush(gradient);
    QPen circlePen(Qt::darkGray, 2);

    painter->setBrush(circleBrush);
    painter->setPen(circlePen);
    painter->drawEllipse(rect);

    painter->setPen(Qt::black);
    QFont font = painter->font();
    font.setPointSize(9);
    font.setBold(true);
    painter->setFont(font);
    painter->drawText(rect, Qt::AlignCenter, getName());

    if (option->state & QStyle::State_Selected) {
        circlePen.setColor(Qt::blue);
        circlePen.setStyle(Qt::DashLine);
        painter->setPen(QPen(Qt::blue, 1, Qt::DashLine));
        painter->setBrush(Qt::NoBrush);
        painter->drawEllipse(rect.adjusted(-3, -3, 3, 3));

        QString metrics = QString("Value: %1\nError: %2")
                              .arg(this->getValue()).arg(this->getError());
        QFontMetrics fm(painter->font());
        QRect textRect = fm.boundingRect(QRect(), Qt::AlignCenter, metrics);
        QPointF metricsPos(rect.right() - textRect.width() / 2,
                           rect.bottom() + textRect.height() / 4);
        textRect.moveCenter(metricsPos.toPoint());

        painter->setPen(Qt::black);
        painter->setBrush(Qt::white);
        painter->drawRect(textRect.adjusted(-5, -5, 5, 5));
        painter->drawText(textRect, Qt::AlignCenter, metrics);
    } else if (option->state & QStyle::State_MouseOver) {
        circlePen.setColor(Qt::blue);
        circlePen.setStyle(Qt::DashLine);
        painter->setPen(QPen(Qt::blue, 1, Qt::DashLine));
        painter->setBrush(Qt::NoBrush);
        painter->drawEllipse(rect.adjusted(-3, -3, 3, 3));

        QString metrics = QString("Value: %1\nError: %2")
                              .arg(this->getValue()).arg(this->getError());
        QFontMetrics fm(painter->font());
        QRect textRect = fm.boundingRect(QRect(), Qt::AlignCenter, metrics);
        QPointF metricsPos(rect.right() - textRect.width() / 2,
                            rect.bottom() - textRect.height() / 2);
        textRect.moveCenter(metricsPos.toPoint());

        painter->setPen(Qt::black);
        painter->setBrush(Qt::white);
        painter->drawRect(textRect.adjusted(-5, -5, 5, 5));
        painter->drawText(textRect, Qt::AlignCenter, metrics);
    }
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
