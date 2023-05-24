# orbitersdk

Sparq subnet source code.

If you are a developer, fill this form out for free support and additional incentives: https://forms.gle/m83ceG3XoJY3fpwU9

## Requirements

* **GCC** with support for **C++20** or higher
* **CMake 3.19.0** or higher
* **Boost 1.74** or higher (components: *chrono, filesystem, program-options, system, thread, nowide*)
* **OpenSSL 1.1.1**
* **CryptoPP 8.2.0** or higher
* **libscrypt**
* **zlib**
* (optional) **clang-tidy** for linting

If building with support for AvalancheGo, you will also need the following:

* **Abseil (absl)**
* **gRPC**
* **Protobuf 3.12** or higher
* **libc-ares**

### One-liners

For **Debian 11 Bullseye or newer**:
* `sudo apt install build-essential cmake tmux clang-tidy autoconf libtool pkg-config libabsl-dev libboost-all-dev libc-ares-dev libcrypto++-dev libgrpc-dev libgrpc++-dev libscrypt-dev libssl-dev zlib1g-dev openssl protobuf-compiler protobuf-compiler-grpc`

#### Caveats

* **Debian 11 Bullseye and older**: CMake version from repos is too old (3.18.4), has to be installed manually from [their website](https://cmake.org/download)

## Documentation

We use [Doxygen](https://www.doxygen.nl/index.html) to generate documentation over the current source code. Run `doxygen` inside the project's root folder. Docs should be inside `docs/html`.

For a more detailed explanation of the project's structure, check the [docs](https://github.com/SparqNet/sparq-docs/tree/main/Sparq_en-US) repository.

## Compiling

* Clone the project: `git clone https://github.com/SparqNet/orbitersdk-cpp
* Go to the project's root folder, create a "build" folder and change to it:
  * `cd orbitersdk && mkdir build && cd build`
* Run `cmake` inside the build folder: `cmake ..`
  * Use `-DCMAKE_BUILD_TYPE={Debug,RelWithDebInfo,Release}` to set the respective debug/release builds (Debug by default)
  * Use `-DDEBUG=OFF` to build without debug flags (ON by default)
  * Use `-DUSE_LINT=ON` to run clang-tidy along the build (OFF by default, WILL TAKE SIGNIFICANTLY LONGER TO COMPILE)
* Build the executable: `cmake --build . -- -j$(nproc)`
  * If using the linter, pipe stderr to a file (e.g. `cmake --build . -- -j$(nproc) 2> log.txt`)

## Deploying

Run the `./scripts/AIO-setup.sh` script in the project's root folder. It will:

* Automatically build the project with the default configuration (and added contracts if properly linked, see documentation on how to create a contract)  
* Setup, deploy and start the blockchain with *five* Validator nodes, *six* normal nodes and *one* Discovery node, all of them acting as rdPoS

You can attach tmux sessions to check if everything is running (e.g. `tmux a -t local_testnet_validator1` will attach to the first Validator node - see more below).

There are some flags that can be used with the script to customize deployment:

| Flag | Description | Default Value |
|------|-------------|---------------|
| --clean | Clean the build folder before building | false |
| --no-deploy | Only build the project, don't deploy the network | false |
| --debug=\<bool\> | Build with debug flags | true |
| --cores=\<int\> | Number of cores to use for building | Maximum available |

Example: `bash ./scripts/AIO-setup.sh --clean --no-deploy --debug=false --cores=4`

This will clean the build folder, only build the project (not deploy it), in release mode, and use 4 cores for building. Remember that GCC uses around 1.5GB of RAM per core, so it is recommended to adjust the number of cores according to the available RAM to maintain stability.

After the script finishes, you can connect with your favorite Ethereum wallet or RPC client to the deployed blockchain using its respective default RPC URL, port and ChainID.

Executing the script again will stop the current network and deploy a new one. This means you must reset your wallet or RPC client if needed (for example, MetaMask keeps track of account nonces - a network reset would set back the nonce to 0, so you'd also have to [reset it inside MetaMask](https://support.metamask.io/hc/en-us/articles/360015488891-How-to-clear-your-account-activity-reset-account)).

## Defaults for local testnet chain

The deployed chain will have the following information by default:

* ChainID: **808080**
* Owner: **0x00dead00665771855a34155f5e7405489df2c3c6**
* Owner Private Key: **0xe89ef6409c467285bcae9f80ab1cfeb3487cfe61ab28fb7d36443e1daa0c2867**
* Owner Initial Balance: **1000000000000000000000 wei**
* ContractManager Address: **0x0001cb47ea6d8b55fe44fdd6b1bdb579efb43e61**
* rdPoS Address: **0xb23aa52dbeda59277ab8a962c69f5971f22904cf**
* Default RPC URL: **http://127.0.0.1:8090**

Nodes are all deployed on the same machine, under the following ports and tmux sessions:

| Session Name             | Node Type | P2P Port | HTTP Port | Validator Key                                                      |
|--------------------------|-----------|----------|-----------|--------------------------------------------------------------------|
| local_testnet_discovery  | Discovery | 8080     | 8090      | XXXX                                                               |
| local_testnet_validator1 | Validator | 8081     | 8090      | 0xba5e6e9dd9cbd263969b94ee385d885c2d303dfc181db2a09f6bf19a7ba26759 |
| local_testnet_validator2 | Validator | 8082     | 8091      | 0xfd84d99aa18b474bf383e10925d82194f1b0ca268e7a339032679d6e3a201ad4 |
| local_testnet_validator3 | Validator | 8083     | 8092      | 0x66ce71abe0b8acd92cfd3965d6f9d80122aed9b0e9bdd3dbe018230bafde5751 |
| local_testnet_validator4 | Validator | 8084     | 8093      | 0x856aeb3b9c20a80d1520a2406875f405d336e09475f43c478eb4f0dafb765fe7 |
| local_testnet_validator5 | Validator | 8085     | 8094      | 0x81f288dd776f4edfe256d34af1f7d719f511559f19115af3e3d692e741faadc6 |
| local_testnet_normal1    | Normal    | 8086     | 8095      | XXXX                                                               |
| local_testnet_normal2    | Normal | 8087     | 8096      | XXXX |
| local_testnet_normal3    | Normal | 8088     | 8097      | XXXX |
| local_testnet_normal4    | Normal | 8089     | 8098      | XXXX |
| local_testnet_normal5    | Normal | 8110     | 8099      | XXXX |
| local_testnet_normal6    | Normal | 8111     | 8100      | XXXX |

