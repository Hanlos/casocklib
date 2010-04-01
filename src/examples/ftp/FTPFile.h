#ifndef __CASOCKLIB__EXAMPLES_FTP_FTP_FILE_H_
#define __CASOCKLIB__EXAMPLES_FTP_FTP_FILE_H_

#include <string>
#include <sstream>
#include <fstream>

class FTPFile
{
  public:
    FTPFile (const std::string& name) : mName (name) , mSize (0), mBuffer (NULL) { }

    void setSize (size_t& size) { mSize = size; }
    void setBuffer (char* buffer) { mBuffer = buffer; }

    const char* getBuffer () const { return mBuffer; }
    const size_t& getSize () const { return mSize; }

    void save ()
    {
      std::ofstream os (mName.c_str (), std::ios::binary);
      os.write (mBuffer, mSize);
      os.close ();
    }

    const string toString ()
    {
      std::stringstream ss;

      ss << "name [";
      ss << mName;
      ss << "], size [";
      ss << mSize;
      ss << "]";

      return ss.str ();
    }

  private:
    std::string mName;
    size_t      mSize;
    char*       mBuffer;
};

#endif // #define __CASOCKLIB__EXAMPLES_FTP_FTP_FILE_H_
