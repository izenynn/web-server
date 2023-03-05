#ifndef NSTD_UNIQUEPTR_H_
# define NSTD_UNIQUEPTR_H_

# include <cassert>
# include <algorithm>

# include "../nullptr_t.h"

namespace nstd {

template <typename Tp>
struct default_delete {
  default_delete() {}
  ~default_delete() {}

  void operator()(Tp* ptr) const {
    //assert(sizeof(Tp) > 0);
    delete ptr;
  }
};

template <typename Tp>
struct default_delete<Tp[]> {
  default_delete() {}

  void operator()(Tp* ptr) const {
    //assert(sizeof(Tp) > 0);
    delete[] ptr;
  }
};

template <typename Tp, typename Dp = default_delete<Tp> >
class unique_ptr {
  public:
    typedef Tp* pointer;
    typedef Tp element_type;
    typedef Dp deleter_type;

    // constructors
    unique_ptr() {}

    explicit unique_ptr(pointer p)
        : p_(p), d_(deleter_type()) {}

    unique_ptr(pointer p, deleter_type d)
        : p_(p), d_(d) {}

    unique_ptr(nullptr_t)
        : p_(nullptr), d_(nullptr) {}

    // move constructors (because we are in c++98 they are copy, but will behave like move constructors)
    unique_ptr(unique_ptr& u) : p_(u.release(), d_(u.get_deleter())) {}

    // destructor
    ~unique_ptr() {
      reset();
    }

    // assigment
    unique_ptr& operator=(unique_ptr& u) {
      reset(u.release());
      get_deleter() = u.get_deleter();
      return *this;
    }

    unique_ptr& operator=(nullptr_t) {
      reset();
      return *this;
    }

    // observers
    //typename element_type::type& operator*() const {
    element_type& operator*() const {
      return *get();
    }

    pointer operator->() const {
      return get();
    }

    pointer get() const {
      return p_;
    }

    deleter_type& get_deleter() {
      return d_;
    }
    const deleter_type& get_deleter() const {
      return d_;
    }

    operator bool() const {
      return get() == pointer() ? false : true;
    }

    // modifiers
    pointer release() {
      pointer p = get();
      p_ = pointer();
      return p;
    }

    void reset(pointer p = pointer()) {
      std::swap(p_, p);
      if (p != pointer()) {
        get_deleter()(p);
      }
    }

    void swap(unique_ptr& u) {
      std::swap(p_, u.p_);
      std::swap(d_, u.d_);
    }
  private:
    pointer p_;
    deleter_type d_;
};

template <typename Tp, typename Dp>
class unique_ptr<Tp[], Dp> {
  public:
    typedef Tp* pointer;
    typedef Tp element_type;
    typedef Dp deleter_type;

    // constructors
    unique_ptr() {}

    explicit unique_ptr(pointer p)
        : p_(p), d_(deleter_type()) {}

    unique_ptr(pointer p, deleter_type d)
        : p_(p), d_(d) {}

    unique_ptr(nullptr_t)
        : p_(nullptr), d_(nullptr) {}

    // move constructors (because we are in c++98 they are copy, but will behave like move constructors)
    unique_ptr(unique_ptr& u) : p_(u.release(), d_(u.get_deleter())) {}

    // destructor
    ~unique_ptr() {
      reset();
    }

    // assigment
    unique_ptr& operator=(unique_ptr& u) {
      reset(u.release());
      get_deleter() = u.get_deleter();
      return *this;
    }

    unique_ptr& operator=(nullptr_t) {
      reset();
      return *this;
    }

    // observers
    //typename element_type::type& operator[](size_t i) const {
    element_type& operator[](size_t i) const {
      return get()[i];
    }

    pointer get() const {
      return p_;
    }

    deleter_type& get_deleter() {
      return d_;
    }
    const deleter_type& get_deleter() const {
      return d_;
    }

    operator bool() const {
      return get() == pointer() ? false : true;
    }

    // modifiers
    pointer release() {
      pointer p = get();
      p_ = pointer();
      return p;
    }

    void reset(pointer p = pointer()) {
      std::swap(p_, p);
      if (p != nullptr) {
        get_deleter()(p);
      }
    }
    void reset(nullptr_t) {
      pointer p = get();
      p_ = pointer();
      if (p != nullptr) {
        get_deleter()(p);
      }
    }

    void swap(unique_ptr& u) {
      std::swap(p_, u.p_);
      std::swap(d_, u.d_);
    }
  private:
    pointer p_;
    deleter_type d_;
};

template <typename Tp, typename Dp>
inline void swap(unique_ptr<Tp, Dp>& x, unique_ptr<Tp, Dp>& y) {
  x.swap(y);
}

template <typename Tp, typename Dp, typename Up, typename Ep>
inline bool operator==(const unique_ptr<Tp, Dp>& x, const unique_ptr<Up, Ep>& y) {
  return x.get() == y.get();
}
template <typename Tp, typename Dp>
inline bool operator==(const unique_ptr<Tp, Dp>& x, nullptr_t) {
  return x.get() == nullptr;
}
template <typename Tp, typename Dp>
inline bool operator==(nullptr_t, const unique_ptr<Tp, Dp>& y) {
  return nullptr == y.get();
}

template <typename Tp, typename Dp, typename Up, typename Ep>
inline bool operator!=(const unique_ptr<Tp, Dp>& x, const unique_ptr<Up, Ep>& y) {
  return !(x.get() == y.get());
}
template <typename Tp, typename Dp>
inline bool operator!=(const unique_ptr<Tp, Dp>& x, nullptr_t) {
  return x.get() != nullptr;
}
template <typename Tp, typename Dp>
inline bool operator!=(nullptr_t, const unique_ptr<Tp, Dp>& y) {
  return nullptr != y.get();
}

template <typename Tp, typename Dp, typename Up, typename Ep>
inline bool operator<(const unique_ptr<Tp, Dp>& x, const unique_ptr<Up, Ep>& y) {
  return x.get() < y.get();
}
template <typename Tp, typename Dp, typename Up, typename Ep>
inline bool operator<=(const unique_ptr<Tp, Dp>& x, const unique_ptr<Up, Ep>& y) {
  return !(y.get() < x.get());
}
template <typename Tp, typename Dp, typename Up, typename Ep>
inline bool operator>(const unique_ptr<Tp, Dp>& x, const unique_ptr<Up, Ep>& y) {
  return y.get() < x.get();
}
template <typename Tp, typename Dp, typename Up, typename Ep>
inline bool operator>=(const unique_ptr<Tp, Dp>& x, const unique_ptr<Up, Ep>& y) {
  return !(x.get() < y.get());
}

} // namespace nstd

#endif // NSTD_UNIQUEPTR_H_
