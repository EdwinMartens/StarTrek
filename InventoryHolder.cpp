#include "InventoryHolder.h"


CInventoryHolder::CInventoryHolder()
{
    m_blHasInventoryItem = false;
    m_InventoryItem.m_blValid = false;
}

CInventoryHolder::CInventoryHolder(ifstream & a_LoadStream)
{
    a_LoadStream.read((char*)& m_blHasInventoryItem, sizeof (m_blHasInventoryItem));
    a_LoadStream.read((char*)& m_InventoryItem, sizeof (TInventoryItem));
    #ifdef _DEBUG
    std::cout << "--Inventory Loaded\n";
    #endif // _DEBUG
}

CInventoryHolder::~CInventoryHolder()
{


}

void CInventoryHolder::Save(ofstream & a_SaveStream)
{
    a_SaveStream.write((char*)& m_blHasInventoryItem, sizeof (m_blHasInventoryItem));
    a_SaveStream.write((char*)& m_InventoryItem, sizeof (TInventoryItem));
    #ifdef _DEBUG
    std::cout << "--Inventory Saved\n";
    #endif // _DEBUG
}

bool CInventoryHolder::HasInventoryItem()
{
    return m_blHasInventoryItem;
}

TInventoryItem CInventoryHolder::GetInventoryItem()
{
    return m_InventoryItem;
}

void CInventoryHolder::RemoveInventoryItem()
{
    m_blHasInventoryItem = false;
    m_InventoryItem.m_blValid = false;
}

void CInventoryHolder::SetInventoryItem(TInventoryItem item)
{
    m_InventoryItem = item;
    m_blHasInventoryItem = true;
}
