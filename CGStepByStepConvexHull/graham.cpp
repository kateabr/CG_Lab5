#include "graham.h"

Direction Graham::getDirection(const QPointF &a, const QPointF &b,
                               const QPointF &c) {
  float det =
      (b.x() - a.x()) * (c.y() - a.y()) - (b.y() - a.y()) * (c.x() - a.x());
  if (det > 0)
    return Direction::CCW;
  else if (det < 0)
    return Direction::CW;
  else
    return Direction::COLL;
}

QVector<Direction> Graham::directions(const QVector<QPointF> &points) {
  QVector<Direction> dirs;

  for (int i = 0; i < points.size() - 3; ++i)
    dirs.push_back(getDirection(points[i], points[i + 1], points[i + 2]));

  return dirs;
}

QPointF Graham::findLowestPoint(const QVector<QPointF> &points) {
  QPointF min = points[0];

  for (int i = 1; i < points.size(); ++i)
    if ((points[i].y() > min.y()) ||
        ((points[i].y() == min.y()) && (points[i].x() < min.x())))
      min = points[i];

  return min;
}

QVector<QPointF> Graham::buildConvexHull(QVector<QPointF> &points) {
  QPointF min = findLowestPoint(points);
  auto it = std::find(points.begin(), points.end(), min);
  points.erase(it);
  std::sort(points.begin(), points.end(), [&](QPointF &p1, QPointF &p2) {
    if (getDirection(p1, min, p2) == Direction::CW)
      return false;
    else
      return true;
  });

  QVector<QPointF> res{min, points[0]};

  int ind = 1;
  while (ind < points.size()) {
    if (getDirection(res[res.size() - 2], res[res.size() - 1], points[ind]) ==
        Direction::CW) {
      res.push_back(points[ind]);
      ++ind;
    } else if (getDirection(res[res.size() - 2], res[res.size() - 1],
                            points[ind]) == Direction::CCW) {
      res.pop_back();
    } else {
      if (distance(res[res.size() - 2], res[res.size() - 1]) <
          distance(res[res.size() - 2], points[ind])) {
        res.pop_back();
        res.push_back(points[ind]);
      }
      ++ind;
    }
  }

  return res;
}

float Graham::distance(QPointF p1, QPointF p2) {
  return qSqrt((p1.x() - p2.x()) * (p1.x() - p2.x()) +
               (p1.y() - p2.y()) * (p1.y() - p2.y()));
}
