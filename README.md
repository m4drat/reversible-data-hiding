# Reversible Data Hiding (c++ implementation)

#### How to build

#### How to use
1. Example encryption: `reversible_data_hiding_run.exe --mode encrypt --image-path ..\..\..\..\images\boat.bmp --result-path ..\..\..\..\images\boat-enc.bmp --enc-key-file ..\..\..\..\example_key.bin`
![encrypted](./images/boat-enc.bmp)

2. Example decryption: `reversible_data_hiding_run.exe --mode decrypt --image-path ..\..\..\..\images\boat-enc.bmp --result-path ..\..\..\..\images\boat-dec.bmp --enc-key-file ..\..\..\..\example_key.bin`
![decrypted](./images/boat-dec.bmp)



#### Performance
Current results (no microoptimizations):

```
----------------------------------------------------------------
Benchmark                      Time             CPU   Iterations
----------------------------------------------------------------
EncryptAirplane_bench   27939076 ns     29871324 ns           34
EncryptCrowd_bench      28414308 ns     27500000 ns           25
EncryptMan_bench        28586656 ns     26875000 ns           25
EncryptBoat_bench       28063516 ns     28750000 ns           25
```

- Possible microoptimisations:
  - parallel algorithms (on image iteration (foreach))
  - Divide image into blocks, and allow each thread-worker to work with each part of the image simultaniously
  - Intrinsics and vector instructions
  - replace `std::vector<std::vector<Color8>>` with just one-dim matrix (cache optimisations)

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