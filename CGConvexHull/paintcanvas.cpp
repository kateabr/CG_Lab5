#include "paintcanvas.h"
#include <queue>

PaintCanvas::PaintCanvas(QWidget *parent) : QFrame(parent) {
  pixmap = QPixmap(this->width(), this->height());
  pixmap.fill();
}

void PaintCanvas::setColor(QColor c) { color = c; }

QPixmap PaintCanvas::getPixmap() const { return pixmap; }

void PaintCanvas::setCurrentPrimitive(Primitives p) { currentPrimitive = p; }

void PaintCanvas::setTableModel(TableModel *tM) { tModel = tM; }

void PaintCanvas::setThickness(int val) { penThickness = val; }

void PaintCanvas::clearArea() {
  pixmap.fill();
  repaint();
}

void PaintCanvas::paintEvent(QPaintEvent *) {
  // pixmap.fill();
  for (Drawable *d : tModel->getData())
    d->draw(pixmap);
  QPainter p(this);
  p.drawPixmap(0, 0, pixmap);
}

void PaintCanvas::mousePressEvent(QMouseEvent *e) {
  curPos = e->pos();
  if (e->buttons() & Qt::LeftButton) {
    switch (currentPrimitive) {
    case Primitives::Point:
      tModel->add(new DrawablePoint(curPos));
      break;
    case Primitives::Line:
      mousePressed = true;
      break;
    case Primitives::Polygon:
      polygonTemporary.push_back(curPos);
      break;
    }
  } else if (e->buttons() & Qt::RightButton && (!polygonTemporary.empty())) {
    tModel->add(new DrawablePolygon(polygonTemporary));
    polygonTemporary.clear();
  }
  pixmap.fill();
  repaint();
}

void PaintCanvas::mouseReleaseEvent(QMouseEvent *e) {
  mousePressed = false;
  if (currentPrimitive == Primitives::Line) {
    tModel->add(new DrawableLine(curPos, e->pos()));
    pixmap.fill();
    repaint();
  }
}

void PaintCanvas::mouseMoveEvent(QMouseEvent *e) {
  if (!mousePressed)
    return;
  QPainter p(&pixmap);
  // p.setRenderHint(QPainter::Antialiasing);
  if (e->buttons() & Qt::LeftButton)
    p.setPen(
        QPen(color, penThickness, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  pixmap.fill();
  p.drawLine(curPos, e->pos());
  p.end();
  repaint();
}

void PaintCanvas::resizeEvent(QResizeEvent *) {
  pixmap = QPixmap(width(), height());
  pixmap.fill();
}
