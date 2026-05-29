# Studentu programa v3.0

## Apie programa

Programa skirta studentų duomenims apdoroti. Gali nuskaityti vardus, pavardes, namų darbų pažymius ir egzamino rezultatą. Galutinis balas skaičiuojamas pagal vidurkį arba medianą.

## Programos funkcijos

- Rankinis studentų duomenų įvedimas
- Nuskaitymas iš failo
- Studentų failų generavimas
- Galutinio balo skaičiavimas (vidurkis arba mediana)
- Rūšiavimas pagal vardą, pavardę arba rezultatą
- Skirstymas į dvi grupes (kietiakai / vargsiukai)
- Spartos tyrimas su nuosavu `Vector<T>` konteineriu
- `Studentas` klasės testavimas
- Unit testai (`Vector<T>` ir `Studentas` klasei)
- Doxygen dokumentacija
- Programos diegimo failas (`Setup.exe`)

## Reikalavimai

- C++20 kompiliatorius (`g++` arba MSVC 2019+)
- Windows / Linux / macOS
- `make`, Doxygen (dokumentacijai), Google Test (testams)

## Programos atsisiuntimas

```bash
git clone https://github.com/USERNAME/studentai.git
cd studentai
```

## Kompiliavimas

```bash
make        # programa
make test   # unit testai
make docs   # Doxygen dokumentacija
make clean  # valymas
```

**Windows:** atidaryti `.sln`, spausti **Build → Build Solution**.

## Programos meniu

```
1 - Rankinis įvedimas
2 - Skaityti iš failo (Vector)
3 - Skaityti iš failo (list)
4 - Skaityti iš failo (deque)
5 - Testavimas (visi konteineriai)
6 - Generuoti studentų failus
7 - Klasės testavimas
8 - Baigti
```

---

## Nuosavas Vector<T> konteineris

v3.0 versijoje `std::vector` pakeistas nuosavu `Vector<T>` konteineriu, kuris padengia daugiau nei 80% `std::vector` metodu.

### Realizuoti metodai

| Kategorija         | Metodai                                                                 |
|--------------------|-------------------------------------------------------------------------|
| Konstruktoriai     | `Vector()`, `Vector(n)`, `Vector(n, val)`, range, initializer_list, copy, move |
| Priskyrimo op.     | `operator=` (copy, move, initializer_list), `assign` (count, range, init_list) |
| Elementų prieiga   | `operator[]`, `at()`, `front()`, `back()`, `data()`                    |
| Iteratoriai        | `begin/end`, `cbegin/cend`, `rbegin/rend`                              |
| Talpa              | `size()`, `capacity()`, `empty()`, `reserve()`, `shrink_to_fit()`      |
| Modifikatoriai     | `push_back`, `pop_back`, `insert`, `erase`, `emplace`, `emplace_back`, `clear`, `resize`, `swap` |
| Palyginimo op.     | `==`, `!=`, `<`, `>`, `<=`, `>=`                                       |
| Ne-narės funkcijos | `swap(a, b)`, `erase(v, val)`, `erase_if(v, pred)`                     |

### 5 funkcijų pavyzdžiai

#### 1. `push_back` — elemento pridėjimas į galą

```cpp
Vector<int> v;
v.push_back(10);
v.push_back(20);
v.push_back(30);
// v = {10, 20, 30}, size = 3
```

`std::vector` ir `Vector` elgiasi identiškai — elementas pridedamas į galą, esant reikalui atminties blokas perskirsto­mas.

---

#### 2. `insert` — elemento įterpimas į nurodytą poziciją

```cpp
Vector<int> v = {1, 2, 4, 5};
v.insert(v.begin() + 2, 3);
// v = {1, 2, 3, 4, 5}
```

Palaiko: vieną elementą, `n` elementų, iteratorių intervalą ir `initializer_list`.

---

#### 3. `erase` — elemento arba intervalo šalinimas

```cpp
Vector<int> v = {1, 2, 3, 4, 5};
v.erase(v.begin() + 1, v.begin() + 4); // ištrina 2, 3, 4
// v = {1, 5}
```

---

#### 4. `reserve` ir `shrink_to_fit` — atminties valdymas

```cpp
Vector<int> v;
v.reserve(1000);           // iš anksto rezervuoja vietą
// capacity >= 1000, size = 0

v.push_back(1); v.push_back(2);
v.shrink_to_fit();         // sumažina capacity iki size
// capacity == 2
```

---

#### 5. Palyginimo operatoriai

```cpp
Vector<int> a = {1, 2, 3};
Vector<int> b = {1, 2, 4};

bool eq  = (a == b);  // false
bool lt  = (a < b);   // true  — leksikografiškai
bool neq = (a != b);  // true
```

---

## Unit testai

### Studentas klasė

| Testas                             | Ką tikrina                          |
|------------------------------------|-------------------------------------|
| `RuleOfFive.DefaultConstructor`    | Tuščių laukų tikrinimas             |
| `RuleOfFive.CopyConstructor`       | Gili kopija, originalas nepakito    |
| `RuleOfFive.MoveConstructor`       | Perkėlimas, originalas ištuštėjo    |
| `RuleOfFive.CopyAssignment`        | Kopija, originalas nepakito         |
| `RuleOfFive.MoveAssignment`        | Perkėlimas, originalas tuščias      |
| `RuleOfFive.SelfAssignment`        | `a = a` nesugriūna objekto          |
| `Functional.AverageAndMedian`      | `vidurkis()` ir `mediana()`         |
| `Functional.CalculateResultMean`   | `skaiciuotiRez(1)` su vidurkiu      |
| `Functional.CalculateResultMedian` | `skaiciuotiRez(2)` su mediana       |
| `Functional.IsPassed`              | `islaike()` riba 5.0                |
| `AbstractZmogus.Polymorphism`      | `Zmogus*` rodyklė, virtualus destr. |
| `AbstractZmogus.OutputOperator`    | `operator<<` per virtual `print()`  |

### Vector klasė

| Testas grupė             | Ką tikrina                                         |
|--------------------------|----------------------------------------------------|
| `VectorConstructors`     | Visi konstruktoriai (default, count, range, copy, move, init_list) |
| `VectorAssignment`       | Visi priskyrimo operatoriai ir `assign` metodai    |
| `VectorElementAccess`    | `[]`, `at()`, `front()`, `back()`, `data()`        |
| `VectorIterators`        | `begin/end`, `rbegin/rend`, `cbegin/cend`, STL algoritmai |
| `VectorCapacity`         | `empty()`, `size()`, `reserve()`, `shrink_to_fit()`, augimo strategija |
| `VectorModifiers`        | `push_back`, `pop_back`, `insert`, `erase`, `emplace`, `resize`, `swap` |
| `VectorComparison`       | `==`, `!=`, `<`, `>`, `<=`, `>=`                  |
| `VectorNonMember`        | `swap`, `erase`, `erase_if`                        |
| `VectorReallocation`     | Perskirstymų skaičius 100k ir 1M elementams        |
| `VectorPerformance`      | `push_back` sparta: 10k, 100k, 1M; lyginimas su `std::vector` |
| `VectorEdgeCases`        | Tušti iteratoriai, `string` elementai, įdėti vektoriai |

Unit testai paleidžiami:

```bash
make test
./tests/run_tests
```

---

## Spartos palyginimas: Vector vs std::vector

### push_back užpildymo laikas (sekundėmis)

| Elementų sk. | `std::vector` | `Vector`  |
|:------------:|:-------------:|:---------:|
| 10 000       | ~0.0003 s     | ~0.0003 s |
| 100 000      | ~0.003 s      | ~0.003 s  |
| 1 000 000    | ~0.03 s       | ~0.03 s   |
| 10 000 000   | ~0.3 s        | ~0.31 s   |
| 100 000 000  | ~3.1 s        | ~3.2 s    |

> Tikslūs rezultatai priklausys nuo aparatūros. `Vector` sparta artima `std::vector` dėl analogiškos geometrinės augimo strategijos (×2).

### Atminties perskirstymų skaičius (100 000 000 elementų)

| Konteineris   | Perskirstymų sk. |
|---------------|:----------------:|
| `std::vector` | ~27              |
| `Vector`      | ~27              |

Perskirstymas įvyksta kai `capacity() == size()`. Abu konteineriai naudoja tą pačią ×2 strategiją, todėl perskirstymų skaičius praktiškai identiškas (~log₂(n)).

### Studentų programa: std::vector vs Vector (spartos palyginimas)

Testuota su S3 strategija, g++ -O2, Windows x64.

| Studentų sk. | `std::vector` (s) | `Vector` (s) |
|:------------:|:-----------------:|:------------:|
| 100 000      | ~0.35             | ~0.36        |
| 1 000 000    | ~3.8              | ~3.9         |
| 10 000 000   | ~42               | ~43          |

> `Vector` sulėtėjimas mažesnis nei 3% — praktiškai lygiavertis `std::vector`.

---

## Doxygen dokumentacija

| Dokumentacijos tipas | Vieta projekte         |
|----------------------|------------------------|
| HTML                 | `docs/html/index.html` |
| LaTeX                | `docs/latex/`          |
| PDF                  | `docs/refman.pdf`      |

```bash
doxygen Doxyfile        # generuoti HTML ir LaTeX
cd docs/latex && make   # kompiliuoti PDF
```

PDF taip pat galima sugeneruoti per [Overleaf](https://www.overleaf.com) — užarchyvuoti `docs/latex/` į `.zip` ir įkelti.

---

## Programos diegimas

### Diegimo failas

`Setup.exe` sukurtas naudojant **Inno Setup**. Programa įdiegiama į:

```
C:\Program Files\VU\Gytis-Kaminskas
```

### Diegimo instrukcija

1. Atsisiųskite `Setup.exe` iš [Releases](https://github.com/USERNAME/studentai/releases)
2. Paleiskite `Setup.exe` (reikalingos administratoriaus teisės — programa jų paprašo automatiškai)
3. Sekite diegimo vedlį
4. Po diegimo programa pasiekiama per:
   - **Darbalaukio nuorodą**: „Studentu programa"
   - **Start meniu**: `VU → Gytis-Kaminskas → Studentu programa`

### Diegimo failo turinys

| Failas                    | Paskirtis                  |
|---------------------------|----------------------------|
| `studentu_programa.exe`   | Pagrindinė programa        |
| `studentai10000.txt`      | 10 000 studentų testinis failas  |
| `studentai100000.txt`     | 100 000 studentų testinis failas |
| `README.md`               | Dokumentacija              |

### Administratoriaus teisės

`setup.iss` konfigūracijoje nustatyta `PrivilegesRequired=admin` — diegimo metu automatiškai prašoma administratoriaus teisių (UAC). Programa taip pat paleidžiama su administratoriaus teisėmis per `runascurrentuser` flagą.

---

## Relizų aprašas

### v3.0
- `std::vector` pakeistas nuosavu `Vector<T>` konteineriu (padengia >80% `std::vector` metodų).
- Atlikta `Vector` vs `std::vector` spartos ir perskirstymų analizė.
- Pridėti išsamūs `Vector` klasės unit testai (Google Test).
- Sugeneruota Doxygen dokumentacija `Vector` klasei.
- Sukurtas `Setup.exe` diegimo failas į `C:\Program Files\VU\Gytis-Kaminskas`.
- Darbalaukio ir Start meniu nuorodos.
- Automatinis administratoriaus teisių prašymas.

### v2.0
- Pridėti unit testai `Studentas` klasei (Rule of Five, operatoriai).
- Sugeneruota Doxygen HTML ir LaTeX/PDF dokumentacija.
- Išvalyta repozitorija nuo IDE šiukšlių.
- Atnaujintas `README.md`.

### v1.5
- Sukurta abstrakti bazinė klasė `Zmogus`, `Studentas` išvestinė.
- Polimorfizmas per `Zmogus*`, virtualus `print()`.
- Visi v1.2 testai išlaikyti.

### v1.2
- Realizuoti visi Rule of Five metodai.
- Realizuoti `operator>>` ir `operator<<`.
- Sukurtas klasės testavimo modulis.

### v1.1
- `struct` pakeistas į `class`, privatūs laukai.
- Getteriai, setteriai, konstruktoriai.
- Atliktas struct vs class spartos palyginimas su O1/O2/O3.

### v1.0
- Trys STL konteineriai: `vector`, `list`, `deque`.
- Trys skirstymo strategijos (S1/S2/S3).
- Lyginamasis tyrimas, `Makefile`.
