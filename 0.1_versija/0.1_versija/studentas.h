#ifndef STUDENTAS_H
#define STUDENTAS_H

#include "zmogus.h"
#include "vector.h"
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>

/**
 * @class Studentas
 * @brief Atstovauja studenta su pazymiais ir egzamino rezultatu
 *
 * Si klase praplecia Zmogus klase ir igyvendina studentams budinga funkcionaluma,
 * iskaitant pazymiu valdyma, rezultatu skaiciavima (vidurkis/mediana) ir
 * klasifikavima i grupes.
 *
 * @author Studentu Valdymo Sistema
 * @version 2.0
 * @date 2026
 */

class Studentas : public Zmogus {
private:
	std::vector<int> paz_; // Pazymiai
	int egz_; // Egzamino rezultatas
	double rez_; // Galutinis rezultatas (vidurkis arba mediana)

public:
    // Konstruktoriai 
    /**
     * @brief Numatytasis konstruktorius
     *
     * Sukuria tuscia studenta su numatytosiomis reiksmemis.
     * Visi pazymiai isvalomi, egzaminas = 0, rezultatas = 0.0
     */
    
    Studentas();

    /**
     * @brief Parametrinis konstruktorius
     * @param vardas Studento vardas
     * @param pavarde Studento pavarde
     * @param paz Namu darbu pazymiu vektorius
     * @param egz Egzamino pazymys
     *
     * @example
     * std::vector<int> pazymiai = {8, 9, 7};
     * Studentas s("Jonas", "Jonaitis", pazymiai, 10);
     */

    
    Studentas(const std::string& vardas,
        const std::string& pavarde,
        const Vector<int>& paz,
        int egz);

    /**
     * @brief Kopijavimo konstruktorius (gilioji kopija)
     * @param other Studentas objektas, is kurio kopijuojama
     * @kompleksiskumas O(n), kur n yra pazymiu skaicius
     */

    // Kopijavimo konstruktorius
    Studentas(const Studentas& other);

    /**
     * @brief Perkelimo konstruktorius
     * @param other Studentas objektas, is kurio perkeliama
     * @kompleksiskumas O(1)
     */

    
    Studentas(Studentas&& other) noexcept;

    /**
     * @brief Virtualus destruktorius
     *
     * Uztikrina tinkama isvestiniu klasiu isvalyma
     */
    ~Studentas();

    //  Priskyrimo operatoriai 

    /**
     * @brief Kopijavimo priskyrimo operatorius
     * @param other Studentas objektas, is kurio kopijuojama
     * @return Nuoroda i si objekta
     */

    Studentas& operator=(const Studentas& other);

    /**
     * @brief Perkelimo priskyrimo operatorius
     * @param other Studentas objektas, is kurio perkeliama
     * @return Nuoroda i si objekta
     */
    Studentas& operator=(Studentas&& other) noexcept;

    //  Palyginimo operatoriai 

     /**
     * @brief Maziau uz (pagal galutini rezultata)
     * @param other Studentas su kuriuo lyginama
     * @return true jei si.rez_ < other.rez_
     */
    bool operator<(const Studentas& other) const;

    /**
     * @brief Daugiau uz (pagal galutini rezultata)
     * @param other Studentas su kuriuo lyginama
     * @return true jei si.rez_ > other.rez_
     */
    bool operator>(const Studentas& other) const;

    /**
    * @brief Lygybe (pagal varda ir pavarde)
    * @param other Studentas su kuriuo lyginama
    * @return true jei vardai ir pavardes sutampa
    */
    bool operator==(const Studentas& other) const;

    /**
     * @brief Maziau arba lygu
     * @return true jei si.rez_ <= other.rez_
     */
    bool operator<=(const Studentas& other) const;

    /**
     * @brief Daugiau arba lygu
     * @return true jei si.rez_ >= other.rez_
     */
    bool operator>=(const Studentas& other) const;

    /**
     * @brief Nelygybe
     * @param other Studentas su kuriuo lyginama
     * @return true jei vardai ar pavardes skiriasi
     */
    bool operator!=(const Studentas& other) const;

    // Prideti pazymi: s += 8
    Studentas& operator+=(int pazymys);

    // Prieiga prie paz_[i]
    int  operator[](size_t i) const;
    int& operator[](size_t i);

    // SRAUTU OPERATORIAI

    /**
     * @brief Isvedimo i ekrana operatorius
     * @param os Isvesties srautas
     * @param s Studentas objektas
     * @return Nuoroda i isvesties srauta
     * @detales Formatas: "Vardas Pavarde [pazymiai] Egz: E Rez: R"
     */
    friend std::ostream& operator<<(std::ostream& os, const Studentas& s);

    /**
     * @brief Ivedimo is failo operatorius
     * @param is Ivesties srautas
     * @param s Studentas objektas
     * @return Nuoroda i ivesties srauta
     * @detales Skaito: "Vardas Pavarde paz1 paz2 ... pazN egzaminas"
     */
    friend std::istream& operator>>(std::istream& is, Studentas& s);

    // 
    // Virtualus metodas – realizuoja bazines klases gryna virtualu metoda
    // 


    /**
     * @brief Spausdina studento informacija (pakeicia Zmogus::print)
     * @param os Isvesties srautas
     */
    void print(std::ostream& os) const override;

    // 
    // GETTERIAI
    // 

     /** @return Pastovi nuoroda i pazymiu vektoriu */
    const std::vector<int>& getPaz()         const { return paz_; }
    
    /** @return Egzamino pazymys */
    int                     getEgz()         const { return egz_; }
    
    /** @return Galutinis rezultatas */
    double                  getRez()         const { return rez_; }
    
    /** @return Namu darbu pazymiu skaicius */
    size_t                  getPazSkaicius() const { return paz_.size(); }

    // 
    // SETTERIAI
    // 

    /** @param e Naujas egzamino pazymys (0-10) */
    void setEgz(int e) { egz_ = e; }

    /** @param r Naujas galutinis rezultatas */
    void setRez(double r) { rez_ = r; }

    // 
    // METODAI
    // 

    /**
     * @brief Prideda viena namu darbu pazymi
     * @param p Pazymys (1-10)
     */
    void   addPazymys(int p);

    /** @brief Isvalo visus namu darbu pazymius */
    void   clearPazymiai();

    /**
     * @brief Skaiciuoja namu darbu pazymiu vidurki
     * @return Aritmetinis vidurkis kaip double
     */
    double vidurkis() const;

    /**
    * @brief Skaiciuoja namu darbu pazymiu mediana
    * @return Mediana kaip double
    */
    double mediana()  const;

    /**
     * @brief Skaiciuoja galutini rezultata
     * @param tipas 1=vidurkis, 2=mediana
     * @detales Formule: 0.4 * pazymys + 0.6 * egzaminas
     */
    void   skaiciuotiRez(int tipas); // 1=vidurkis, 2=mediana

    /**
     * @brief Tikrina ar studentas islaike
     * @return true jei rez_ >= 5.0
     */
    bool   islaike() const { return rez_ >= 5.0; }
};

#endif 