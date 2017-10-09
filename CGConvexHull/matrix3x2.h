#ifndef MATRIX3X2_H
#define MATRIX3X2_H

#include <QtWidgets>
#include <algorithm>

class Matrix3x2 {
public:
  Matrix3x2(float a, float b, float c, float d, float tx, float ty) {
    m[0] = a;
    m[1] = b;
    m[2] = c;
    m[3] = d;
    m[4] = tx;
    m[5] = ty;
  }

  // identity matrix
  Matrix3x2() {
    m[0] = 1;
    m[1] = 0;
    m[2] = 0;
    m[3] = 1;
    m[4] = 0;
    m[5] = 0;
  }

  Matrix3x2(const Matrix3x2 &) = default;
  Matrix3x2(Matrix3x2 &&) = default;

  Matrix3x2 &operator=(const Matrix3x2 &) = default;
  Matrix3x2 &operator=(Matrix3x2 &&) = default;

  QPointF mul(const QPointF &p) {
    QPointF res;
    res.setX(p.x() * m[0] + p.y() * m[2] + m[4]);
    res.setY(p.x() * m[1] + p.y() * m[3] + m[5]);
    return res;
  }

  Matrix3x2 &mul(const Matrix3x2 &other) {
    Matrix3x2 temp(*this);
    m[0] = temp.m[0] * other.m[0] + temp.m[1] * other.m[2];
    m[1] = temp.m[0] * other.m[1] + temp.m[1] * other.m[3];
    m[2] = temp.m[2] * other.m[0] + temp.m[3] * other.m[2];
    m[3] = temp.m[2] * other.m[1] + temp.m[3] * other.m[3];
    m[4] = temp.m[4] * other.m[0] + temp.m[5] * other.m[2] + other.m[4];
    m[5] = temp.m[4] * other.m[1] + temp.m[5] * other.m[3] + other.m[5];

    return *this;
  }

private:
  /* | a  | b  |
   * | c  | d  |
   * | tx | ty |
   */
  float m[6];
};

#endif // MATRIX3X2_H
