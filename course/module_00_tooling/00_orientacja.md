# 0.0 Orientacja: zanim zainstalujemy cokolwiek

Zanim zaczniemy cokolwiek instalować, warto rozłożyć na czynniki pierwsze kilka słów, których
będziemy używać przez cały semestr. Jeśli już je znasz — świetnie, przejrzyj pobieżnie i przejdź
dalej. Jeśli nie — to jest dokładnie ten moment, żeby się nie spieszyć.

## System operacyjny

System operacyjny (Windows, Linux, macOS) to program, który zarządza komputerem: plikami,
pamięcią, urządzeniami, innymi programami. Wszystko inne, o czym mówimy niżej, działa *na* nim.

## Terminal (konsola, wiersz poleceń)

Terminal to program, w którym wydajesz polecenia tekstem, zamiast klikać myszą. Nie jest ani
straszny, ani "dla zaawansowanych" — to po prostu inny sposób rozmowy z komputerem. W tym module
prawie go nie użyjemy — pracujemy głównie przez IDE. Ale terminal będzie się pojawiał coraz
częściej, więc oswajamy się z jego istnieniem od razu.

## Kompilator

Kompilator to program, który tłumaczy kod źródłowy (tekst, który piszesz — pliki `.cpp`) na kod
zrozumiały dla procesora. Bez kompilatora Twój plik `.cpp` to tylko tekst — nic się nie
"uruchamia" wprost z pliku źródłowego. Kompilator jest osobnym programem, zainstalowanym na
Twoim komputerze, niezależnym od edytora, w którym piszesz kod.

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
Kiedy uruchamiasz ten plik (przez IDE, dwuklik albo z terminala), system operacyjny tworzy
**proces** — działającą, żywą instancję tego programu, z własną pamięcią, działającą "teraz".
Ten sam plik wykonywalny możesz uruchomić wiele razy — za każdym razem powstanie osobny proces.

## Dlaczego to wszystko ma znaczenie

Kiedy coś "nie działa", pierwsze pytanie brzmi: na którym etapie? Czy to błąd kompilatora (kod się
nie tłumaczy)? Czy to błąd CMake (pliki się nie łączą)? Czy program się zbudował, ale coś robi źle,
kiedy już działa jako proces? W kolejnych krokach nauczymy się to rozróżniać na żywym przykładzie.

**Dalej:** instalacja narzędzi dla Twojego systemu —
[Windows](./01_instalacja_windows.md) · [Linux](./01_instalacja_linux.md) ·
[macOS](./01_instalacja_macos.md).
