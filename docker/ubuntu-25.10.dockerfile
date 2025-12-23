FROM ubuntu:25.10

# Prevent interactive prompts during installation
ENV DEBIAN_FRONTEND=noninteractive

ARG cmake_version="4.2.0"
ARG ninja_version="1.13.2"
ARG fastbuild_version="1.18"
ARG USERNAME="devcontainer"
ARG USERID="1000"

####################################################
#               Install tools
# Install all package in one run to reduce image size
# by removeing unnecessary files after installation
####################################################
RUN    apt-get -y update \
    && apt-get -y upgrade \
    && apt-get -y --no-install-recommends --no-install-suggests install \
        zip unzip xz-utils git sudo \
        curl wget ca-certificates \
        bash-completion \
        graphviz  \
        gcc-14 g++-14 \
        gcc-15 g++-15 \
        gdb \
        gcovr lcov \
        clang-20 clang-format-20 clang-tidy-20 \
        clang-21 clang-format-21 clang-tidy-21 \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Install Ninja
RUN wget -O ninja.zip https://github.com/ninja-build/ninja/releases/download/v${ninja_version}/ninja-linux.zip \
    && unzip ninja.zip -d /opt/ninja-${ninja_version} \
    && rm ninja.zip

# Install FastBuild
RUN wget -O fastbuild.zip https://www.fastbuild.org/downloads/v${fastbuild_version}/FASTBuild-Linux-x64-v${fastbuild_version}.zip \
    && unzip fastbuild.zip -d /opt/fastbuild-${fastbuild_version} \
    && rm fastbuild.zip  \
    && chmod +x /opt/fastbuild-${fastbuild_version}/fbuild  \
    && chmod +x /opt/fastbuild-${fastbuild_version}/fbuildworker

# Install CMake
RUN wget -O cmake.tar.gz https://github.com/Kitware/CMake/releases/download/v${cmake_version}/cmake-${cmake_version}-linux-x86_64.tar.gz \
    && mkdir -p /opt/cmake-${cmake_version} \
    && tar -xzf cmake.tar.gz --strip-components=1 -C /opt/cmake-${cmake_version} \
    && rm cmake.tar.gz

# Install doxygen
RUN wget -O doxygen.tar.gz https://github.com/doxygen/doxygen/releases/download/Release_1_15_0/doxygen-1.15.0.linux.bin.tar.gz \
    && mkdir -p /opt/doxygen \
    && tar -xzf doxygen.tar.gz --strip-components=2 -C /opt/doxygen doxygen-1.15.0/bin \
    && rm doxygen.tar.gz

# Update PATH
ENV PATH="/opt/cmake-${cmake_version}/bin:$PATH"
ENV PATH="/opt/ninja-${ninja_version}:$PATH"
ENV PATH="/opt/fastbuild-${fastbuild_version}:$PATH"
ENV PATH="/opt/doxygen:$PATH"

# Setup an app user so the container doesn't run as root.
# remove default ubuntu user which as user id 1000.
RUN touch /var/mail/ubuntu && chown ubuntu /var/mail/ubuntu && userdel -r ubuntu
RUN useradd --shell /bin/bash          \
            --create-home ${USERNAME} \
            --uid ${USERID}

USER ${USERNAME}
WORKDIR /workspace

ENTRYPOINT ["/bin/bash"]
