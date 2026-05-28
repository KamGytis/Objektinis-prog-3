#include "list_ops.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <stdexcept>
#include <chrono>

void skaitymas_is_failo_l(const std::string& filename, std::list<Studentas>& studentai) {
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

void isvedimas_i_faila_l(const std::list<Studentas>& studentai,
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

void pasirinkimo_metodas_l(int tipas, std::list<Studentas>& studentai) {
	for (auto& s : studentai) s.skaiciuotiRez(tipas);
}

//list rusiavimas pagal pavarde, varda
void rusiavimas_l(std::list<Studentas>& studentai) {
	studentai.sort([](const Studentas& a, const Studentas& b) {
		if (a.getPavarde() != b.getPavarde()) return a.getPavarde() < b.getPavarde();
		return a.getVardas() < b.getVardas();
		});
}

// Originali strategija - du nauji konteineriai, originalas lieka
double skirstymas_i_grupes_l(const std::list<Studentas>& visi,
	std::list<Studentas>& kieti,
	std::list<Studentas>& vargsai) {
	auto t0 = std::chrono::high_resolution_clock::now();
	kieti.clear(); vargsai.clear();
	for (const auto& s : visi) {
		if (s.getRez() < 5.0) vargsai.push_back(s);
		else                   kieti.push_back(s);
	}
	return std::chrono::duration<double>(
		std::chrono::high_resolution_clock::now() - t0).count();
}

// S1: du nauji konteineriai su kopijomis (std::copy_if)
double skirstymas_s1_l(const std::list<Studentas>& visi,
	std::list<Studentas>& kieti,
	std::list<Studentas>& vargsai) {
	auto t0 = std::chrono::high_resolution_clock::now();
	kieti.clear(); vargsai.clear();
	std::copy_if(visi.begin(), visi.end(), std::back_inserter(kieti),
		[](const Studentas& s) { return s.getRez() >= 5.0; });
	std::copy_if(visi.begin(), visi.end(), std::back_inserter(vargsai),
		[](const Studentas& s) { return s.getRez() < 5.0; });
	return std::chrono::duration<double>(
		std::chrono::high_resolution_clock::now() - t0).count();
}

// S2: vienas naujas konteineris + erase kilpa
double skirstymas_s2_l(std::list<Studentas>& studentai,
	std::list<Studentas>& vargsai) {
	auto t0 = std::chrono::high_resolution_clock::now();
	vargsai.clear();
	for (auto it = studentai.begin(); it != studentai.end(); ) {
		if (it->getRez() < 5.0) {
			vargsai.push_back(*it);
			it = studentai.erase(it);
		}
		else { ++it; }
	}
	return std::chrono::duration<double>(
		std::chrono::high_resolution_clock::now() - t0).count();
}

// S3: splice - nulines kopijos, O(n)
double skirstymas_s3_l(std::list<Studentas>& studentai,
	std::list<Studentas>& vargsai) {
	auto t0 = std::chrono::high_resolution_clock::now();
	vargsai.clear();
	for (auto it = studentai.begin(); it != studentai.end(); ) {
		if (it->getRez() < 5.0) {
			vargsai.splice(vargsai.end(), studentai, it++);
		}
		else { ++it; }
	}
	return std::chrono::duration<double>(
		std::chrono::high_resolution_clock::now() - t0).count();
}