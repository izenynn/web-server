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
};

} // namespace nstd

#if defined(OSX) && defined(nullptr)
#undef nullptr
#endif // OSX && nullptr

const nstd::nullptr_t nullptr = {};

#endif // NSTD_NULLPTRT_H_
