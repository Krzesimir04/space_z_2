# Komunikacja CAN (TWAI) w Systemach Pokładowych (Zadanie 2)

Ten projekt realizuje niezawodną komunikację między dwoma mikrokontrolerami ESP32 przy użyciu magistrali CAN (Controller Area Network). W środowisku ESP-IDF interfejs ten nosi nazwę TWAI (Two-Wire Automotive Interface).

Program symuluje architekturę rozproszoną, w której nadawca (np. główny komputer pokładowy) wysyła pakiety danych telemetrycznych, a odbiorca (np. sterownik zaworów silnika) odbiera i analizuje te komendy w czasie rzeczywistym.

## Uwagi
1. W repozytorium znajdują się dwa osobne projekty: `receiver` (odbiorca) oraz `sender` (nadawca).
2. Ze względu na specyfikę symulatora Wokwi, Nadawca został skonfigurowany w trybie testowym (flaga `enable_loopback`), co pozwala mu na wysyłanie ramek  i odbieranie ich przez wysyłającego co zapobiega to blokowaniu symulacji.

## Połączenia sprzętowe (Pinout)
Dwa układy ESP32 komunikują się bezpośrednio przez złącza RX-TX. W fizycznym układzie między kontrolerami muszą znajdować się transceivery CAN (np. SN65HVD230).

| Sygnał | Odbiorca (ESP32) | Nadawca (ESP32) | Opis |
| :--- | :--- | :--- | :--- |
| **CAN TX** | GPIO 4 | GPIO 16 | Linia nadawcza kontrolera CAN |
| **CAN RX** | GPIO 5 | GPIO 17 | Linia odbiorcza kontrolera CAN|
| **Wspólna masa** | GND | GND | Wyrównuje potencjały |

## Symulacja w Wokwi (VS Code)
Projekt został w pełni przygotowany przy użyciu rozszerzenia **Wokwi Simulator dla Visual Studio Code**.

**Jak uruchomić symulację:**
0. Wejdź do katalogu `sender` lub `receiver`.
1. Upewnij się, że masz zainstalowane rozszerzenia *ESP-IDF* oraz *Wokwi Simulator* w VS Code.
2. Skompiluj projekt w terminalu za pomocą komendy: `idf.py build` (upewnij się, że wykonałeś `source ~/.../esp-idf/export.sh`).
3. Otwórz plik `diagram.json`, który zawiera zdefiniowany wirtualny schemat okablowania.
4. Uruchom symulację klikając przycisk **"Play"** w edytorze (lub wciśnij `F1` i wpisz `Wokwi: Start Simulator`).
5. Program automatycznie zainicjuje mikrokontroler, wyświetlając postęp w terminalu (UART).

## Format danych

Nadawca cyklicznie dystrybuuje po magistrali różne typy ramek o określonych ID. Każda z nich może nieść od 1 do 8 bajtów danych:

- ID 0x10 (Temperatura): Ramka 1-bajtowa. Przekazuje aktualny odczyt z czujników (np. 25°C).

- ID 0x01 (Ciśnienie): Ramka 4-bajtowa. Przekazuje precyzyjne dane o ciśnieniu w układzie zasilania silnika (np. ciśnienie w butli z N₂O).

- ID 0x100 (Status): Ramka 4-bajtowa. Służy do przekazywania wieloparametrowych flag wektorowych (np. wektory wejściowe z żyroskopu lub stany krytyczne).
