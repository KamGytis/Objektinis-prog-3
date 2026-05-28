/**
 * @file vector_ops.cpp
 * @brief Vektoriaus operacijų realizacija naudojant nuosavą Vector konteinerį.
 *
 * v3.0: std::vector pakeistas nuosavu Vector<T> konteineriu.
 */

#include "vector_ops.h"
#include "utils.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <stdexcept>
#include <chrono>

void skaitymas_is_failo(const std::string& filename, Vector<Studentas>& studentai) {
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Nepavyko atidaryti failo: " + filename);

    studentai.clear();
    std::string header, line;
    std::getline(file, header);

    int nr = 0;
    while (std::getline(file, line)) {
        ++nr;
        std::stringstream ss(line);
        std::string vardas, pavarde;
        ss >> vardas >> pavarde;
        Vector<int> paz;
        int v;
        while (ss >> v) paz.push_back(v);
        if (paz.empty()) continue;
        int egz = paz.back();
        paz.pop_back();
        studentai.emplace_back(vardas, pavarde, paz, egz);
        if (nr % 1000000 == 0)
            std::cout << "  Nuskaityta: " << nr << " studentu...\n";
    }
    std::cout << "  Is viso nuskaityta: " << studentai.size() << " studentu\n";
}

void isvedimas(const Vector<Studentas>& studentai, int metodas) {
    std::string rez = (metodas == 1) ? "Galutinis (vidurkis)" : "Galutinis (mediana)";
    std::cout << std::left << std::setw(15) << "Vardas"
        << std::setw(15) << "Pavarde"
        << std::right << std::setw(20) << rez << "\n";
    for (const auto& s : studentai)
        std::cout << s << "\n";
}

void isvedimas_i_faila(const Vector<Studentas>& studentai,
    const std::string& filename,
    const std::string& kategorija) {
    std::ofstream out(filename);
    if (!out.is_open())
        throw std::runtime_error("Nepavyko sukurti failo: " + filename);

    out << std::left << std::setw(20) << "Vardas"
        << std::setw(20) << "Pavarde"
        << std::right << std::setw(20) << "Galutinis\n"
        << std::string(60, '-') << "\n";

    for (const auto& s : studentai)
        out << s << "\n";

    std::cout << "  [" << kategorija << "] " << studentai.size()
        << " studentu -> " << filename << "\n";
}

void pasirinkimo_metodas(int tipas, Vector<Studentas>& studentai) {
    for (auto& s : studentai)
        s.skaiciuotiRez(tipas);
}

void rusiavimas(Vector<Studentas>& studentai, int budas) {
    switch (budas) {
    case 1: std::sort(studentai.begin(), studentai.end(),
        [](const Studentas& a, const Studentas& b) { return a.getVardas() < b.getVardas(); }); break;
    case 2: std::sort(studentai.begin(), studentai.end(),
        [](const Studentas& a, const Studentas& b) { return a.getVardas() > b.getVardas(); }); break;
    case 3: std::sort(studentai.begin(), studentai.end(),
        [](const Studentas& a, const Studentas& b) { return a.getPavarde() < b.getPavarde(); }); break;
    case 4: std::sort(studentai.begin(), studentai.end(),
        [](const Studentas& a, const Studentas& b) { return a.getPavarde() > b.getPavarde(); }); break;
    case 5: std::sort(studentai.begin(), studentai.end(),
        [](const Studentas& a, const Studentas& b) { return a.getRez() < b.getRez(); }); break;
    case 6: std::sort(studentai.begin(), studentai.end(),
        [](const Studentas& a, const Studentas& b) { return a.getRez() > b.getRez(); }); break;
    default: std::cerr << "Neteisingas rusiavimo budas.\n"; break;
    }
}

int pasirinkimas_rusiavimo_budo() {
    std::cout << "Pasirinkite rusiavimo buda:\n"
        << "1 - pagal varda didejanciai\n"
        << "2 - pagal varda mazejanciai\n"
        << "3 - pagal pavarde didejanciai\n"
        << "4 - pagal pavarde mazejanciai\n"
        << "5 - pagal galutini rezultata didejanciai\n"
        << "6 - pagal galutini rezultata mazejanciai\n";
    int p; std::cin >> p;
    if (std::cin.fail() || p < 1 || p > 6) {
        std::cerr << "Neteisingas pasirinkimas.\n";
        return 5;
    }
    return p;
}

void skirstymas_i_grupes(const Vector<Studentas>& visi,
    Vector<Studentas>& kieti,
    Vector<Studentas>& vargsai) {
    for (const auto& s : visi) {
        if (s.getRez() >= 5.0) kieti.push_back(s);
        else                   vargsai.push_back(s);
    }
}

double skirstymas_s1(const Vector<Studentas>& studentai,
    Vector<Studentas>& kieti,
    Vector<Studentas>& vargsai) {
    auto t0 = std::chrono::high_resolution_clock::now();
    kieti.clear(); vargsai.clear();
    std::copy_if(studentai.begin(), studentai.end(), std::back_inserter(kieti),
        [](const Studentas& s) { return s.getRez() >= 5.0; });
    std::copy_if(studentai.begin(), studentai.end(), std::back_inserter(vargsai),
        [](const Studentas& s) { return s.getRez() < 5.0; });
    return std::chrono::duration<double>(
        std::chrono::high_resolution_clock::now() - t0).count();
}

double skirstymas_s2(Vector<Studentas>& studentai,
    Vector<Studentas>& vargsai) {
    auto t0 = std::chrono::high_resolution_clock::now();
    vargsai.clear();
    std::copy_if(studentai.begin(), studentai.end(), std::back_inserter(vargsai),
        [](const Studentas& s) { return s.getRez() < 5.0; });
    studentai.erase(
        std::remove_if(studentai.begin(), studentai.end(),
            [](const Studentas& s) { return s.getRez() < 5.0; }),
        studentai.end());
    return std::chrono::duration<double>(
        std::chrono::high_resolution_clock::now() - t0).count();
}

double skirstymas_s3(Vector<Studentas>& studentai,
    Vector<Studentas>& vargsai) {
    auto t0 = std::chrono::high_resolution_clock::now();
    vargsai.clear();
    auto pivot = std::partition(studentai.begin(), studentai.end(),
        [](const Studentas& s) { return s.getRez() >= 5.0; });
    vargsai.assign(pivot, studentai.end());
    studentai.erase(pivot, studentai.end());
    return std::chrono::duration<double>(
        std::chrono::high_resolution_clock::now() - t0).count();
}