#ifndef VECTOR_OPS_H
#define VECTOR_OPS_H

#include "studentas.h"
#include "Vector.h"
#include <string>

/**
 * @file vector_ops.h
 * @brief Vektoriaus operacijos naudojant nuosavą Vector konteinerį.
 *
 * v3.0: std::vector pakeistas nuosavu Vector<T> konteineriu.
 */

// Skaitymas ir isvedimas
void skaitymas_is_failo(const std::string& filename, Vector<Studentas>& studentai);


/**
 * @brief Išveda studentų sąrašą į konsolę.
 * @param studentai Studentų vektorius.
 * @param metodas 1 – vidurkis, 2 – mediana (nurodoma antraštėje).
 */
void isvedimas(const Vector<Studentas>& studentai, int metodas);
/**
 * @brief Išveda studentų sąrašą į failą.
 * @param studentai Studentų vektorius.
 * @param filename Išvesties failo pavadinimas.
 * @param kategorija "Kietiakai" arba "Vargsiukai" – naudojama pranešimui.
 * @throws std::runtime_error Jei failo nepavyksta sukurti.
 */
void isvedimas_i_faila(const Vector<Studentas>& studentai,
    const std::string& filename,
    const std::string& kategorija);
/**
 * @brief Apskaičiuoja galutinį rezultatą kiekvienam studentui.
 * @param tipas 1 – pagal vidurkį, 2 – pagal medianą.
 * @param studentai Studentų vektorius (modifikuojamas).
 */


void pasirinkimo_metodas(int tipas, Vector<Studentas>& studentai);

/**
 * @brief Surūšiuoja studentų vektorių pagal pasirinktą kriterijų.
 * @param studentai Vektorius (modifikuojamas).
 * @param budas Rūšiavimo būdas:
 *   1 – pagal vardą didėjant,
 *   2 – pagal vardą mažėjant,
 *   3 – pagal pavardę didėjant,
 *   4 – pagal pavardę mažėjant,
 *   5 – pagal galutinį rezultatą didėjant,
 *   6 – pagal galutinį rezultatą mažėjant.
 */


void rusiavimas(Vector<Studentas>& studentai, int budas);

/**
 * @brief Paprašo vartotojo pasirinkti rūšiavimo būdą.
 * @return Pasirinkimo kodas (1..6).
 */
int  pasirinkimas_rusiavimo_budo();

/**
 * @brief Paprastas skirstymas į dvi grupes (originalas lieka nepakitęs).
 * @param visi Visi studentai (const).
 * @param kieti [out] Studentai su rez >= 5.0.
 * @param vargsai [out] Studentai su rez < 5.0.
 */

void   skirstymas_i_grupes(const Vector<Studentas>& visi,
    Vector<Studentas>& kieti,
    Vector<Studentas>& vargsai);
/**
 * @brief Strategija S1: du nauji konteineriai, kopijavimas su copy_if.
 * @return Sugaištas laikas sekundėmis.
 */

double skirstymas_s1(const Vector<Studentas>& studentai,
    Vector<Studentas>& kieti,
    Vector<Studentas>& vargsai);

/**
 * @brief Strategija S2: vienas naujas + erase/remove_if.
 * @return Sugaištas laikas sekundėmis.
 */
double skirstymas_s2(Vector<Studentas>& studentai,
    Vector<Studentas>& vargsai);
/**
 * @brief Strategija S3: std::partition.
 * @return Sugaištas laikas sekundėmis.
 */

double skirstymas_s3(Vector<Studentas>& studentai,
    Vector<Studentas>& vargsai);

#endif
