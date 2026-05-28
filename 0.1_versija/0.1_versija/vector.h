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

    //  Priskyrimo operatoriai

    /**
     * @brief Kopijos priskyrimo operatorius.
     */
    Vector& operator=(const Vector& other) {
        if (this == &other) return *this;
        if constexpr (AllocTraits::propagate_on_container_copy_assignment::value)
            alloc_ = other.alloc_; // <- Čia ištaisyta klaida (buvo alloc = ...)
        assign(other.begin(), other.end());
        return *this;
    }

    /**
     * @brief Perkėlimo priskyrimo operatorius.
     */

    Vector& operator=(Vector&& other) noexcept {
        if (this == &other) return *this;
        deallocate_all();
        if constexpr (AllocTraits::propagate_on_container_move_assignment::value)
            alloc_ = std::move(other.alloc_);
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
        return *this;
    }

    /**
     * @brief Priskyrimo operatorius iš initializer_list.
     */
    Vector& operator=(std::initializer_list<T> il) {
        assign(il.begin(), il.end());
        return *this;
    }

    // priskyrimo funkcijos

    /**
     * @brief Priskiria count kopijų reikšmės value.
     * @param count Elementų skaičius
     * @param value Reikšmė
     */
    void assign(size_type count, const T& value) {
        clear();
        reserve(count);
        for (size_type i = 0; i < count; ++i)
            AllocTraits::construct(alloc_, data_ + i, value);
        size_ = count;
    }

    /**
     * @brief Priskiria elementus iš iteratorių diapazono.
     */
    template<typename InputIt,
        typename = std::enable_if_t<std::is_base_of_v<
        std::input_iterator_tag,
        typename std::iterator_traits<InputIt>::iterator_category>>>
        void assign(InputIt first, InputIt last) {
        clear();
        for (auto it = first; it != last; ++it)
            push_back(*it);
    }

    /**
     * @brief Priskiria elementus iš initializer_list.
     */
    void assign(std::initializer_list<T> il) {
        assign(il.begin(), il.end());
    }

    /**
     * @brief Grąžina paskirstytojo kopiją.
     */
    allocator_type get_allocator() const noexcept { return alloc_; }

    // element access

    /**
     * @brief Prieiga prie elemento su ribų tikrinimu.
     * @throws std::out_of_range jei pos >= size()
     */
    reference at(size_type pos) {
        if (pos >= size_) throw std::out_of_range("Vector::at - indeksas uz ribu");
        return data_[pos];
    }

    /**
     * @brief Prieiga prie elemento su ribų tikrinimu (const).
     */
    const_reference at(size_type pos) const {
        if (pos >= size_) throw std::out_of_range("Vector::at - indeksas uz ribu");
        return data_[pos];
    }

    /**
     * @brief Prieiga prie elemento pagal indeksą (be tikrinimo).
     */
    reference operator[](size_type pos) { return data_[pos]; }

    /**
     * @brief Prieiga prie elemento pagal indeksą (be tikrinimo, const).
     */
    const_reference operator[](size_type pos) const { return data_[pos]; }

    /**
     * @brief Grąžina pirmą elementą.
     */
    reference front() { return data_[0]; }
    const_reference front() const { return data_[0]; }

    /**
     * @brief Grąžina paskutinį elementą.
     */
    reference back() { return data_[size_ - 1]; }
    const_reference back() const { return data_[size_ - 1]; }

    /**
     * @brief Grąžina rodyklę į vidinį duomenų masyvą.
     */
    T* data() noexcept { return data_; }
    const T* data() const noexcept { return data_; }

    // Iteratoriai

    /** @brief Grąžina iteratorių į pradžią. */
    iterator begin() noexcept { return data_; }
    const_iterator begin() const noexcept { return data_; }
    const_iterator cbegin() const noexcept { return data_; }

    /** @brief Grąžina iteratorių į pabaigą (past-the-end). */
    iterator end() noexcept { return data_ + size_; }
    const_iterator end() const noexcept { return data_ + size_; }
    const_iterator cend() const noexcept { return data_ + size_; }

    /** @brief Grąžina atvirkštinį iteratorių. */
    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }

    /** @brief Grąžina atvirkštinį past-the-end iteratorių. */
    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
    const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin()); }

    // Talpa ir dydis

    /** @brief Tikrina ar vektorius tuščias. */
    [[nodiscard]] bool empty() const noexcept { return size_ == 0; }

    /** @brief Grąžina elementų skaičių. */
    size_type size() const noexcept { return size_; }

    /** @brief Grąžina maksimalų galimą elementų skaičių. */
    size_type max_size() const noexcept {
        return AllocTraits::max_size(alloc_);
    }

    /**
     * @brief Rezervuoja atmintį new_cap elementams.
     * @param new_cap Naujas capacity
     * @throws std::length_error jei new_cap > max_size()
     */
    void reserve(size_type new_cap) {
        if (new_cap <= capacity_) return;
        if (new_cap > max_size())
            throw std::length_error("Vector::reserve - per didelis dydis");
        reallocate(new_cap);
    }

    /** @brief Grąžina dabartiną capacity. */
    size_type capacity() const noexcept { return capacity_; }

    /**
     * @brief Sumažina capacity iki size (shrink to fit).
     */
    void shrink_to_fit() {
        if (size_ < capacity_)
            reallocate(size_);
    }

    // Modifikacija

    /** @brief Ištrina visus elementus (size = 0, capacity nesikeičia). */
    void clear() noexcept {
        for (size_type i = 0; i < size_; ++i)
            AllocTraits::destroy(alloc_, data_ + i);
        size_ = 0;
    }

    /**
     * @brief Įterpia elementą prieš pos.
     * @param pos   Iteratorius į vietą
     * @param value Įterpiama reikšmė
     * @return Iteratorius į įterptą elementą
     */
    iterator insert(const_iterator pos, const T& value) {
        return emplace(pos, value);
    }

    iterator insert(const_iterator pos, T&& value) {
        return emplace(pos, std::move(value));
    }

    /**
     * @brief Įterpia count kopijų reikšmės prieš pos.
     */
    iterator insert(const_iterator pos, size_type count, const T& value) {
        size_type idx = pos - cbegin();
        if (count == 0) return begin() + idx;
        if (size_ + count > capacity_) {
            size_type new_cap = std::max(size_ + count, capacity_ * 2);
            reallocate(new_cap);
        }

        for (size_type i = size_; i > idx; --i) {
            size_type old_idx = i - 1;
            size_type new_idx = old_idx + count;
            if (new_idx >= size_) {
                AllocTraits::construct(alloc_, data_ + new_idx, std::move(data_[old_idx]));
            }
            else {
                data_[new_idx] = std::move(data_[old_idx]);
            }
        }

        size_type affected_constructed = std::min(size_, idx + count);
        for (size_type i = idx; i < affected_constructed; ++i) {
            AllocTraits::destroy(alloc_, data_ + i);
        }

        for (size_type i = 0; i < count; ++i) {
            AllocTraits::construct(alloc_, data_ + idx + i, value);
        }
        size_ += count;
        return begin() + idx;
    }

    /**
     * @brief Įterpia elementus iš diapazono prieš pos.
     */
    template<typename InputIt,
        typename = std::enable_if_t<std::is_base_of_v<
        std::input_iterator_tag,
        typename std::iterator_traits<InputIt>::iterator_category>>>
        iterator insert(const_iterator pos, InputIt first, InputIt last) {
        size_type idx = pos - cbegin();
        Vector tmp(first, last);
        size_type count = tmp.size();
        if (count == 0) return begin() + idx;
        if (size_ + count > capacity_)
            reallocate(std::max(size_ + count, capacity_ * 2));

        for (size_type i = size_; i > idx; --i) {
            size_type old_idx = i - 1;
            size_type new_idx = old_idx + count;
            if (new_idx >= size_) {
                AllocTraits::construct(alloc_, data_ + new_idx, std::move(data_[old_idx]));
            }
            else {
                data_[new_idx] = std::move(data_[old_idx]);
            }
        }

        size_type affected_constructed = std::min(size_, idx + count);
        for (size_type i = idx; i < affected_constructed; ++i) {
            AllocTraits::destroy(alloc_, data_ + i);
        }

        for (size_type i = 0; i < count; ++i)
            AllocTraits::construct(alloc_, data_ + idx + i, std::move(tmp[i]));
        size_ += count;
        return begin() + idx;
    }

    iterator insert(const_iterator pos, std::initializer_list<T> il) {
        return insert(pos, il.begin(), il.end());
    }

    /**
     * @brief Sukonstruoja elementą vietoje prieš pos.
     */
    template<typename... Args>
    iterator emplace(const_iterator pos, Args&&... args) {
        size_type idx = pos - cbegin();
        if (size_ == capacity_)
            reallocate(capacity_ == 0 ? 1 : capacity_ * 2);

        for (size_type i = size_; i > idx; --i) {
            size_type old_idx = i - 1;
            size_type new_idx = old_idx + 1;
            if (new_idx >= size_) {
                AllocTraits::construct(alloc_, data_ + new_idx, std::move(data_[old_idx]));
            }
            else {
                data_[new_idx] = std::move(data_[old_idx]);
            }
        }

        if (idx < size_) {
            AllocTraits::destroy(alloc_, data_ + idx);
        }

        AllocTraits::construct(alloc_, data_ + idx, std::forward<Args>(args)...);
        ++size_;
        return begin() + idx;
    }

    /**
     * @brief Ištrina elementą ties pos.
     * @return Iteratorius į kitą elementą po ištrintojo
     */
    iterator erase(const_iterator pos) {
        return erase(pos, pos + 1);
    }

    /**
     * @brief Ištrina elementus diapazone [first, last).
     */
    iterator erase(const_iterator first, const_iterator last) {
        iterator f = begin() + (first - cbegin());
        iterator l = begin() + (last - cbegin());
        size_type count = l - f;
        std::move(l, end(), f);
        for (size_type i = 0; i < count; ++i)
            AllocTraits::destroy(alloc_, end() - 1 - i);
        size_ -= count;
        return f;
    }

    /**
     * @brief Prideda elementą į galą (kopija).
     * @param value Pridedama reikšmė
     */
    void push_back(const T& value) {
        emplace_back(value);
    }
    /**
     * @brief Prideda elementą į galą (perkėlimas).
     */
    void push_back(T&& value) {
        emplace_back(std::move(value));
    }

    /**
     * @brief Sukonstruoja elementą gale vietoje.
     * @return Nuoroda į sukonstruotą elementą
     */
    template<typename... Args>
    reference emplace_back(Args&&... args) {
        if (size_ == capacity_)
            reallocate(capacity_ == 0 ? 1 : capacity_ * 2);
        AllocTraits::construct(alloc_, data_ + size_, std::forward<Args>(args)...);
        return data_[size_++];
    }

    /**
     * @brief Pašalina paskutinį elementą.
     */
    void pop_back() {
        AllocTraits::destroy(alloc_, data_ + --size_);
    }

    /**
     * @brief Pakeičia vektoriaus dydį.
     * @param count Naujas dydis
     * @param value Reikšmė naujiems elementams (jei didėja)
     */
    void resize(size_type count, const T& value = T()) {
        if (count < size_) {
            for (size_type i = count; i < size_; ++i)
                AllocTraits::destroy(alloc_, data_ + i);
            size_ = count;
        }
        else if (count > size_) {
            reserve(count);
            for (size_type i = size_; i < count; ++i)
                AllocTraits::construct(alloc_, data_ + i, value);
            size_ = count;
        }
    }

    /**
     * @brief Sukeičia du vektorius vietomis (O(1)).
     * @param other Kitas vektorius
     */
    void swap(Vector& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
        if constexpr (AllocTraits::propagate_on_container_swap::value)
            std::swap(alloc_, other.alloc_);
    }
};

//  Non-member functions 

/** @brief Lygybės operatorius. */
template<typename T, typename A>
bool operator==(const Vector<T, A>& lhs, const Vector<T, A>& rhs) {
    if (lhs.size() != rhs.size()) return false;
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

/** @brief Nelygybės operatorius. */
template<typename T, typename A>
bool operator!=(const Vector<T, A>& lhs, const Vector<T, A>& rhs) {
    return !(lhs == rhs);
}

/** @brief Leksikografinis < palyginimas. */
template<typename T, typename A>
bool operator<(const Vector<T, A>& lhs, const Vector<T, A>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
        rhs.begin(), rhs.end());
}

/** @brief Leksikografinis <= palyginimas. */
template<typename T, typename A>
bool operator<=(const Vector<T, A>& lhs, const Vector<T, A>& rhs) {
    return !(rhs < lhs);
}

/** @brief Leksikografinis > palyginimas. */
template<typename T, typename A>
bool operator>(const Vector<T, A>& lhs, const Vector<T, A>& rhs) {
    return rhs < lhs;
}

/** @brief Leksikografinis >= palyginimas. */
template<typename T, typename A>
bool operator>=(const Vector<T, A>& lhs, const Vector<T, A>& rhs) {
    return !(lhs < rhs);
}

/** @brief Globalus swap (ADL). */
template<typename T, typename A>
void swap(Vector<T, A>& lhs, Vector<T, A>& rhs) noexcept {
    lhs.swap(rhs);
}

/** @brief Ištrina elementus pagal reikšmę (C++20 stilius). */
template<typename T, typename A, typename U>
typename Vector<T, A>::size_type erase(Vector<T, A>& c, const U& value) {
    auto it = std::remove(c.begin(), c.end(), value);
    auto n = c.end() - it;
    c.erase(it, c.end());
    return static_cast<typename Vector<T, A>::size_type>(n);
}

/** @brief Ištrina elementus pagal predikato sąlygą (C++20 stilius). */
template<typename T, typename A, typename Pred>
typename Vector<T, A>::size_type erase_if(Vector<T, A>& c, Pred pred) {
    auto it = std::remove_if(c.begin(), c.end(), pred);
    auto n = c.end() - it;
    c.erase(it, c.end());
    return static_cast<typename Vector<T, A>::size_type>(n);
}

#endif // VECTOR_H