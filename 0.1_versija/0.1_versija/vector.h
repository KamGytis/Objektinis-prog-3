#ifndef VECTOR_H
#define VECTOR_H

/**
 * @file Vector.h
 * @brief Nuosavos Vector konteinerio klasės aprašas.
 *
 * Pilnavertė std::vector alternatyva, realizuojanti visus
 * Member types, Member functions ir Non-member functions pagal:
 * https://en.cppreference.com/w/cpp/container/vector
 *
 * @author Studentas
 * @version 3.0
 */

#include <memory>
#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <initializer_list>
#include <limits>
#include <cassert>

 /**
  * @brief Nuosava Vector konteinerio klasė.
  *
  * Dinaminės atminties masyvas su automatine atminties valdymu.
  * Veikia kaip pilnavertis std::vector pakaitalas.
  *
  * @tparam T Elementų tipas
  * @tparam Allocator Atminties paskirstytojas (pagal nutylėjimą std::allocator<T>)
  */
template<typename T, typename Allocator = std::allocator<T>>
class Vector {
public:
    //  Member types 
	using value_type = T; //<! Elementų tipas
	using allocator_type = Allocator; //<! Atminties paskirstytojo tipas
	using size_type = std::size_t; //<! Dydžio tipas
	using difference_type = std::ptrdiff_t; //<! Skirtumo tipas
	using reference = T&;//<! Elementų nuorodų tipas
	using const_reference = const T&; //<! Konstantiškų elementų nuorodų tipas
	using pointer = typename std::allocator_traits<Allocator>::pointer; //<! Elementų rodyklės tipas
	using const_pointer = typename std::allocator_traits<Allocator>::const_pointer; //<! Konstantiškų elementų rodyklės tipas
	using iterator = T*; //<! Elementų iteratoriaus tipas
	using const_iterator = const T*; //<! Konstantiškų elementų iteratoriaus tipas
	using reverse_iterator = std::reverse_iterator<iterator>; //<! Atvirkštinių elementų iteratoriaus tipas
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;//<! Atvirkštinių konstantiškų elementų iteratoriaus tipas

private:
    pointer   data_;      ///< Rodyklė į duomenų masyvą
    size_type size_;      ///< Dabartinis elementų skaičius
    size_type capacity_;  ///< Maksimalus galimas elementų skaičius be perskirstymo
    Allocator alloc_;     ///< Atminties paskirstytojas

    using AllocTraits = std::allocator_traits<Allocator>;

    /**
     * @brief Atlaisvina visą naudojamą atmintį.
     */
    void deallocate_all() {
        if (data_) {
            for (size_type i = 0; i < size_; ++i)
                AllocTraits::destroy(alloc_, data_ + i);
            AllocTraits::deallocate(alloc_, data_, capacity_);
            data_ = nullptr;
        }
        size_ = 0;
        capacity_ = 0;
    }
    /**
     * @brief Perskirsto atmintį naujam dydžiui.
     * @param new_cap Naujas capacity dydis
     */