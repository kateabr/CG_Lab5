#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "matrix3x2.h"
#include "primitives.h"
#include <QtWidgets>

class Drawable {
public:
  virtual void draw(QPixmap &pixmap) const = 0;
  virtual QString toString() = 0;
  virtual void update(Matrix3x2 &tr) = 0;
  virtual QPointF getCenter() = 0;
  virtual void updateCenter(Matrix3x2 &tr) = 0;
  virtual Primitives whoAmI() const = 0;
  virtual QPair<bool, QPointF> intersectsWith(Drawable *other) = 0;

protected:
  QString pToStr(QPointF const &p) {
    return "(" + QString::number(p.x()) + ", " + QString::number(p.y()) + ")";
  }
};

class DrawablePoint : public Drawable {
private:
  QPointF center;

public:
  DrawablePoint(QPointF p) : center(p) {}
  void draw(QPixmap &pixmap) const override {
    QPainter p(&pixmap);
    p.setBrush(QBrush(Qt::black));
    p.setPen(Qt::black);
    p.drawEllipse(center, 3, 3);
  }

  void update(Matrix3x2 &tr) override { center = tr.mul(center); }

  QPointF getCenter() override { return center; }

  void updateCenter(Matrix3x2 &tr) override { center = tr.mul(center); }

  Primitives whoAmI() const override { return Primitives::Point; }

  QPair<bool, QPointF> intersectsWith(Drawable *other) override {
    return QPair<bool, QPointF>(false, QPointF(0, 0));
  }

  QString toString() override { return "Point " + pToStr(center); }
};

class DrawableLine : public Drawable {
private:
  QPointF p1, p2;

public:
  DrawableLine(QPointF pFrom, QPointF pTo) : p1(pFrom), p2(pTo) {}
  void draw(QPixmap &pixmap) const override {
    QPainter p(&pixmap);
    p.setBrush(QBrush(Qt::black));
    p.setPen(QPen(Qt::black, 2));
    p.drawLine(p1, p2);
  }

  void update(Matrix3x2 &tr) override {
    p1 = tr.mul(p1);
    p2 = tr.mul(p2);
  }

  QPointF getCenter() override {
    return QPointF(qMin(p1.x(), p2.x()) + (qAbs(p1.x() - p2.x()) / 2),
                   qMin(p1.y(), p2.y()) + (qAbs(p1.y() - p2.y()) / 2));
  }

  void updateCenter(Matrix3x2 &tr) override {
    QPointF c(getCenter());
    Matrix3x2 p(1, 0, 0, 1, -c.x(), -c.y());
    p1 = p.mul(p1);
    p2 = p.mul(p2);
    p1 = tr.mul(p1);
    p2 = tr.mul(p2);
    Matrix3x2 pp(1, 0, 0, 1, c.x(), c.y());
    p1 = pp.mul(p1);
    p2 = pp.mul(p2);
  }

  QPair<bool, QPointF> intersectsWith(Drawable *other) override {
    DrawableLine otherLine = *static_cast<DrawableLine *>(other);
    /*попарно-векторно вектор разделяющего отрезка на векторы направленные от
     * начала разделяющего отрезка к обеим точкам проверяемого отрезка.
     * разд отр : p1-p2
     * p1-other p1
     * p1-other p2
     */
    QPointF dividingV(qAbs(p1.x() - p2.x()), qAbs(p1.y() - p2.y()));
    QPointF v1(qAbs(p1.x() - otherLine.p1.x()),
               qAbs(p1.y() - otherLine.p1.y()));
    QPointF v2(qAbs(p2.x() - otherLine.p2.x()),
               qAbs(p2.y() - otherLine.p2.y()));
    float a1 = dividingV.x() * v1.y() - dividingV.y() * v1.x();
    float a2 = dividingV.x() * v2.y() - dividingV.y() * v2.x();
    if (a1 * a2 >= 0) {
      /*
       * y = m1 * x + c1
         y = m2 * x + c2
       * intersectionX = (c2 - c1) / (m1 - m2)
         intersectionY = m1 * intersectionX + c1
       */

      float m1 = (p1.y() - p2.y());
      float y1 = (p2.x() - p1.x());
      float c1 = (p1.x() * p2.y() - p2.x() * p1.y());
      float m2 = (otherLine.p1.y() - otherLine.p2.y());
      float y2 = (otherLine.p2.x() - otherLine.p1.x());
      float c2 = (otherLine.p1.x() * otherLine.p2.y() -
                  otherLine.p2.x() * otherLine.p1.y()) /
                 (otherLine.p2.x() - otherLine.p1.x());

      float intersectionX = (c2 - c1) / (m1 - m2);
      float intersectionY = m1 * intersectionX + c1;
      return QPair<bool, QPointF>(true, QPointF(intersectionX, intersectionY));
    } else
      return QPair<bool, QPointF>(false, QPointF(0, 0));
  }

  Primitives whoAmI() const override { return Primitives::Line; }

  QString toString() override {
    return "Line " + pToStr(p1) + " -- " + pToStr(p2);
  }
};

class DrawablePolygon : public Drawable {
private:
  QVector<QPointF> points;

public:
  DrawablePolygon(QVector<QPointF> ps) : points(ps) {}
  void draw(QPixmap &pixmap) const override {
    QPainter p(&pixmap);
    p.setBrush(QBrush(Qt::black));
    p.setPen(QPen(Qt::black, 2));
    for (int i = 0; i < points.size(); ++i)
      p.drawLine(points[i], points[(i + 1) % points.size()]);
  }

  void update(Matrix3x2 &tr) override {
    for (int i = 0; i < points.size(); ++i)
      points[i] = tr.mul(points[i]);
  }

  QPointF getCenter() override {
    float area = 0;
    float cX = 0;
    float cY = 0;
    for (int i = 0; i < points.size(); ++i) {
      float cur = points[i].x() * points[(i + 1) % points.size()].y() -
                  points[(i + 1) % points.size()].x() * points[i].y();
      area += cur;
      cX += (points[i].x() + points[(i + 1) % points.size()].x()) * cur;
      cY += (points[i].y() + points[(i + 1) % points.size()].y()) * cur;
    }
    area /= 2;
    cX /= 6 * area;
    cY /= 6 * area;
    return QPointF(cX, cY);
  }

  void updateCenter(Matrix3x2 &tr) override {
    QPointF c(getCenter());
    Matrix3x2 p(1, 0, 0, 1, -c.x(), -c.y());
    Matrix3x2 pp(1, 0, 0, 1, c.x(), c.y());
    for (int i = 0; i < points.size(); ++i) {
      points[i] = p.mul(points[i]);
      points[i] = tr.mul(points[i]);
      points[i] = pp.mul(points[i]);
    }
  }

  Primitives whoAmI() const override { return Primitives::Polygon; }

  QPair<bool, QPointF> intersectsWith(Drawable *other) override {
    return QPair<bool, QPointF>(false, QPointF(0, 0));
  }

  QString toString() override {
    return "Polygon (" + QString::number(points.size()) + " angles)";
  }
};

#endif // DRAWABLE_H
