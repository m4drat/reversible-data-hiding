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
Default realisation (pixel-by-pixel encryption):
----------------------------------------------------------------
Benchmark                      Time             CPU   Iterations
----------------------------------------------------------------
EncryptAirplane_bench    3598250 ns      3576807 ns          166
EncryptCrowd_bench       3331209 ns      3044872 ns          195
EncryptMan_bench         3397647 ns      3605769 ns          195
EncryptBoat_bench        3334771 ns      3404018 ns          280
EncryptLiberty_bench    13529130 ns     13750000 ns           50
Load512x512_bench        1750459 ns      1778739 ns          448
Load514x514_bench        1283643 ns      1283482 ns          560
Load1024x1024_bench      6525012 ns      6556920 ns          112

Threaded realisation (Looks worse now):
-------------------------------------------------------------------------
Benchmark                               Time             CPU   Iterations
-------------------------------------------------------------------------
EncryptAirplane_512x512_bench        15.2 ms         2.56 ms          195
EncryptCrowd_512x512_bench           14.8 ms         1.95 ms          224
EncryptMan_512x512_bench             14.6 ms         1.51 ms          560
EncryptBoat_512x512_bench            14.1 ms         1.69 ms          407
EncryptLiberty_1024x1024_bench       17.3 ms         1.59 ms          373
Load512x512_bench                    1.68 ms         1.67 ms          448
Load514x514_bench                    1.26 ms         1.26 ms          498
Load1024x1024_bench                  8.08 ms         8.16 ms           90
```

- Possible microoptimisations:
  - parallel algorithms (on image iteration (foreach))
  - Divide image into blocks, and allow each thread-worker to work with each part of the image simultaniously
  - Intrinsics and vector instructions
  - ~~replace `std::vector<std::vector<Color8>>` with just one-dim matrix (cache optimisations)~~ (already done by compiler in release build)

#### Program modules
- ~~Image matrix Class~~
- ~~Image Loader - Loads bmp image and converts it to the matrix of colours~~
- ~~Encryptor - Encrypts image using XOR encryption~~
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