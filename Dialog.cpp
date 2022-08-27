#include "Dialog.h"


extern SCREEN_MODE g_nScreenMode;

CDialog::CDialog()
{
   m_sIndex = 0;
   m_blBusy = false;
}

CDialog::~CDialog()
{
    Clear();
}

void CDialog::SetResult(int a_nResult)
{
    m_nResult = a_nResult;
}

int CDialog::GetResult()
{
    return m_nResult;
}


void CDialog::EndDialog()
{
    Clear();
    g_nScreenMode = MODE_MAINSCREEN;
}

void CDialog::OnMouseButtonDown(const ALLEGRO_MOUSE_EVENT & a_MouseEvent)
{
   if (m_sIndex < m_vStatement.size())
   {
       m_vStatement[m_sIndex]->OnMouseButtonDown(a_MouseEvent);
   }

}

void CDialog::SelectStatement(size_t a_sIndex)
{
    m_sIndex = a_sIndex;
}

void CDialog::OnMouseMove(const ALLEGRO_MOUSE_EVENT & a_MouseEvent)
{
   if (m_sIndex < m_vStatement.size())
   {
       m_vStatement[m_sIndex]->OnMouseMove(a_MouseEvent);
   }
}

void CDialog::SetBusy()
{
    m_blBusy = true;
}

bool CDialog::IsBusy()
{
    return m_blBusy;
}

void CDialog::Clear()
{
    vector<CStatement * >::iterator p = m_vStatement.begin();
    while (p != m_vStatement.end())
    {
        CStatement * pStatement = *p;
        delete pStatement;
        p = m_vStatement.erase(p);
    }
    m_sIndex =0;
    m_blBusy = false;
}

bool CDialog::IsEmpty()
{
    if (m_vStatement.size() == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void CDialog::AddStatement(CStatement * a_pStatement)
{
    if ( a_pStatement != NULL)
    {
        a_pStatement->m_pParent = this;
        m_vStatement.push_back(a_pStatement);
    }
}


void CDialog::Draw()
{
   if (m_sIndex < m_vStatement.size())
   {
       m_vStatement[m_sIndex]->Draw();
   }
}
