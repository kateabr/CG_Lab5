#ifndef CONVEXHULL_H
#define CONVEXHULL_H

#include "drawable.h"
#include "graham.h"
#include <QVector>

namespace ConvexHull {

enum class Position { Left, Right, Belongs };

void build(QVector<QPointF> &curHull, QPointF newPoint);

} // namespace ConvexHull

#endif // CONVEXHULL_H
