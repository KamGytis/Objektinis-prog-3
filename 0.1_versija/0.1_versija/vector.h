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
    void reallocate(size_type new_cap) {
        pointer new_data = AllocTraits::allocate(alloc_, new_cap);
        size_type i = 0;
        try {
            for (; i < size_; ++i)
                AllocTraits::construct(alloc_, new_data + i, std::move_if_noexcept(data_[i]));
        }
        catch (...) {
            for (size_type j = 0; j < i; ++j)
                AllocTraits::destroy(alloc_, new_data + j);
            AllocTraits::deallocate(alloc_, new_data, new_cap);
            throw;
        }
        deallocate_all();
        data_ = new_data;
        size_ = i;
        capacity_ = new_cap;
    } 
public:
    // Constructors 

    /**
     * @brief Numatytasis konstruktorius. Sukuria tuščią vektorių.
     */
    Vector() noexcept : data_(nullptr), size_(0), capacity_(0), alloc_() {}

    /**
     * @brief Konstruktorius su paskirstytoju.
     * @param alloc Naudojamas paskirstytojas
     */

    explicit Vector(const Allocator& alloc) noexcept
        : data_(nullptr), size_(0), capacity_(0), alloc_(alloc) {
    }

    /**
     * @brief Konstruktorius su dydžiu.
     * @param count Elementų skaičius
     * @param value Pradinė reikšmė (pagal nutylėjimą – numatytoji)
     * @param alloc Paskirstytojas
     */
    explicit Vector(size_type count, const T& value = T(),
        const Allocator& alloc = Allocator())
        : data_(nullptr), size_(0), capacity_(0), alloc_(alloc) {
        assign(count, value);
    }

    /**
     * @brief Konstruktorius iš iteratorių diapazono.
     * @param first Pradžios iteratorius
     * @param last  Pabaigos iteratorius
     * @param alloc Paskirstytojas
     */

    template<typename InputIt,
        typename = std::enable_if_t<std::is_base_of_v<
        std::input_iterator_tag,
        typename std::iterator_traits<InputIt>::iterator_category>>>
        Vector(InputIt first, InputIt last, const Allocator& alloc = Allocator())
        : data_(nullptr), size_(0), capacity_(0), alloc_(alloc) {
        assign(first, last);
    }

    /**
     * @brief Kopijos konstruktorius.
     * @param other Kopijuojamas vektorius
     */
    Vector(const Vector& other)
        : data_(nullptr), size_(0), capacity_(0),
        alloc_(AllocTraits::select_on_container_copy_construction(other.alloc_)) {
        assign(other.begin(), other.end());
    }

    /**
     * @brief Perkėlimo konstruktorius.
     * @param other Perkeliamas vektorius
     */

    Vector(Vector&& other) noexcept
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_),
        alloc_(std::move(other.alloc_)) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    /**
     * @brief Konstruktorius iš std::initializer_list.
     * @param il   Pradinių reikšmių sąrašas
     * @param alloc Paskirstytojas
     */
    Vector(std::initializer_list<T> il, const Allocator& alloc = Allocator())
        : data_(nullptr), size_(0), capacity_(0), alloc_(alloc) {
        assign(il.begin(), il.end());
    }

    /**
     * @brief Destruktorius. Atlaisvina visą atmintį.
     */
    ~Vector() { deallocate_all(); }

    // ===== Assignment operators =====

    /**
     * @brief Kopijos priskyrimo operatorius.
     */
    Vector& operator=(const Vector& other) {
        if (this == &other) return *this;
        if constexpr (AllocTraits::propagate_on_container_copy_assignment::value)
            alloc_ = other.alloc_;
        assign(other.begin(), other.end());
        return *this;
    }

    /**
     * @brief Perkėlimo priskyrimo operatorius.
     */