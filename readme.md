## Web Assembly
Instructions how to build [wasm](https://webassembly.github.io/) binaries on Windows.

* Install Python.
* Install Visual Studio 2015 with **Git**, **Python**, **Java** and **Node.js**.
* Add `C:\Program Files (x86)\Java\jdk1.7.0_55\bin` to `Path`.
* Add `C:\Program Files (x86)\nodejs` to `Path`.

Verify the requirements.

```cmd
python --version
node -v
javac -version
git --version
```

Create the directories mkdir `C:\Web` and `C:\Web\temp`.

## Fastcomp
<http://kripken.github.io/emscripten-site/docs/building_from_source/LLVM-Backend.html>

```cmd
cd C:\Web
git clone -b incoming --depth 1 https://github.com/kripken/emscripten-fastcomp llvm
git clone -b incoming --depth 1 https://github.com/kripken/emscripten-fastcomp-clang llvm/tools/clang
mkdir llvm\build
cd llvm\build
cmake .. -G "Visual Studio 14 2015 Win64" -DCMAKE_INSTALL_PREFIX=.. ^
  -DCMAKE_BUILD_TYPE=Release -DLLVM_TARGETS_TO_BUILD="X86;JSBackend" ^
  -DLLVM_EXPERIMENTAL_TARGETS_TO_BUILD="WebAssembly" -DLLVM_INCLUDE_EXAMPLES=OFF ^
  -DLLVM_INCLUDE_TESTS=OFF -DCLANG_INCLUDE_EXAMPLES=OFF -DCLANG_INCLUDE_TESTS=OFF
cmake --build . --target install --config Release
```

Repeat the last command after fixing JSBackend NaCl bugs.

Add `C:\Web\llvm\bin` to `Path`.

## Emscripten SDK
<http://emscripten.org/>

```cmd
cd C:\Web
git clone -b incoming --depth 1 https://github.com/kripken/emscripten emsdk
```

Add `C:\Web\emsdk` to `Path`. Execute `em++` once and modify the settings in `%UserProfile%\.emscripten`.

```py
EMSCRIPTEN_ROOT = os.path.expanduser(os.getenv('EMSCRIPTEN') or 'C:\\Web\\emsdk') # directory
LLVM_ROOT = os.path.expanduser(os.getenv('LLVM') or 'C:\\Web\\llvm\\bin') # directory
BINARYEN_ROOT = os.path.expanduser(os.getenv('BINARYEN') or 'C:\\Web\\binaryen') # directory
NODE_JS = os.path.expanduser(os.getenv('NODE') or 'C:\\Program Files (x86)\\nodejs\\node.exe') # executable
SPIDERMONKEY_ENGINE = [os.path.expanduser(os.getenv('SPIDERMONKEY') or 'js')] # executable
V8_ENGINE = os.path.expanduser(os.getenv('V8') or 'd8') # executable
JAVA = 'java' # executable
TEMP_DIR = 'C:\\Web\\temp'
COMPILER_ENGINE = NODE_JS
JS_ENGINES = [NODE_JS]
```

## Binaryen
<https://github.com/WebAssembly/binaryen>

```cmd
cd C:\Web
git clone --depth 1 https://github.com/WebAssembly/binaryen binaryen
mkdir binaryen\build
cd binaryen\build
cmake .. -G "Visual Studio 14 2015 Win64" -DCMAKE_INSTALL_PREFIX=.. -DBUILD_STATIC_LIB=ON
cmake --build . --target install --config Release
```

Add `C:\Web\binaryen\bin` to `Path`.

## Test
Compile the file [main.cc](main.cc) with the following command and use [index.html](index.html) to load it.

```cmd
em++ -Os -std=c++14 -s BINARYEN=1 -s BINARYEN_METHOD='native-wasm' ^
  --memory-init-file 1 -s ELIMINATE_DUPLICATE_FUNCTIONS=1 -s MODULARIZE=1 ^
  -s EXPORT_NAME='main' -s NO_EXIT_RUNTIME=1 -s USE_GLFW=3 main.cc -o main.js
```
