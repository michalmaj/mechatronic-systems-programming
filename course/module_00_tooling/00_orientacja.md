🇵🇱 Polski | [🇬🇧 English](00_orientacja.en.md)

# 0.0 Orientacja: zanim zainstalujemy cokolwiek

Zanim przejdziemy do instalacji, uporządkujmy kilka pojęć używanych przez cały semestr. Jeśli są Ci
znane, wystarczy szybka lektura. Jeśli widzisz je po raz pierwszy, poświęć chwilę na zrozumienie
różnic między nimi.

## System operacyjny

System operacyjny (Windows, Linux, macOS) to program, który zarządza komputerem: plikami,
pamięcią, urządzeniami, innymi programami. Wszystko inne, o czym mówimy niżej, działa *na* nim.

## Terminal (konsola, wiersz poleceń)

Terminal to program, w którym wydajesz polecenia tekstem zamiast wybierać je myszą. W tym module
większość pracy wykonasz w IDE, ale z czasem terminal będzie pojawiał się coraz częściej.

## Kompilator

Kompilator tłumaczy kod źródłowy z plików `.cpp` na kod zrozumiały dla procesora. Sam plik `.cpp`
jest tekstem, a nie gotowym programem. Kompilator działa niezależnie od edytora, w którym piszesz.

## CMake

Kod w projekcie tego kursu składa się z wielu plików `.cpp`. CMake to narzędzie, które opisuje,
jak te pliki połączyć w gotowe programy — które pliki wchodzą w skład którego programu, jakich
opcji kompilatora użyć. CMake sam nie kompiluje kodu — woła kompilator za Ciebie, z odpowiednimi
poleceniami.

## IDE (zintegrowane środowisko programistyczne)

IDE (Visual Studio, CLion) to program, w którym piszesz kod i który *dla wygody* potrafi
samodzielnie wywołać CMake i kompilator, pokazać Ci błędy w czytelnej formie i uruchomić gotowy
program jednym kliknięciem. IDE nie jest kompilatorem — jest nakładką, która ułatwia korzystanie
z kompilatora i CMake, żebyś nie musiał wpisywać poleceń ręcznie w terminalu (na razie).

## Proces i plik wykonywalny

Kiedy kompilator skończy pracę, powstaje **plik wykonywalny** (na Windows: `.exe`, na Linux/macOS:
plik bez rozszerzenia, oznaczony jako uruchamialny) — gotowy program, leżący na dysku.
Kiedy uruchamiasz ten plik przez IDE, dwuklikiem albo z terminala, system operacyjny tworzy
**proces**, czyli działający egzemplarz programu z własną pamięcią.
Ten sam plik wykonywalny możesz uruchomić wiele razy — za każdym razem powstanie osobny proces.

## Dlaczego to wszystko ma znaczenie

Kiedy coś "nie działa", pierwsze pytanie brzmi: na którym etapie? Czy to błąd kompilatora (kod się
nie tłumaczy)? Czy to błąd CMake (pliki się nie łączą)? Czy program się zbudował, ale coś robi źle,
kiedy już działa jako proces? W kolejnych krokach nauczymy się to rozróżniać na żywym przykładzie.

**Dalej:** instalacja narzędzi dla Twojego systemu —
[Windows](./01_instalacja_windows.md) · [Linux](./01_instalacja_linux.md) ·
[macOS](./01_instalacja_macos.md).
