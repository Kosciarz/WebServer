# WebServer

Repository contains a simple web server written in C++ using Boost.Asio library.

## Setup Instructions

1. Clone the repository:

    ```bash
    git clone --recurse-submodules https://github.com/Kosciarz/WebServer.git
    ```

2. Run the bootstrap script to initialize all submodules and install dependencies:
    - On Windows:

      ```bash
      cd scripts
      ./bootstrap.bat
      ```

    - On Linux:

      ```bash
      cd scripts
      chmod +x ./bootstrap.sh
      ./bootstrap.sh
      ```

3. Once the bootstrap is complete, you can proceed with the build:

    ```bash
    mkdir build
    cd build
    cmake ..
    cmake --build .
    ```
