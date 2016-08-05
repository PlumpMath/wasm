@echo off
echo cm++ -Oz -std=c++14 main.cc -o main.js
em++ -Oz -std=c++14 --memory-init-file 0 ^
  -s BINARYEN=1 -s BINARYEN_METHOD='native-wasm,asmjs' -s MODULARIZE=1 -s EXPORT_NAME='main' ^
  -s ELIMINATE_DUPLICATE_FUNCTIONS=1  -s NO_EXIT_RUNTIME=1 -s ASSERTIONS=0 ^
  -s USE_GLFW=3 -s FULL_ES3=1 ^
  main.cc -o main.js
if not %errorlevel% == 0 (
  pause
)