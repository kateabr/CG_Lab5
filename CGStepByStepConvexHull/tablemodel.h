#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include "drawable.h"
#include "graham.h"
#include "matrix3x2.h"
#include "primitives.h"
#include <QtWidgets>
#include <convexhull.h>

class TableModel : public QAbstractListModel {
  Q_OBJECT
public:
  explicit TableModel(QObject *parent = 0) : QAbstractListModel(parent) {}
  int rowCount(const QModelIndex &parent = QModelIndex()) const override {
    //    qDebug() << "row count " << drawables.size();
    return drawables.size();
  }

  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override {
    if (role == Qt::DisplayRole || index.row() < 0 ||
        index.row() >= drawables.size()) {
      return drawables[index.row()]->toString();
    }
    return QVariant();
  }

  QVector<Drawable *> getData() const {
    QVector<Drawable *> res;
    for (int i = 0; i < drawables.size(); ++i)
      res.push_back(drawables[i]);
    for (int i = 0; i < hull.size(); ++i)
      res.push_back(new DrawableLine(hull[i], hull[(i + 1) % hull.size()]));
    return res;
  }

  void add(Drawable *item, bool graham) {
    beginInsertRows(QModelIndex(), drawables.size(), drawables.size());
    drawables.push_back(item);
    endInsertRows();

    QModelIndex top = createIndex(drawables.size() - 1, 0, nullptr);
    QModelIndex bottom = createIndex(drawables.size() - 1, 0, nullptr);

    update(graham);

    emit dataChanged(top, bottom);
  }

  bool removeRows(int row, int count,
                  const QModelIndex &parent = QModelIndex()) override {
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    while (count--)
      delete drawables.takeAt(row);
    endRemoveRows();
    update(true);
    return true;
  }

  void update(bool graham, bool deleted = false) {
    if (drawables.size() < 3) {
      hull.clear();
      return;
    } else if (drawables.size() == 3) {
      hull.clear();
      hull.push_back(drawables[0]->getCenter());
      hull.push_back(drawables[1]->getCenter());
      hull.push_back(drawables[2]->getCenter());
    } else if (!deleted) {
      if (static_cast<DrawablePoint *>(drawables.last())
              ->insidePolygon(DrawablePolygon(hull)))
        return;
      if (!graham)
        ConvexHull::build(hull, drawables.last()->getCenter());
      else {
        hull.push_back(drawables.last()->getCenter());
        hull = Graham::buildConvexHull(hull);
      }
    } else {
      QVector<QPointF> points;
      for (int i = 0; i < drawables.size(); ++i)
        points.push_back(drawables[i]->getCenter());
      hull.clear();
      hull = Graham::buildConvexHull(points);
    }
  }

  void intersect(int ind1, int ind2) {
    if ((drawables[ind1]->whoAmI() == Primitives::Line) &&
        (drawables[ind2]->whoAmI() == Primitives::Line)) {
      QPair<bool, QPointF> res =
          drawables[ind1]->intersectsWith(drawables[ind2]);
      qDebug() << res.first;
      if (res.first) {
        drawables.push_back(new DrawablePoint(res.second));
        QModelIndex top = createIndex(drawables.size() - 2, 0, nullptr);
        QModelIndex bottom = createIndex(drawables.size() - 1, 0, nullptr);
        emit dataChanged(top, bottom);
      }
    }
  }

private:
  QVector<Drawable *> drawables;
  QVector<QPointF> hull;
};

#endif // TABLEMODEL_H
