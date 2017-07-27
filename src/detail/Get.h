
#ifndef GET_DETAIL_H_ 
#define GET_DETAIL_H_ 

#include <boost/mpl/vector.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <algorithm>
#include <tuple>
#include <type_traits>

#include "Log.h"


#if defined(__CUDACC__)
namespace thrust {
    template <>
    struct iterator_traits<thrust::null_type> {
        typedef thrust::null_type value_type;
        typedef thrust::null_type reference;
        typedef thrust::null_type pointer;
    };

    template <typename mpl_vector_type, typename T0, typename ... T>
    struct iterator_system<Aboria::zip_iterator<tuple_ns::tuple<T0,T...>,mpl_vector_type>> {
        typedef typename iterator_system<T0>::type type;
    };

template <typename TUPLE, typename mpl_vector_type> 
struct getter_type;

// what follows is a copy of thrust's detail/raw_reference_cast.h for Aboria's getter type
/*
namespace detail {

// specialize is_unwrappable
template <typename TUPLE, typename mpl_vector_type> 
  struct is_unwrappable<Aboria::getter_type<TUPLE,mpl_vector_type>>
    : is_unwrappable<TUPLE>
    {};

namespace raw_reference_detail
{


// recurse on tuples
template <typename mpl_vector_type, typename ... T> 
struct raw_reference_tuple_helper<
    Aboria::getter_type<tuple_ns::tuple<T ...>,mpl_vector_type>
    > {
  typedef Aboria::getter_type<
        tuple_ns::tuple<typename raw_reference_tuple_helper<T>::type ...>
        ,mpl_vector_type> type;
};

} //namespace raw_reference_detail

template <typename TUPLE, typename mpl_vector_type> 
struct raw_reference<Aboria::getter_type<TUPLE,mpl_vector_type>> {
  private:
    typedef TUPLE tuple_type;

  public:
    typedef typename eval_if<
      is_unwrappable<tuple_type>::value,
      raw_reference_detail::raw_reference_tuple_helper<tuple_type>,
      add_reference<Aboria::getter_type<TUPLE,mpl_vector_type>>
    >::type type;
};


} //namespace detail

template <typename TUPLE, typename mpl_vector_type> 
__host__ __device__
typename detail::enable_if_unwrappable<
  Aboria::getter_type<TUPLE,mpl_vector_type>,
  typename detail::raw_reference<
    Aboria::getter_type<TUPLE,mpl_vector_type>
  >::type
>::type
raw_reference_cast(Aboria::getter_type<TUPLE,mpl_vector_type> t);

namespace detail  {
namespace aboria_addition {

struct raw_reference_caster
{
  template<typename T>
  __host__ __device__
  typename detail::raw_reference<T>::type operator()(T &ref)
  {
    return thrust::raw_reference_cast(ref);
  }

  template<typename T>
  __host__ __device__
  typename detail::raw_reference<const T>::type operator()(const T &ref)
  {
    return thrust::raw_reference_cast(ref);
  }


  template <typename TUPLE, typename mpl_vector_type> 
  __host__ __device__
  typename detail::raw_reference<
    Aboria::getter_type<TUPLE,mpl_vector_type>
  >::type
  operator()(Aboria::getter_type<TUPLE,mpl_vector_type> t,
             typename enable_if<
               is_unwrappable<Aboria::getter_type<TUPLE,mpl_vector_type>>::value
             >::type * = 0)
  {
    return thrust::raw_reference_cast(t);
  }
}; // end raw_reference_caster


} //namespace aboria_addition
} //namespace detail

template <typename TUPLE, typename mpl_vector_type> 
__host__ __device__
typename detail::enable_if_unwrappable<
  Aboria::getter_type<TUPLE,mpl_vector_type>,
  typename detail::raw_reference<
    Aboria::getter_type<TUPLE,mpl_vector_type>
  >::type
>::type
raw_reference_cast(Aboria::getter_type<TUPLE,mpl_vector_type> t)
{
  thrust::detail:aboria_addition::raw_reference_caster f;

  // note that we pass raw_reference_tuple_helper, not raw_reference as the unary metafunction
  // the different way that raw_reference_tuple_helper unwraps tuples is important
  return thrust::detail::tuple_host_device_transform<detail::raw_reference_detail::raw_reference_tuple_helper>(t, f);
} // end raw_reference_cast

*/

} //namespace thrust
#endif

namespace Aboria {


namespace detail {








template <typename T>
struct is_std_getter_type {
    const static bool value = false;
    typedef std::false_type type;
};

template <typename M, typename ... T>
struct is_std_getter_type<getter_type<std::tuple<T...>,M>> {
    const static bool value = true;
    typedef std::true_type type;
};

template <typename T>
struct is_thrust_getter_type {
    const static bool value = false;
    typedef std::false_type type;
};

template <typename M, typename ... T>
struct is_thrust_getter_type<getter_type<thrust::tuple<T...>,M>> {
    const static bool value = true;
    typedef std::true_type type;
};

template<size_t I, typename ... T>
__aboria_hd_warning_disable__ 
CUDA_HOST_DEVICE
typename std::tuple_element<I,std::tuple<T...>>::type const &
get_impl(const std::tuple<T...>& arg)
{
#if defined(__CUDA_ARCH__)
    ERROR_CUDA("Cannot use get_impl on `std::tuple` in device code");
    return std::get<I>(arg);
#else
    return std::get<I>(arg);
#endif
}

template<size_t I, typename ... T>
__aboria_hd_warning_disable__ 
CUDA_HOST_DEVICE
typename std::tuple_element<I,std::tuple<T...>>::type &
get_impl(std::tuple<T...>& arg)
{
#if defined(__CUDA_ARCH__)
    ERROR_CUDA("Cannot use get_impl on `std::tuple` in device code");
    return std::get<I>(arg);
#else
    return std::get<I>(arg);
#endif
}

template<size_t I, typename ... T>
__aboria_hd_warning_disable__ 
CUDA_HOST_DEVICE
typename std::tuple_element<I,std::tuple<T...>>::type &
get_impl(std::tuple<T...>&& arg)
{
#if defined(__CUDA_ARCH__)
    ERROR_CUDA("Cannot use get_impl on `std::tuple` in device code");
    return std::get<I>(arg);
#else
    return std::get<I>(arg);
#endif
}

template<size_t I, typename ... T>
CUDA_HOST_DEVICE
typename thrust::tuple_element<I,thrust::tuple<T...>>::type const &
get_impl(const thrust::tuple<T...>& arg) {
    return thrust::get<I>(arg);
}

template<size_t I, typename ... T>
CUDA_HOST_DEVICE
typename thrust::tuple_element<I,thrust::tuple<T...>>::type &
get_impl(thrust::tuple<T...>& arg) {
    return thrust::get<I>(arg);
}

template<size_t I, typename ... T>
CUDA_HOST_DEVICE
typename thrust::tuple_element<I,thrust::tuple<T...>>::type &
get_impl(thrust::tuple<T...>&& arg) {
    return thrust::get<I>(arg);
}




template<typename IteratorTuple, typename MplVector>
struct zip_iterator_base {};

template <typename mpl_vector_type, typename ... Types> 
struct zip_iterator_base<std::tuple<Types...>, mpl_vector_type>{
    typedef std::tuple<Types...> iterator_tuple_type;
 
    typedef getter_type<typename zip_helper<iterator_tuple_type>::tuple_value_type,mpl_vector_type> value_type;
    typedef getter_type<typename zip_helper<iterator_tuple_type>::tuple_reference,mpl_vector_type> reference;
  
 public:
  
typedef boost::iterator_facade<
    zip_iterator<iterator_tuple_type,mpl_vector_type>,
    value_type,  
    typename zip_helper<iterator_tuple_type>::iterator_category,
    reference,
    typename zip_helper<iterator_tuple_type>::difference_type
> type;

}; // end zip_iterator_base

#ifdef __aboria_have_thrust__
template <typename mpl_vector_type, typename ... Types> 
struct zip_iterator_base<thrust::tuple<Types...>, mpl_vector_type>{
    typedef thrust::tuple<Types...> iterator_tuple_type;
 
    typedef getter_type<typename zip_helper<iterator_tuple_type>::tuple_value_type,mpl_vector_type> value_type;
    typedef getter_type<typename zip_helper<iterator_tuple_type>::tuple_reference,mpl_vector_type> reference;
  
 public:
  
typedef thrust::iterator_facade<
        zip_iterator<iterator_tuple_type,mpl_vector_type>,
        value_type,  
        typename zip_helper<iterator_tuple_type>::system,
        typename zip_helper<iterator_tuple_type>::iterator_category,
        reference,
        typename zip_helper<iterator_tuple_type>::difference_type
    > type;


}; // end zip_iterator_base
#endif






__aboria_hd_warning_disable__
template<typename pointer, typename tuple_type, std::size_t... I>
CUDA_HOST_DEVICE
static pointer make_pointer(tuple_type&& tuple, index_sequence<I...>) {
    return pointer(&(get_impl<I>(std::forward(tuple)))...);
}


template <typename ZipIterator, std::size_t... I>
typename ZipIterator::tuple_raw_pointer 
iterator_to_raw_pointer_impl(const ZipIterator& arg, index_sequence<I...>) {
#ifdef __aboria_have_thrust__
    return typename ZipIterator::tuple_raw_pointer(thrust::raw_pointer_cast(&*get_impl<I>(arg.get_tuple()))...);
#else
    return typename ZipIterator::tuple_raw_pointer(&*get_impl<I>(arg.get_tuple())...);
#endif
}
    
template <typename iterator_tuple_type, typename mpl_vector_type>
typename zip_iterator<iterator_tuple_type,mpl_vector_type>::tuple_raw_pointer
iterator_to_raw_pointer(const zip_iterator<iterator_tuple_type,mpl_vector_type>& arg, std::true_type) {
    typedef typename zip_helper<iterator_tuple_type>::index_type index_type;
    return iterator_to_raw_pointer_impl(arg,index_type());
}

template <typename Iterator>
typename std::iterator_traits<Iterator>::value_type*
iterator_to_raw_pointer(const Iterator& arg, std::false_type) {
#ifdef __aboria_have_thrust__
    return thrust::raw_pointer_cast(&*arg);
#else
    return &*arg;
#endif
}


template <typename T>
struct is_zip_iterator {
    typedef std::false_type type;
    static const bool value = false; 
};

template <typename tuple_type, typename mpl_vector_type>
struct is_zip_iterator<zip_iterator<tuple_type,mpl_vector_type>> {
    typedef std::true_type type;
    static const bool value = true; 
};



}
}



#endif //GET_DETAIL_H_
