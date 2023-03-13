#ifndef NSTD_NULLPTRT_H_
#define NSTD_NULLPTRT_H_

namespace nstd {

class nullptr_t {
  public:
    template <class T>
    operator T*() const {
      return 0;
    }
    template <class C, class T>
    operator T C::*() const {
      return 0;
    }
  private:
    void operator&() const;
  public:
	friend inline bool operator==(nullptr_t, nullptr_t) {
      return true;
    }
	friend inline bool operator!=(nullptr_t, nullptr_t) {
      return false;
    }
};

} // namespace nstd

#ifdef nullptr
#undef nullptr
#endif // nullptr

const nstd::nullptr_t nullptr = {};

#endif // NSTD_NULLPTRT_H_
