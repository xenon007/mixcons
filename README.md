# mixcons

mixcons — это консольная утилита для распаковки MIX‑архивов и конвертации SHP-графики.

## Требования
- **Windows**: Visual Studio 2022 (подойдет и 2019) с компонентом *Desktop development with C++*.
- [vcpkg](https://github.com/microsoft/vcpkg) для установки зависимостей.

## Установка зависимостей
1. Клонируйте vcpkg и выполните bootstrap.
2. Из каталога проекта запустите:
   ```powershell
   vcpkg install --triplet x64-windows `cat mixcons/vcpkg-install.txt`
   ```
   или просто `vcpkg install` при использовании манифеста `mixcons/vcpkg.json`.

## Сборка
1. Откройте файл `mixcons.sln` в Visual Studio.
2. Выберите конфигурацию `Release|x64`.
3. Соберите проект `mixcons`.

В результате в каталоге `x64/Release` появится `mixcons.exe`.

## Запуск
```
mixcons <mix> <pal>
```
