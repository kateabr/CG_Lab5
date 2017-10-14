#ifndef GRAHAM_H
#define GRAHAM_H

#include "direction.h"
#include <QVector>
#include <QtWidgets>

namespace Graham {
Direction getDirection(const QPointF &a, const QPointF &b, const QPointF &c);

QVector<Direction> directions(const QVector<QPointF> &points);

QPointF findLowestPoint(const QVector<QPointF> &points);

QVector<QPointF> buildConvexHull(QVector<QPointF> &points);

float distance(QPointF p1, QPointF p2);
} // namespace Graham

#endif // GRAHAM_H
