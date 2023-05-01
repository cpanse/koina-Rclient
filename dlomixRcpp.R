#R


Sys.setenv("PKG_CXXFLAGS"="-I/home/cpanse/src/v2.34.0dev.clients/include/ -Wall -pedantic -std=c++17 -O3")
Sys.setenv("PKG_LIBS"="-L /home/cpanse/src/v2.34.0dev.clients/lib/ -lgrpcclient")

Rcpp::sourceCpp("dlomixRcpp.cc", verbose = TRUE, rebuild = TRUE)


rv <- lapply(protViz::iRTpeptides[,1], dlomix_AlphaPept_ms2_generic_ensemble, verbose = FALSE) 


#swissprot.tryptic <- scan('/scratch/swissprot.peptides.txt', what=as.character())
#rv <- lapply(swissprot.tryptic, dlomix_AlphaPept_ms2_generic_ensemble, verbose = FALSE) 
