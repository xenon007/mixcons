FROM ubuntu:22.04

# Установка основных пакетов для кросс-компиляции под Windows
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
        build-essential \
        cmake \
        ninja-build \
        git \
        zip unzip \
        pkg-config \
        mingw-w64 \
        g++-mingw-w64-x86-64 \
        && rm -rf /var/lib/apt/lists/*

# Рабочий каталог по умолчанию
WORKDIR /project

# Папки, куда можно монтировать дополнительные пакеты
VOLUME ["/build_packages", "/vcpkg_packages"]

CMD ["bash"]
