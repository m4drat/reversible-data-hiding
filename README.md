# Reversible Data Hiding (c++ implementation)

### How to build

`cmake --build`

### How to use

1. Example encryption: `reversible_data_hiding_run.exe --mode encrypt --image-path ..\..\..\..\images\boat.bmp --result-path ..\..\..\..\images\boat-enc.bmp --enc-key-file ..\..\..\..\example_key.bin`  
![encrypted](./images/boat-enc.bmp)

2. Example decryption: `reversible_data_hiding_run.exe --mode decrypt --image-path ..\..\..\..\images\boat-enc.bmp --result-path ..\..\..\..\images\boat-dec.bmp --enc-key-file ..\..\..\..\example_key.bin`  
![decrypted](./images/boat-dec.bmp)

### Performance

- Default realization (pixel-by-pixel encryption):

```none
-------------------------------------------------------------------------
Benchmark                               Time             CPU   Iterations
-------------------------------------------------------------------------
EncryptAirplane_512x512_bench        3.32 ms         3.25 ms          154
EncryptCrowd_512x512_bench           3.30 ms         3.29 ms          204
EncryptMan_512x512_bench             3.25 ms         2.98 ms          236
EncryptBoat_512x512_bench            3.26 ms         3.18 ms          187
EncryptLiberty_1024x1024_bench       13.1 ms         13.2 ms           64
EncryptMan_2048x2048_bench           52.1 ms         51.6 ms           10
EncryptMan_4096x4096_bench            212 ms          214 ms            3
```

- Threaded realization:

```none
-------------------------------------------------------------------------
Benchmark                               Time             CPU   Iterations
-------------------------------------------------------------------------
EncryptAirplane_512x512_bench        13.6 ms         2.42 ms          265
EncryptCrowd_512x512_bench           13.2 ms         1.72 ms          299
EncryptMan_512x512_bench             13.2 ms         1.74 ms          448
EncryptBoat_512x512_bench            13.3 ms         1.59 ms          747
EncryptLiberty_1024x1024_bench       15.7 ms         1.72 ms          299
EncryptMan_2048x2048_bench           28.7 ms         4.53 ms          100
EncryptMan_4096x4096_bench           74.6 ms         11.8 ms           41
```

- Hybrid approach

```none
-------------------------------------------------------------------------
Benchmark                               Time             CPU   Iterations
-------------------------------------------------------------------------
EncryptAirplane_512x512_bench        3.57 ms         3.20 ms          166
EncryptCrowd_512x512_bench           3.37 ms         3.44 ms          236
EncryptMan_512x512_bench             3.38 ms         3.53 ms          195
EncryptBoat_512x512_bench            3.40 ms         3.49 ms          179
EncryptLiberty_1024x1024_bench       13.8 ms         13.4 ms           50
EncryptMan_2048x2048_bench           29.8 ms         6.00 ms          112
EncryptMan_4096x4096_bench           74.1 ms         11.7 ms           56
```

- Possible micro optimizations:
  - ~~Divide image into blocks, and allow each thread-worker to work with each part of the image simultaneously~~
  - ~~Intrinsics and vector instructions~~ (didn't work) (info links are commented)
    <!---
    - https://software.intel.com/content/www/us/en/develop/articles/introduction-to-intel-advanced-vector-extensions.html
    - https://software.intel.com/sites/landingpage/IntrinsicsGuide/
    - https://docs.microsoft.com/en-us/cpp/intrinsics/x64-amd64-intrinsics-list?view=msvc-160
    - http://sci.tuomastonteri.fi/programming/sse
    - https://stackoverflow.com/questions/1389712/getting-started-with-intel-x86-sse-simd-instructions
    - https://users.ece.cmu.edu/~franzf/teaching/slides-18-645-simd.pdf
    -->  
- ~~replace `std::vector<std::vector<Color8>>` with just one-dim matrix (cache optimizations)~~ (already done by compiler in release build)

### Program modules

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

### Libraries

- BOOST - program options
- Cimg - BMP opening/saving/showing
- eigen - math operations
- gtest - google testing library
- google benchmark - benchmarking library
