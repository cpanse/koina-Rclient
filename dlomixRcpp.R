#R
options(prompt = "R> ", continue = "+  ", width = 70, useFancyQuotes = FALSE)

Sys.setenv("PKG_CXXFLAGS"="-I/home/cp/src/v2.34.0dev.clients/include/ -Wall -pedantic -std=c++17 -O3")
Sys.setenv("PKG_LIBS"="-L /home/cp/src/v2.34.0dev.clients/lib/ -lgrpcclient")

Rcpp::sourceCpp("dlomixRcpp.cc", verbose = TRUE, rebuild = TRUE, cacheDir = "dlomixRcpp")

out <- dlomix_AlphaPept_ms2_generic_ensemble("LGGNEQVTR", FALSE) 
round(out * 100)
