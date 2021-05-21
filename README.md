# Reversible Data Hiding (c++ implementation)

Implementation for technique described in this paper

### How to build

On windows:
```
cmake -G "Visual Studio 16 2019" -S . -B build -DBENCHMARK_ENABLE_GTEST_TESTS=OFF -DBOOST_ROOT=<path to boost root> -DBOOST_LIBRARYDIR=<boost library dir>
cmake --build build --config Release
```

### Examples

1. Example encryption: `rdh.exe --mode encrypt --image-path ./lena_gray.bmp --result-path ./images/lena_gray-enc.bmp --enc-key-file ./example_encrypt_key.bin`  

2. Example decryption: `rdh.exe --mode decrypt --image-path ./lena_gray-enc.bmp --result-path ./images/lena_gray-dec.bmp --enc-key-file ./example_encrypt_key.bin`  

3. Example data embedding: `rdh.exe --mode embed --image-path ./lena_gray-enc.bmp --result-path ./images/lena_gray-enc-embed.bmp --embed-key-file ./example_embed_key.bin --data-file ./example_data_to_embed.bin`

4. Example data extraction: `./rdh.exe --mode extract --image-path ./marked-encrypted.bmp --result-path-data ./extracted.bin --embed-key FFDDEE`  

5. Example image recovery: `rdh.exe --mode extract --image-path ./marked-encrypted.bmp --result-path ./extracted.bmp --encryption-key AABBCC`  

6. Example data extraction and original image recovery: `rdh.exe --mode extract --image-path ./marked-encrypted.bmp --result-path ./extracted.bmp --result-path-data ./extracted.bin --encryption-key AABBCC --embed-key FFDDEE`

7. Example SSIM calculation: `rdh.exe --mode ssim --image-path ./recovered_image_man.bmp --second-image ./man_original.bmp`

8. Example PSNR calculation: `rdh.exe --mode psnr --image-path ./recovered_image_man.bmp --second-image ./man_original.bmp`

### Performance

### PSNR & SSIM

### Dependencies

- BOOST - program_options, dynamic_bitset, sha1
- Cimg - BMP opening/saving/showing
- eigen - math operations
- gtest - google testing library
- google benchmark - benchmarking library
