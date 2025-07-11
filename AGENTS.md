# AGENT.md – конвертация XCC Mixer в консольную утилиту / DLL

> Этот документ предназначен для Codex‑агента, который будет автоматически вносить изменения в репозиторий.

## 1. Контекст

XCC Mixer — старый Win32/MFC‑инструмент для работы с ресурсами игр Westwood (MIX, SHP, PAL, AUD…). 
Все необходимые форматы реализованы в репозитории XCC https://github.com/OlafvdSpek/xcc каталоге **Library/**.
Мы хотим вынести функционал распаковки в консольное приложение **mixcons** и опционально экспортировать C‑интерфейс в виде **xmix.dll**. Файлы взятые из https://github.com/OlafvdSpek/xcc можно удалить, но добавить git Module https://github.com/OlafvdSpek/xcc если это упростит задачу

## 2. Цели

* 📦 **mixcons.exe**: кроссплатформенная CLI‑утилита

  * Распаковывает файлы из MIX‑архива
  * Декодирует SHP‑графику в PNG с выбранной палитрой
  * Возможность указать выходную директорию и фильтры по расширениям
* 🧩 **mix.dll**: тонкая DLL с функцией `Mix_Extract` без конвертации и `Mix_ExtractСonverted` с конвертацией для дальнейшей интеграции (C#/Python)
* 🛠 **CMake**‑сборка с использованием mingw-w64

## 3. Область работ (task list)

1. **Bootstrap**

   * [ ] Собрать корректный `CMakeLists.txt` в корень
   * [ ] Настроить опцию `BUILD_SHARED_LIBS` (ON — будет собираться с dll, OFF — только mixcons.exe)
   * [ ] Переписать проект с MSVC ≥ v142 в сборку в контейнерах  Linux, GCC 11+, Clang 13+, mingw-w64...
2. **CLI утилита**

   * [ ] Создать цель `mixcons` (type: EXECUTABLE)
   * [ ] Подключить зависимости: файлы из XCC репо и библиотеки (zlib, libpng, stb\_image\_write)
   * [ ] Реализовать парсер аргументов на [cxxopts](https://github.com/jarro2783/cxxopts)
   * [ ] Основная логика в `src/mixcons/mixcons.cpp`

     ```cpp
     mixcons <mix> <pal> [-o <dir>] [--no-shp] [--only-ext="aud,wav"]
     ```
3. **DLL обёртка**
   DLL позволит работать с архивами и файлами Westwood из своего приложения
   * [ ] Цель `mix.dll` (type: SHARED)
   * [ ] Экспортировать функцию

     ```cpp
     extern "C" __declspec(dllexport)
     int __stdcall Mix_Extract(const wchar_t* mix,
                               const wchar_t* pal,
                               const wchar_t* outDir);
     ```
   * [ ] Убедиться, что публичный ABI не тащит `std::string`/`std::vector`
4. **Тестирование**

   * [ ] Подключить GoogleTest
   * [ ] Юнит‑тесты для `mix_file` и `shp_ts_file`
   * [ ] Интеграционный тест: распаковка `conquer.mix` → сверка SHA‑1
5. **CI / CD**

   * [ ] GitHub Actions: matrix { windows‑latest, ubuntu‑latest }
   * [ ] Кэш vcpkg
   * [ ] Артефакты: `mixcons‑win.zip`, `mixcons‑linux.tar.gz`, `xcc_mix.dll`
6. **Документация**

   * [ ] Обновить `README.md` с примерами использования
   * [ ] Сгенерировать Doxygen‑страницы для Library

## 4. Технические детали

* **Стандарт C++**: минимум C++17 (для `<filesystem>`); если нужен `<span>`, переключаем `/std:c++20`.
* **Inline ASM**: не включаем файлы с VQA‑кодеком в Linux‑билд.
* **Пути инклудов**:

  ```cmake
  target_include_directories(xcc PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Library>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/misc>)
  ```
* **Зависимости**: через vcpkg (`zlib`, `libpng`, `jpeg-turbo`, `bzip2`).

## 5. Acceptance Criteria

| # | Критерий                                            | Проверка                                              |
| - | --------------------------------------------------- | ----------------------------------------------------- |
| 1 | `mixcons` распаковывает MIX и конвертирует SHP в PNG | `mixcons conquer.mix unittem.pal -o out/` ⇨ \~1000 PNG |
| 2 | DLL возвращает `0` на валидном архиве               | C# P/Invoke тест                                      |
| 3 | CI проходит «зелёным» в MSVC и GCC                  | GitHub Actions badge                                  |
| 4 | Код стильно форматирован `clang‑format`‑ом          | Pre‑commit hook                                       |

## 6. Out of Scope

* GUI‑рефакторинг старого XCC Mixer
* Поддержка форматов VQA/AUD‑потоков в CLI v1

## 7. Доп. ссылки

* [ModEnc Wiki – MIX format](https://modenc.renegadeprojects.com/MIX)
* [OlafvdSpek/xcc (master)](https://github.com/OlafvdSpek/xcc)
* [Boost.Filesystem → std::filesystem migration guide](https://www.boost.org/doc/libs/1_84_0/libs/filesystem/doc/index.htm)

---

*Документ создан Xenon007 2025‑07‑10 для автоматического агента Codex.*
