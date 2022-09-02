#ifndef _INVENTORY_HOLDER
#define _INVENTORY_HOLDER

#include "Transporter.h"


class CInventoryHolder
{
private:
    bool m_blHasInventoryItem;
	TInventoryItem m_InventoryItem;

protected:

public:

  CInventoryHolder();
  CInventoryHolder(ifstream & a_LoadStream);
  ~CInventoryHolder();

  void Save(ofstream & a_SaveStream);
  bool HasInventoryItem();
  TInventoryItem GetInventoryItem();
  void RemoveInventoryItem();
  void SetInventoryItem(TInventoryItem item);

};

#endif // _INVENTORY_HOLDER
