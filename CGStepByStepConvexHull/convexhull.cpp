#include "convexhull.h"

void ConvexHull::build(QVector<QPointF> &curHull, QPointF newPoint) {
  if (DrawablePoint(newPoint).insidePolygon(DrawablePolygon(curHull)))
    return;
  curHull.push_back(newPoint);
  curHull = Graham::buildConvexHull(curHull);
}
