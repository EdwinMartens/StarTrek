#ifndef _PROBE
#define _PROBE

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include "types.h"
#include "Engine.h"
#include "Enterprise.h"
#include <vector>




/**
 * \class TProbe
 * Probes can be launched from the Enterprise.
 * When they hit something they will scan it and send the information to the enterprise
 * Where the information will be displayed in a probe rapport.
 * When a probe crosses the sector bounds,, a sector area will be scanned instead of an object.
 * This will be shown on the navigation screen with updated information.
 */

class TProbe:public TSprite
{
    friend class TEngine;
private:
    int m_nWait;
    TEnterprise * m_pOwner;

    void ScanObject(TSpaceObject * a_pObject);


public:

    /** \brief
     *  Constructs a probe
     * \param a_dX
     *  X start position
     * \param a_dY
     *  Y start position
     * \param a_dSpeed
     * probe starting speed
     * \param a_dAngle
     * probe angle
     * \param a_nZ
     * probe depth (Z order)
     * \return
     *
     */
	TProbe(double a_dX, double a_dY,double a_dSpeed, double a_dAngle, int a_nZ, TEnterprise * a_pOwner);
	/// default constructor
	TProbe();

    TProbe(ifstream & a_LoadStream, ID a_id);
	void Save(ofstream & a_SaveStream);

	/// Handle collision
    void DoCollision(TSprite * sprite);

	/// AI entry point
	void Do_ai();

	/// Class initialization
	static bool Init();

	/// probe drawing
	void Draw(double a_dCamX, double a_dCamY);
};
















#endif // _PROBE
