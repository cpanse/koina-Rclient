# Rpkg client for DLOmix service

## FAQ
### how to run the server?

#### Option 1
follow the install:
github.com:wilhelm-lab/koina.git

```
docker compose up serving -d
docker compose logs serving -f
```

#### Option 2

use a public one, e.g., dlomix.fgcz.uzh.ch:8080

### how to set the library path?

```
$ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/cpanse/src/v2.34.0dev.clients/lib/


cpanse@fgcz-h-480:~/src/github.com/cpanse/dlomix-Rclient  (main)> ldd dlomix 
        linux-vdso.so.1 (0x00007ffcbfba0000)
        libgrpcclient.so => /home/cpanse/src/v2.34.0dev.clients/lib/libgrpcclient.so (0x00007fa98cd29000)
        libstdc++.so.6 => /lib/x86_64-linux-gnu/libstdc++.so.6 (0x00007fa98cb51000)
        libgcc_s.so.1 => /lib/x86_64-linux-gnu/libgcc_s.so.1 (0x00007fa98cb37000)
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007fa98c962000)
        libz.so.1 => /lib/x86_64-linux-gnu/libz.so.1 (0x00007fa98c945000)
        libssl.so.1.1 => /lib/x86_64-linux-gnu/libssl.so.1.1 (0x00007fa98c8b2000)
        libcrypto.so.1.1 => /lib/x86_64-linux-gnu/libcrypto.so.1.1 (0x00007fa98c5bb000)
        libpthread.so.0 => /lib/x86_64-linux-gnu/libpthread.so.0 (0x00007fa98c599000)
        libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007fa98c455000)
        /lib64/ld-linux-x86-64.so.2 (0x00007fa98d654000)
        libdl.so.2 => /lib/x86_64-linux-gnu/libdl.so.2 (0x00007fa98c44f000)

```
see also [INSTALL](INSTALL)

### how to set the right data type?

https://github.com/triton-inference-server/server/blob/main/docs/user_guide/model_configuration.md


### how to get the Rcpp example to work?

```
#R

##  otherwise the library can not resolve all entries!
Sys.setenv("PKG_CXXFLAGS"="-I/home/cpanse/src/v2.34.0dev.clients/include/ -Wall -pedantic -std=c++17 -O3")
Sys.setenv("PKG_LIBS"="-L /home/cpanse/src/v2.34.0dev.clients/lib/ -lgrpcclient")

Rcpp::sourceCpp("src/rcpp_alphapept.cpp ", verbose = TRUE, rebuild = TRUE, cacheDir = "dlomixRcpp")
## call 
out <- dlomix_AlphaPept_ms2_generic_ensemble("ELVISK")
```

## links

- https://github.com/eubic/EuBIC2023/issues/12
- https://koina.proteomicsdb.org/
