#pragma once

#ifndef __NULLPTR_T_HPP__
# define __NULLPTR_T_HPP__

/** CLASS -------------------------------------- */

namespace webserv {
  const class nullptr_t {
    public:
      template<class T>
      operator T*() const {
        return 0;
      }
      template<class C, class T>
      operator T C::*() const {
        return 0;
      }
    private:
      void operator&() const;
  } nullptr_t = {};
}

#endif /** __NULLPTR_T_HPP__ */
