#include "convexhull.h"
#include "graham.h"
#include "position.h"

void ConvexHull::build(QVector<QPointF> &curHull, QPointF newPoint) {
  int indLeft = -1;
  int indRight = -1;
  bool good = true;
  for (int i = 0; i < curHull.size(); ++i) {
    QPointF p1, p2;
    if (newPoint.x() > curHull[i].x()) {
      p1 = newPoint;
      p2 = curHull[i];
    } else {
      p2 = newPoint;
      p1 = curHull[i];
    }
    DrawableLine curLine(p1, p2);
    Position pos = DrawablePoint(curHull[i]).relativePosition(curLine);
    for (int j = 0; j < curHull.size(); ++j) {
      Position newPos = DrawablePoint(curHull[j]).relativePosition(curLine);
      if (newPos == Position::Belongs) {
        if (i == j)
          continue;
        else {
          if (Graham::distance(newPoint, curHull[i]) >
              Graham::distance(newPoint, curHull[j]))
            curHull.erase(curHull.begin() + j);
          else
            curHull.erase(curHull.begin() + i);
          continue;
        }
      }
      if (pos == Position::Belongs)
        pos = newPos;
      else if (newPos != pos) {
        good = false;
        break;
      }
    }
    if (good) {
      if (indRight == -1)
        indRight = i;
      else
        indLeft = i;
    }
    good = true;
  }

  if (indRight < indLeft)
    std::swap(indRight, indLeft);
  float minDistance = qMin(Graham::distance(curHull[indLeft], newPoint),
                           Graham::distance(curHull[indRight], newPoint));
  int ind = (indLeft + 1) % curHull.size();
  while ((ind != indLeft) && ((indLeft + 1) % curHull.size() != indRight)) {
    if (Graham::distance(curHull[ind], newPoint) < minDistance) {
      curHull.erase(curHull.begin() + ind);
      if (ind < indLeft)
        --indLeft;
      if (ind < indRight)
        --indRight;
    } else
      ind = (ind + 1) % curHull.size();

    ind = ind % curHull.size();
  }

  if (!indLeft) {
    if (indRight == curHull.size() - 1)
      curHull.insert(indRight + 1, newPoint);
    else
      curHull.insert(indRight, newPoint);
  } else
    curHull.insert(indRight, newPoint);
}
