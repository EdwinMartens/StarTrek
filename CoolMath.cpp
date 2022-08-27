#include "CoolMath.h"


double Distance(double a_dX1,double a_dY1, double a_dX2,double a_dY2)
{
	double DX=a_dX1-a_dX2;
	double DY=a_dY1-a_dY2;
	if (DX==0) return (DY < 0) ? -DY : DY;
	else if (DY==0) return (DX < 0) ? -DX : DX;
	else return (sqrt((DX*DX)+(DY*DY)));
}

int Distance(int a_nX1,int a_nY1, int a_nX2,int a_nY2)
{
	int DX=a_nX1-a_nX2;
	int DY=a_nY1-a_nY2;
	if (DX==0) return (DY < 0) ? -DY : DY;
	else if (DY==0) return (DX < 0) ? -DX : DX;
	else return (int)(sqrt((float)(DX*DX)+(DY*DY)));
}


bool PointInTriangle(int a_nPx, int a_nPy,
                     int a_nTriPo1x, int a_nTriPo1y,
                     int a_nTriPo2x, int a_nTriPo2y,
                     int a_nTriPo3x, int a_nTriPo3y)
{
    double TellertermAlpha = ((a_nTriPo2y - a_nTriPo3y)*(a_nPx - a_nTriPo3x) + (a_nTriPo3x - a_nTriPo2x)*(a_nPy - a_nTriPo3y));
    double NoemertermAlpha = ((a_nTriPo2y - a_nTriPo3y)*(a_nTriPo1x - a_nTriPo3x) + (a_nTriPo3x - a_nTriPo2x)*(a_nTriPo1y - a_nTriPo3y));

    double TellertermBeta  = ((a_nTriPo3y - a_nTriPo1y)*(a_nPx - a_nTriPo3x) + (a_nTriPo1x - a_nTriPo3x)*(a_nPy - a_nTriPo3y));
    double NoemertermBeta  = ((a_nTriPo2y - a_nTriPo3y)*(a_nTriPo1x - a_nTriPo3x) + (a_nTriPo3x - a_nTriPo2x)*(a_nTriPo1y - a_nTriPo3y));

    double alpha = TellertermAlpha / NoemertermAlpha;
    double beta  = TellertermBeta / NoemertermBeta;
    double gamma = 1.0f - alpha - beta;

    if ((alpha>0)&&(beta >0)&& (gamma>0))
    {
        return true;
    }
    else
    {
        return false;
    }
}























