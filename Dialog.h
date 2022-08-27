#ifndef _DIALOG
#define _DIALOG

#include <vector>
#include <string>
#include <allegro5/allegro.h>
#include "types.h"
#include "Statement.h"


using namespace std;



class CStatement;
class CDialog
{
public:
    CDialog();
    ~CDialog();

    void EndDialog();
    void Clear();
    bool IsEmpty();
    void Draw();
    void SetResult(int a_nResult);
    int GetResult();
    void AddStatement(CStatement * a_pStatement);
    bool IsBusy();
    void SetBusy();
    void OnMouseButtonDown(const ALLEGRO_MOUSE_EVENT & a_MouseEvent);
    void OnMouseButtonUp(const ALLEGRO_MOUSE_EVENT & a_MouseEvent);
    void OnMouseMove(const ALLEGRO_MOUSE_EVENT & a_MouseEvent);
    void SelectStatement(size_t a_sIndex);

protected:

private:
    int m_nResult;
    bool m_blBusy;
    size_t m_sIndex;
    vector<CStatement * > m_vStatement;

};









#endif
