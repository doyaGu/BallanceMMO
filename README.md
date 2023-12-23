# BallanceMMO

Build status:
[![Server](https://github.com/Swung0x48/BallanceMMO/actions/workflows/server.yml/badge.svg)](https://github.com/Swung0x48/BallanceMMO/actions/workflows/server.yml)
·
[![Client](https://github.com/Swung0x48/BallanceMMO/actions/workflows/client.yml/badge.svg)](https://github.com/Swung0x48/BallanceMMO/actions/workflows/client.yml)

## What is BallanceMMO?

BallanceMMO is a project which brings online experiences to Ballance.

## Getting Started

### Client

1. Install Ballance Mod Loader (aka. BML).
    - Download the latest version of BML [here](https://github.com/Gamepiaynmo/BallanceModLoader/releases).
    - Extract and put `BML.dll` under `$(game_directory)\BuildingBlocks`, where `$(game_directory)` is where you installed your game to.
2. Download the latest mod release from the [BMMO download site](https://dl.bmmo.bcrc.site). Alternatively, try out [builds from GitHub Actions](https://github.com/Swung0x48/BallanceMMO/actions/workflows/client.yml) which also support [BallanceModLoaderPlus](https://github.com/doyaGu/BallanceModLoaderPlus).

3. Put `BallanceMMO_x.y.z-[stage]b.bmod` and required `.dll` dependencies under `$(game_directory)\ModLoader\Mods`. (If the directory is not present, you may launch the game and BML will generate that for you)

4. Launch the game and enjoy!

### Server

1. Download your desired [server builds from GitHub Actions](https://github.com/Swung0x48/BallanceMMO/actions/workflows/server.yml), as currently we don't have a release page for it (maybe we'll make one in the future?).

2. Extract the archive and run `start_ballancemmo_loop.bat` (Windows) or `start_ballancemmo_loop.sh` (Linux/macOS), which is our helper script to take care of logging and server crashes.

3. Modify the generated `config.yml` according to your needs; you may also run `./BallanceMMOServer --help` to see if there are any configurable command line arguments (you can also supply them through our helper script!).

4. Type `reload` in the server console and apply your config changes. Now you're ready to share your address and invite others!

## Dependencies or build tools

- CMake 3.12 or later: Generate makefile for server
- A build tool like GNU Make, Ninja: Build server binary
- A compiler with core C++20 feature support
  - This project has been successfully compiled under:
    - GCC ~~9.4~~, 10.3, 11.3, 12.2, and 13.2 (server-side components; GCC 9 support dropped since January 2023)
    - Apple Clang 13.1 and 16.0 (server-side)
    - Visual Studio 2019 and 2022 (client-side, or specifically, BML-related stuff)
- One of the following crypto solutions:
  - OpenSSL 1.1.1 or later
  - OpenSSL 1.1.x, plus ed25519-donna and curve25519-donna. (Valve GNS has made some minor changes, so the source is included in this project.)
  - libsodium
- Google protobuf 2.6.1+ (included in submodule)
- GNU Readline (for UNIX-like systems)
- Dev pack of BallanceModLoader (client-side, [*release page*](https://github.com/Gamepiaynmo/BallanceModLoader/releases)) and (optionally) [BallanceModLoaderPlus](https://github.com/doyaGu/BallanceModLoaderPlus)

## Building server

### Linux/macOS

1. Clone this repo __RECURSIVELY__

    ```commandline
    git clone https://github.com/Swung0x48/BallanceMMO.git --recursive
    ```

2. Install OpenSSL, protobuf, and GNU Readline

    - Debian/Ubuntu

        ```commandline
        apt install libssl-dev
        apt install libprotobuf-dev protobuf-compiler
        apt install libreadline-dev
        ```

    - Arch

        ```commandline
        pacman -S openssl
        pacman -S protobuf
        pacman -S readline
        ```

    - macOS with brew

        ```commandline
        brew install openssl@1.1
        export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/opt/openssl@1.1/lib/pkgconfig
        brew install protobuf
        brew install readline
        ```

    (Haven't tried on a Linux distro with yum as package manager. Sorry Fedora/RHEL/CentOS guys...)

3. Building!

    - Ninja

        ```commandline
        chmod +x build.sh
        ./build.sh
        cd build
        ninja
        ```

    - GNU Make

        ```commandline
        chmod +x build.sh
        ./build.sh -G
        cd build
        make
        ```

    You may also want to supply command line arguments such as `-DCMAKE_BUILD_TYPE=Release` when running `build.sh`, to specify the build type.

    If CMake failed to find openssl for some reason, you may need to specify path to openssl yourself.

    For example:

    ```commandline
    $ ./build.sh -DOPENSSL_ROOT_DIR="/usr/local/opt/openssl@1.1/"
    ```

4. Run your server!

    (Don't forget to navigate to the build directory first: `cd BallanceMMOServer`)

    ```commandline
    $ ./BallanceMMOServer
    ```

    For help or customization, use the command line argument `--help`.

    ```commandline
    $ ./BallanceMMOServer --help
    Usage: ./BallanceMMOServer [OPTION]...
    Options:
      -p, --port=PORT      Use PORT as the server port instead (default: 26676).
      -l, --log=PATH       Write log to the file at PATH in addition to stdout.
      -h, --help           Display this help and exit.
      -v, --version        Display version information and exit.
          --dry-run        Test the server by starting it and exiting immediately.
    ```

    Alternatively, use the bash script `start_ballancemmo_loop.sh` (`start_ballancemmo_loop.bat` on Windows) which handles file logging automatically and restarts the server after crashes. *All command line arguments for the server executable also applies there.*

    ```commandline
    $ ./start_ballancemmo_loop.sh
    ```

## Building client (Game Mod)

On Windows, we can use the vcpkg package manager. The following instructions assume that you will follow the steps and fetch vcpkg as a submodule. If you want to install vcpkg somewhere else, you're on your own.

__Warning Beforehand__: DO NOT upgrade (cancel when prompted) when Visual Studio asks you to upgrade the solution/project on first launch. It will wipe include directories which has been set in the project file (wtf Microsoft???). Don't worry, following steps will guide you to switch to your already-set-up msvc toolchain.

1. Install/Modify Visual Studio
    - In `Workload` tab, make sure you have installed `Desktop Development with C++`.
    - __IMPORTANT!__ In `Language pack` tab, make sure you have installed `English`.

2. Bootstrap vcpkg
    - In a `Visual Studio Developer PowerShell`/`Visual Studio Developer Command Prompt`

        ```commandline
        cd submodule\vcpkg
        .\bootstrap-vcpkg.bat
        ```

3. Build the prerequisites using vcpkg

    - This is the step where vcpkg will fail if you haven't got your English language pack installed. (Again, wtf Microsoft???)

        ```commandline
        > .\vcpkg --overlay-ports=..\GameNetworkingSockets\vcpkg_ports install gamenetworkingsockets
        ```

        You may also try

        ```commandline
        > .\vcpkg --overlay-ports=..\GameNetworkingSockets\vcpkg_ports install gamenetworkingsockets[core,libsodium]
        ```

        if openssl fails to compile.

4. Install vcpkg integrate so that Visual Studio will pick up installed packages

    ```commandline
    > vcpkg integrate install
    ```

5. Extract BML Dev pack and [Boost Library](https://www.boost.org/users/download/), and place the following:
    - *BML*:
        - include\BML -> BallanceMMOClient\include\BML
        - lib\Debug -> BallanceMMOClient\lib\BML\Debug
        - lib\Release -> BallanceMMOClient\lib\BML\Release
    - *Boost*:
        - boost -> BallanceMMOClient\include\boost

6. Open the Visual Studio project file `BallanceMMO.sln`.
    - DO NOT upgrade the project(cancel) when prompted! (Yet again, wtf Microsoft???)

7. In `Solution Explorer` panel, Right click on the project `BallanceMMOClient`, select `Properties`.

8. Navigate to `Configuration Properties` - `General` - `Platform toolset`, select your toolset of your choice (tested under `v142`(Visual Studio 2019)/`v143`(Visual Studio 2022)), then press `OK`.

9. Build the project!
