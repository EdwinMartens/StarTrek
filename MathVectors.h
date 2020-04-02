#ifndef _MATH_VECTORS
#define _MATH_VECTORS

#include <math.h>
#include <stdlib.h>
#include <time.h>


using namespace std;

class Vector2D
{
    public:

    // Constructors
    Vector2D();
    Vector2D(double a_dX, double a_dY);

    // Copy constructor
    Vector2D(const Vector2D & a_rVector);

    // destructor
    ~Vector2D();

    // Functions
    void rotate(double angle);
    double magnitude();
    void normalise();
    double crossproduct(const Vector2D & a_rVector);
    double dotproduct(const Vector2D & a_rVector);
    void Limit(double a_dMaxMagnitude);

    // Operators
    Vector2D operator*(double a_dScalar);
    Vector2D operator/(double a_dScalar);
    Vector2D operator+(const Vector2D & a_rVector);
    Vector2D operator-(const Vector2D & a_rVector);

    Vector2D & operator+=(const Vector2D & a_Vector);
    Vector2D & operator-=(const Vector2D & a_Vector);
    Vector2D & operator*=(double a_dScalar);
    Vector2D & operator/=(double a_dScalar);

    Vector2D & operator=(const Vector2D & a_rVector);

    double GetX();
    double GetY();

    void SetX(double a_dX);
    void SetY(double a_dY);

protected:



private:
    double m_dX;
    double m_dY;
};



double Random(int a_nMin, int a_nMax);









#endif
