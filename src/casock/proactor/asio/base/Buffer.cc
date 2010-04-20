#include "casock/proactor/asio/base/Buffer.h"

namespace casock {
  namespace proactor {
    namespace asio {
      namespace base {
        const size_t Buffer::DEFAULT_SIZE;

        Buffer::Buffer (const unsigned int& s)
          : size (s)
        {
          buff = new char[size];
          clear ();
        }

        Buffer::Buffer (const char* buffer, const unsigned int& s)
          : size (s)
        {
          buff = new char[size];
          strncpy (buff, buffer, size);
        }

        Buffer::~Buffer ()
        {
          delete[] buff;
        }

        void Buffer::clear ()
        {
          bzero (buff, size);
        }
      }
    }
  }
}
