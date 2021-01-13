# Reversible Data Hiding (c++ implementation)

#### How to build

#### How to use

#### Performance

#### Program modules
- Image matrix Class
- Image Loader - Loads bmp image and converts it to the matrix of colours
- Encryptor - Encrypts image using XOR encryption
- Data Hider
  - Pre-encoder
  - Block Classificator
  - Compressor (RLC + LSB)
  - Embedder
- Data Extractor
  - Data Extractor
  - Image Decryptor
  - Image Recoverer

#### Additional program modules
- Benchmarks
- Tests
- GUI - ???

#### Libraries
- Cimg
- eigen - math operations
    - http://eigen.tuxfamily.org/dox/
    - https://gitlab.com/libeigen/eigen/-/releases/3.3.9
- testing???
- benchmarking???
- Graphing (mb python + jupyter) (speed results comparisons)
- https://github.com/jupyter-xeus/xeus-cling
- https://blog.jupyter.org/interactive-workflows-for-c-with-jupyter-fe9b54227d92
- https://blog.llvm.org/posts/2020-12-21-interactive-cpp-for-data-science/