#ifndef _FUNCTIONS
#define _FUNCTIONS

#include <sstream>
#include <string>
#include <exception>

using namespace std;

class EConversionException: public exception
{
  private:
  string m_strError;

  public:

  EConversionException(string a_strError)
  {
     m_strError = a_strError;
  }

  virtual const char* what() const throw()
  {
    return m_strError.c_str();
  }

  virtual ~EConversionException() throw() { }
};


template <typename T>
T StringToNumber (const string &Text )
{
	stringstream ss(Text);
	T result;
	if ( !(ss >> result) )
	{
        throw EConversionException( Text + " Is not a valid number");
	}
	return result;
}

template <typename T>
string NumberToString ( T Number )
{
	stringstream ss;
	ss << Number;
	return ss.str();
}

#endif
