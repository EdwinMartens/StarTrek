#ifndef _STATEMENT
#define _STATEMENT

#include "Dialog.h"

#include <vector>
#include <string>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

using namespace std;

class CAnswer;
class CDialog;

class CStatement
{
friend class CDialog;
    public:

    static void INIT( ALLEGRO_FONT * a_pFont,
                      ALLEGRO_COLOR a_StatementColor,
                      ALLEGRO_COLOR a_AnswerColor,
                      ALLEGRO_COLOR a_AnswerColorHighlight);

    CStatement(int a_nId);
    ~CStatement();
    void AddText(string a_strText);
    void SetImage(string a_strImagePath);
    void AddText(const char * a_szText);
    void AddAnswer(CAnswer * a_pAnswer);
    void Draw();
    void ReleaseMouse();

    void OnMouseButtonDown(const ALLEGRO_MOUSE_EVENT & a_MouseEvent);
    void OnMouseButtonUp(const ALLEGRO_MOUSE_EVENT & a_MouseEvent);
    void OnMouseMove(const ALLEGRO_MOUSE_EVENT & a_MouseEvent);

    protected:



private:

int m_nId;
vector<string> m_vStatement;
vector<CAnswer *> m_vAnswer;
int m_nX;
int m_nY;
int m_nWidth;
int m_nHeight;
int m_nAnswerY;

static ALLEGRO_COLOR  clStatement;
static ALLEGRO_COLOR  clAnswer;
static ALLEGRO_COLOR  clAnswerHighlight;
static ALLEGRO_FONT * pTextFont;
static int            nFontHeight;
bool m_blMouseWasUp;

CDialog * m_pParent;
ALLEGRO_BITMAP * m_pImage;

};



class CAnswer
{
friend class CStatement;
public:
CAnswer(int a_nTo, int a_nValue);
void SetText(string a_strText);
void SetText(const char * a_szText);
void Draw(int a_nOffsetX, int a_noffsetY, ALLEGRO_FONT * a_pFont, ALLEGRO_COLOR a_AnswerColor, ALLEGRO_COLOR a_AnswerHighLight);
bool MouseOver(int a_nOffsetX, int a_noffsetY,int a_nMouseX, int a_nMouseY);

protected:

private:
  string m_strText;
  int m_nTo;
  int m_nValue;
  int m_nX;
  int m_nY;
  int m_nWidth;
  int m_nHeight;
  bool m_blHighLight;
};

#endif
