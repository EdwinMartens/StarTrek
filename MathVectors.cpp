#include "MathVectors.h"

Vector2D::Vector2D()
{
    m_dX=0;
    m_dY=0;
}

// Constructors
Vector2D::Vector2D(double a_dX, double a_dY)
{
    m_dX = a_dX;
    m_dY = a_dY;
}

// Copy Constructor
Vector2D::Vector2D(const Vector2D & a_rVector)
{
  m_dX = a_rVector.m_dX;
  m_dY = a_rVector.m_dY;
}


// destructor
Vector2D::~Vector2D()
{

}

double Vector2D::GetX()
{
    return m_dX;
}

double Vector2D::GetY()
{
    return m_dY;
}

void Vector2D::SetX(double a_dX)
{
    m_dX = a_dX;
}

void Vector2D::SetY(double a_dY)
{
    m_dY=a_dY;
}

// Functions
void Vector2D::rotate(double a_dAngle)
{
    double dX = (m_dX * cos(a_dAngle)) - (m_dY * sin(a_dAngle));
    double dY = (m_dY * cos(a_dAngle)) + (m_dX * sin(a_dAngle));
    m_dX = dX;
    m_dY = dY;
}

double Vector2D::crossproduct(const Vector2D & a_rVector)
{
    return (m_dX * a_rVector.m_dY) - (m_dY * a_rVector.m_dX);
}

double Vector2D::dotproduct(const Vector2D & a_rVector)
{
    return (m_dX * a_rVector.m_dX) + (m_dY * a_rVector.m_dY);
}


double Vector2D::magnitude()
{
    return sqrt(m_dX * m_dX +m_dY * m_dY);
}

void Vector2D::normalise()
{
    double dMagnitude = magnitude();
    m_dX /= dMagnitude;
    m_dY /= dMagnitude;

}

// Operators
Vector2D Vector2D::operator*(double a_dScalar)
{
    return Vector2D(m_dX * a_dScalar, m_dY * a_dScalar);
}

Vector2D Vector2D::operator/(double a_dScalar)
{
    return Vector2D(m_dX / a_dScalar, m_dY / a_dScalar);
}

Vector2D Vector2D::operator+(const Vector2D & a_rVector)
{
    return Vector2D(m_dX + a_rVector.m_dX, m_dY + a_rVector.m_dY);
}

Vector2D Vector2D::operator-(const Vector2D & a_rVector)
{
    return Vector2D(m_dX - a_rVector.m_dX, m_dY - a_rVector.m_dY);
}

Vector2D & Vector2D::operator=(const Vector2D & a_rVector)
{
    if (&a_rVector != this)
    {
        m_dX = a_rVector.m_dX;
        m_dY = a_rVector.m_dY;
    }
    return *this;
}

Vector2D & Vector2D::operator+=(const Vector2D & a_Vector)
{
    m_dX += a_Vector.m_dX;
    m_dY += a_Vector.m_dY;

    return *this;
}


Vector2D & Vector2D::operator-=(const Vector2D & a_Vector)
{
    m_dX -= a_Vector.m_dX;
    m_dY -= a_Vector.m_dY;

    return *this;
}

Vector2D & Vector2D::operator*=(double a_dScalar)
{
     m_dX *= a_dScalar;
     m_dY *= a_dScalar;
     return * this;
}

Vector2D & Vector2D::operator/=(double a_dScalar)
{
     m_dX /= a_dScalar;
     m_dY /= a_dScalar;
     return * this;
}

void Vector2D::Limit(double a_dMaxMagnitude)
{
    if (magnitude()>a_dMaxMagnitude)
    {
        normalise();
        m_dX *= a_dMaxMagnitude;
        m_dY *= a_dMaxMagnitude;
    }

}


double Random(int a_nMin, int a_nMax)
{
    a_nMax *= 500;
    a_nMin *= 500;
    int dV = rand() % (a_nMax-a_nMin + 1) + a_nMin;

    return double(dV)/500;
}


