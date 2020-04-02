#ifndef _DIALOGPARSER
#define _DIALOGPARSER
#include <vector>
#include "Sax.h"
#include "Statement.h"
#include "Dialog.h"

class CDialogParser: public XMLParser
{
   public:
   static CDialogParser * GetInstance();
   void SetOutput(CDialog * a_pDialog);


   protected:
   static CDialogParser * pInstance;


   //! Called when start of XML file detected
   //! @param a_strScheme  passes a scheme string
   virtual void StartFile(string & a_strScheme);

    //! Called when the end
    virtual void EndFile();

    //! Called at the start of a new node
    //! @param a_strNodeName name of the new node
    //! @param a_strAttributes Attributes string, pass to ParseAttributes to get a Variable, value list
    virtual void StartNode(string & a_strNodeName, string & a_strAttributes);

    //! Called at the end of a new node
    //! @param a_strNodeName name of the new node
    //! @param a_strAttributes Attributes string, pass to ParseAttributes to get a Variable, value list
    virtual void EndNode(string & a_strNodeName, string & a_strAttributes);

    //! Called for textual content between nodes
    //! @param a_strText plain text
    virtual void Content(string & a_strText);

   private:



   CDialogParser();
   virtual ~CDialogParser();

   vector<string> Split(string a_strText);

   bool m_blReadDialog;
   CStatement * m_pReadingStatement;
   CAnswer    * m_pReadingAnswer;
   CDialog    * m_pDialog;
};



#endif // _DIALOGPARSER
