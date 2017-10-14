#ifndef CONVEXHULL_H
#define CONVEXHULL_H

#include "drawable.h"
#include "graham.h"
#include <QVector>

namespace ConvexHull {

void build(QVector<QPointF> &curHull, QPointF newPoint);

} // namespace ConvexHull

#endif // CONVEXHULL_H
