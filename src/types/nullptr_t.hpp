#ifndef WEBSERV_NULLPTRT_H_
# define WEBSERV_NULLPTRT_H_

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
} nullptr = {};

} // namespace webserv

#endif // WEBSERV_NULLPTRT_H_
