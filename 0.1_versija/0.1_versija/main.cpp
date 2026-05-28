#include <iostream>
#include <limits>
#include <string>
#include <list>
#include <deque>
#include <chrono>

#include "studentas.h"
#include "vector_ops.h"
#include "list_ops.h"
#include "deque_ops.h"
#include "utils.h"
#include "Vector.h"

void generuoti_testu_failus();
void generuoti_faila(const std::string&, int, int);
void atlikti_visus_testus();
void atlikti_klases_testus();

//  Vector 

static void skaityti_vector() {
    std::string fn;
    std::cout << "Failo pavadinimas: "; std::cin >> fn;

    Vector<Studentas> studentai;
    try {
        auto t0 = std::chrono::high_resolution_clock::now();
        skaitymas_is_failo(fn, studentai);
        double dt = std::chrono::duration<double>(
            std::chrono::high_resolution_clock::now() - t0).count();
        if (studentai.empty()) throw std::runtime_error("Failas tuscias");
        std::cout << "Skaitymas: " << dt << " s\n";
    }
    catch (const std::exception& e) { std::cerr << e.what() << "\n"; return; }

    int met = skaiciavimo_metodas();
    pasirinkimo_metodas(met, studentai);

    int rus = pasirinkimas_rusiavimo_budo();
    auto t0 = std::chrono::high_resolution_clock::now();
    rusiavimas(studentai, rus);
    std::cout << "Rusiavimas: "
        << std::chrono::duration<double>(
            std::chrono::high_resolution_clock::now() - t0).count()
        << " s\n";

    Vector<Studentas> kieti, vargsai;
    t0 = std::chrono::high_resolution_clock::now();
    skirstymas_i_grupes(studentai, kieti, vargsai);
    std::cout << "Skirstymas: "
        << std::chrono::duration<double>(
            std::chrono::high_resolution_clock::now() - t0).count()
        << " s\n";
    std::cout << "Kietiakai: " << kieti.size()
        << " | Vargsai: " << vargsai.size() << "\n";

    std::string base = fn.substr(0, fn.find_last_of('.'));
    isvedimas_i_faila(kieti, base + "_kietiakai.txt", "Kietiakai");
    isvedimas_i_faila(vargsai, base + "_vargsiukai.txt", "Vargsiukai");
}

// List 

static void skaityti_list() {
    std::string fn;
    std::cout << "Failo pavadinimas: "; std::cin >> fn;

    std::list<Studentas> studentai;   
    try {
        auto t0 = std::chrono::high_resolution_clock::now();
        skaitymas_is_failo_l(fn, studentai);
        double dt = std::chrono::duration<double>(
            std::chrono::high_resolution_clock::now() - t0).count();
        if (studentai.empty()) throw std::runtime_error("Failas tuscias");
        std::cout << "Skaitymas: " << dt << " s\n";
    }
    catch (const std::exception& e) { std::cerr << e.what() << "\n"; return; }

    pasirinkimo_metodas_l(1, studentai);

    auto t0 = std::chrono::high_resolution_clock::now();
    rusiavimas_l(studentai);
    std::cout << "Rusiavimas: "
        << std::chrono::duration<double>(
            std::chrono::high_resolution_clock::now() - t0).count()
        << " s\n";

    std::list<Studentas> kieti, vargsai;
    t0 = std::chrono::high_resolution_clock::now();
    skirstymas_i_grupes_l(studentai, kieti, vargsai);
    std::cout << "Skirstymas: "
        << std::chrono::duration<double>(
            std::chrono::high_resolution_clock::now() - t0).count()
        << " s\n";
    std::cout << "Kietiakai: " << kieti.size()
        << " | Vargsai: " << vargsai.size() << "\n";

    std::string base = fn.substr(0, fn.find_last_of('.'));
    isvedimas_i_faila_l(kieti, base + "_kietiakai.txt", "Kietiakai");
    isvedimas_i_faila_l(vargsai, base + "_vargsiukai.txt", "Vargsiukai");
}

//  Deque

static void skaityti_deque() {
    std::string fn;
    std::cout << "Failo pavadinimas: "; std::cin >> fn;

    std::deque<Studentas> studentai;
    try {
        auto t0 = std::chrono::high_resolution_clock::now();
        skaitymas_is_failo_d(fn, studentai);
        double dt = std::chrono::duration<double>(
            std::chrono::high_resolution_clock::now() - t0).count();
        if (studentai.empty()) throw std::runtime_error("Failas tuscias");
        std::cout << "Skaitymas: " << dt << " s\n";
    }
    catch (const std::exception& e) { std::cerr << e.what() << "\n"; return; }

    pasirinkimo_metodas_d(1, studentai);

    auto t0 = std::chrono::high_resolution_clock::now();
    rusiavimas_d(studentai);
    std::cout << "Rusiavimas: "
        << std::chrono::duration<double>(
            std::chrono::high_resolution_clock::now() - t0).count()
        << " s\n";

    std::deque<Studentas> kieti, vargsai;
    t0 = std::chrono::high_resolution_clock::now();
    skirstymas_i_grupes_d(studentai, kieti, vargsai);
    std::cout << "Skirstymas: "
        << std::chrono::duration<double>(
            std::chrono::high_resolution_clock::now() - t0).count()
        << " s\n";
    std::cout << "Kietiakai: " << kieti.size()
        << " | Vargsai: " << vargsai.size() << "\n";

    std::string base = fn.substr(0, fn.find_last_of('.'));
    isvedimas_i_faila_d(kieti, base + "_kietiakai.txt", "Kietiakai");
    isvedimas_i_faila_d(vargsai, base + "_vargsiukai.txt", "Vargsiukai");
}

//  Rankinis ivedimas 

static void rankinis_ivedimas() {
    Vector<Studentas> studentai;   
    int chosen = 1;
    do {
        std::string vardas = ivesti_varda_ar_pavarde("Vardas: ");
        std::string pavarde = ivesti_varda_ar_pavarde("Pavarde: ");
        Vector<int> paz;   // laikinas Vector pazymiu nuskaitymui
        std::cout << "Pazymiai (0-10, -1 baigti):\n";
        while (true) {
            int p = ivesties_tikrinimas("Pazymys: ");
            if (p == -1) break;
            paz.push_back(p);
        }
        int egz = ivesties_tikrinimas("Egzamino pazymys: ");
        studentai.emplace_back(vardas, pavarde, paz, egz);
        std::cout << "Dar vienas? (1-taip, 0-ne): "; std::cin >> chosen;
    } while (chosen == 1);

    if (!studentai.empty()) {
        int t = skaiciavimo_metodas();
        pasirinkimo_metodas(t, studentai);
        isvedimas(studentai, t);
    }
}

//  main

int main() {
    while (true) {
        std::cout << "\n======= STUDENTU PROGRAMA v3.0 (Vector<T>) =======\n"
            << "1 - Rankinis ivedimas\n"
            << "2 - Skaityti is failo (Vector)\n"
            << "3 - Skaityti is failo (list)\n"
            << "4 - Skaityti is failo (deque)\n"
            << "5 - Testavimas (visi konteineriai)\n"
            << "6 - Generuoti studentu failus\n"
            << "7 - Testavimas (klases)\n"
            << "8 - Baigti\n"
            << "Pasirinkimas: ";

        int p;
        std::cin >> p;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (p) {
        case 1: rankinis_ivedimas();    break;
        case 2: skaityti_vector();      break;
        case 3: skaityti_list();        break;
        case 4: skaityti_deque();       break;
        case 5: atlikti_visus_testus(); break;
        case 6: {
            std::cout << "1 - Visus testu failus (1k-10M)\n"
                "2 - Viena faila\nPasirinkimas: ";
            int sub; std::cin >> sub;
            if (sub == 1) {
                generuoti_testu_failus();
            }
            else {
                int n; std::string fn;
                std::cout << "Kiek studentu? "; std::cin >> n;
                std::cout << "Failo pavadinimas: "; std::cin >> fn;
                generuoti_faila(fn, n, 5);
            }
            break;
        }
        case 7: atlikti_klases_testus(); break;
        case 8: std::cout << "Programa baigta.\n"; return 0;
        default: std::cerr << "Tokio pasirinkimo nera.\n";
        }
    }
}