#include "vector_ops.h"
#include "list_ops.h"
#include "deque_ops.h"
#include "vector.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <list>
#include <deque>
#include <string>
#include <chrono>
#include <algorithm>

struct TestRow {
    std::string konteineris;
    std::string strategija;
    std::string failas;
    int    studentu_sk = 0;
    double skaitymas = 0.0;
    double rusiavimas = 0.0;
    double skirstymas = 0.0;
    double bendras = 0.0;
};

static void spausdinti_lentele(const std::vector<TestRow>& eilutes) {
    std::cout << "\n" << std::string(90, '=') << "\n";
    std::cout << std::left
        << std::setw(8) << "Kont."
        << std::setw(5) << "Strat"
        << std::setw(24) << "Failas"
        << std::right
        << std::setw(10) << "Stud."
        << std::setw(12) << "Skait.(s)"
        << std::setw(11) << "Rusi.(s)"
        << std::setw(12) << "Skirsti.(s)"
        << std::setw(12) << "Bendras(s)"
        << "\n" << std::string(90, '-') << "\n";
    for (const auto& r : eilutes) {
        std::cout << std::left
            << std::setw(8) << r.konteineris
            << std::setw(5) << r.strategija
            << std::setw(24) << r.failas
            << std::right << std::fixed << std::setprecision(6)
            << std::setw(10) << r.studentu_sk
            << std::setw(12) << r.skaitymas
            << std::setw(11) << r.rusiavimas
            << std::setw(12) << r.skirstymas
            << std::setw(12) << r.bendras
            << "\n";
    }
    std::cout << std::string(90, '=') << "\n";
}

static TestRow testuoti_vector(const std::string& f, int strat) {
    TestRow r; r.konteineris = "vector";
    r.strategija = "S" + std::to_string(strat); r.failas = f;
    auto total = std::chrono::high_resolution_clock::now();

    std::vector<Studentas> studentai;
    auto t0 = std::chrono::high_resolution_clock::now();
    skaitymas_is_failo(f, studentai);
    r.skaitymas = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - t0).count();
    r.studentu_sk = (int)studentai.size();
    pasirinkimo_metodas(1, studentai);
    t0 = std::chrono::high_resolution_clock::now();
    rusiavimas(studentai, 5);
    r.rusiavimas = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - t0).count();

    std::vector<Studentas> kieti, vargsai, tmp;
    if (strat == 1) { r.skirstymas = skirstymas_s1(studentai, kieti, vargsai); }
    else if (strat == 2) { tmp = studentai; r.skirstymas = skirstymas_s2(tmp, vargsai); }
    else { tmp = studentai; r.skirstymas = skirstymas_s3(tmp, vargsai); }

    r.bendras = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - total).count();
    return r;
}

static TestRow testuoti_list(const std::string& f, int strat) {
    TestRow r; r.konteineris = "list";
    r.strategija = "S" + std::to_string(strat); r.failas = f;
    auto total = std::chrono::high_resolution_clock::now();

    std::list<Studentas> studentai;
    auto t0 = std::chrono::high_resolution_clock::now();
    skaitymas_is_failo_l(f, studentai);
    r.skaitymas = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - t0).count();
    r.studentu_sk = (int)studentai.size();
    pasirinkimo_metodas_l(1, studentai);
    t0 = std::chrono::high_resolution_clock::now();
    rusiavimas_l(studentai);
    r.rusiavimas = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - t0).count();

    std::list<Studentas> kieti, vargsai, tmp;
    if (strat == 1) { r.skirstymas = skirstymas_s1_l(studentai, kieti, vargsai); }
    else if (strat == 2) { tmp = studentai; r.skirstymas = skirstymas_s2_l(tmp, vargsai); }
    else { tmp = studentai; r.skirstymas = skirstymas_s3_l(tmp, vargsai); }

    r.bendras = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - total).count();
    return r;
}

static TestRow testuoti_deque(const std::string& f, int strat) {
    TestRow r; r.konteineris = "deque";
    r.strategija = "S" + std::to_string(strat); r.failas = f;
    auto total = std::chrono::high_resolution_clock::now();

    std::deque<Studentas> studentai;
    auto t0 = std::chrono::high_resolution_clock::now();
    skaitymas_is_failo_d(f, studentai);
    r.skaitymas = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - t0).count();
    r.studentu_sk = (int)studentai.size();
    pasirinkimo_metodas_d(1, studentai);
    t0 = std::chrono::high_resolution_clock::now();
    rusiavimas_d(studentai);
    r.rusiavimas = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - t0).count();

    std::deque<Studentas> kieti, vargsai, tmp;
    if (strat == 1) { r.skirstymas = skirstymas_s1_d(studentai, kieti, vargsai); }
    else if (strat == 2) { tmp = studentai; r.skirstymas = skirstymas_s2_d(tmp, vargsai); }
    else { tmp = studentai; r.skirstymas = skirstymas_s3_d(tmp, vargsai); }

    r.bendras = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - total).count();
    return r;
}

void atlikti_visus_testus() {
    const std::vector<std::string> failai = {
        "studentai1000.txt",
        "studentai10000.txt",
        "studentai100000.txt",
        "studentai1000000.txt",
        "studentai10000000.txt"
    };

    std::vector<TestRow> rezultatai;
    std::cout << "\n########## PRADEDAMAS TESTAVIMAS ##########\n\n";
    for (const auto& f : failai) {
        for (int s = 1; s <= 3; ++s) {
            std::cout << ">> vector S" << s << " | " << f << "\n";
            try { rezultatai.push_back(testuoti_vector(f, s)); }
            catch (const std::exception& e) { std::cerr << "  KLAIDA: " << e.what() << "\n"; }

            std::cout << ">> list   S" << s << " | " << f << "\n";
            try { rezultatai.push_back(testuoti_list(f, s)); }
            catch (const std::exception& e) { std::cerr << "  KLAIDA: " << e.what() << "\n"; }

            std::cout << ">> deque  S" << s << " | " << f << "\n";
            try { rezultatai.push_back(testuoti_deque(f, s)); }
            catch (const std::exception& e) { std::cerr << "  KLAIDA: " << e.what() << "\n"; }
        }
    }

    std::cout << "\n########## TESTAVIMAS BAIGTAS ##########\n";
    spausdinti_lentele(rezultatai);
}