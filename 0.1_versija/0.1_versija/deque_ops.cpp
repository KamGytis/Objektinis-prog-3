#include "deque_ops.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <stdexcept>
#include <chrono>
#include "vector.h"

void skaitymas_is_failo_d(const std::string& filename, std::deque<Studentas>& studentai) {
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
        int egz = paz.back(); paz.pop_back();
        studentai.emplace_back(vardas, pavarde, paz, egz);
        if (nr % 1000000 == 0)
            std::cout << "  Nuskaityta: " << nr << " studentu...\n";
    }
    std::cout << "  Is viso nuskaityta: " << studentai.size() << " studentu\n";
}

void isvedimas_i_faila_d(const std::deque<Studentas>& studentai,
    const std::string& filename,
    const std::string& kategorija) {
    std::ofstream out(filename);
    if (!out.is_open())
        throw std::runtime_error("Nepavyko sukurti failo: " + filename);
    out << std::left << std::setw(20) << "Vardas"
        << std::setw(20) << "Pavarde"
        << std::right << std::setw(20) << "Galutinis\n"
        << std::string(60, '-') << "\n";
    for (const auto& s : studentai) out << s << "\n";
    std::cout << "  [" << kategorija << "] " << studentai.size()
        << " studentu -> " << filename << "\n";
}

void pasirinkimo_metodas_d(int tipas, std::deque<Studentas>& studentai) {
    for (auto& s : studentai) s.skaiciuotiRez(tipas);
}

// deque turi atsitiktine prieiga - naudojame std::sort
void rusiavimas_d(std::deque<Studentas>& studentai) {
    std::sort(studentai.begin(), studentai.end(),
        [](const Studentas& a, const Studentas& b) { return a.getRez() < b.getRez(); });
}

double skirstymas_i_grupes_d(const std::deque<Studentas>& visi,
    std::deque<Studentas>& kieti,
    std::deque<Studentas>& vargsai) {
    auto t0 = std::chrono::high_resolution_clock::now();
    kieti.clear(); vargsai.clear();
    std::copy_if(visi.begin(), visi.end(), std::back_inserter(vargsai),
        [](const Studentas& s) { return s.getRez() < 5.0; });
    std::copy_if(visi.begin(), visi.end(), std::back_inserter(kieti),
        [](const Studentas& s) { return s.getRez() >= 5.0; });
    return std::chrono::duration<double>(
        std::chrono::high_resolution_clock::now() - t0).count();
}

// STRATEGIJA 1 - du nauji konteineriai, originalas lieka
// Naudoja std::copy_if
double skirstymas_s1_d(const std::deque<Studentas>& visi,
    std::deque<Studentas>& kieti,
    std::deque<Studentas>& vargsai) {
    auto t0 = std::chrono::high_resolution_clock::now();
    kieti.clear(); vargsai.clear();
    std::copy_if(visi.begin(), visi.end(), std::back_inserter(kieti),
        [](const Studentas& s) { return s.getRez() >= 5.0; });
    std::copy_if(visi.begin(), visi.end(), std::back_inserter(vargsai),
        [](const Studentas& s) { return s.getRez() < 5.0; });
    return std::chrono::duration<double>(
        std::chrono::high_resolution_clock::now() - t0).count();
}

// STRATEGIJA 2 - vienas naujas konteineris + erase/remove_if
double skirstymas_s2_d(std::deque<Studentas>& studentai,
    std::deque<Studentas>& vargsai) {
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

// STRATEGIJA 3 - stable_partition (greiciausia deque atveju)
double skirstymas_s3_d(std::deque<Studentas>& studentai,
    std::deque<Studentas>& vargsai) {
    auto t0 = std::chrono::high_resolution_clock::now();
    vargsai.clear();
    auto pivot = std::stable_partition(studentai.begin(), studentai.end(),
        [](const Studentas& s) { return s.getRez() < 5.0; });
    vargsai.assign(studentai.begin(), pivot);
    studentai.erase(studentai.begin(), pivot);
    return std::chrono::duration<double>(
        std::chrono::high_resolution_clock::now() - t0).count();
}