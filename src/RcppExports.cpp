// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

#ifdef RCPP_USE_GLOBAL_ROSTREAM
Rcpp::Rostream<true>&  Rcpp::Rcout = Rcpp::Rcpp_cout_get();
Rcpp::Rostream<false>& Rcpp::Rcerr = Rcpp::Rcpp_cerr_get();
#endif

// alphaPeptMs2GenericEnsemble
Rcpp::NumericVector alphaPeptMs2GenericEnsemble(Rcpp::StringVector peptide, float collisionEnergy, int precursorCharge, int instrument, bool verbose, std::string url);
RcppExport SEXP _dlomix_alphaPeptMs2GenericEnsemble(SEXP peptideSEXP, SEXP collisionEnergySEXP, SEXP precursorChargeSEXP, SEXP instrumentSEXP, SEXP verboseSEXP, SEXP urlSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< Rcpp::StringVector >::type peptide(peptideSEXP);
    Rcpp::traits::input_parameter< float >::type collisionEnergy(collisionEnergySEXP);
    Rcpp::traits::input_parameter< int >::type precursorCharge(precursorChargeSEXP);
    Rcpp::traits::input_parameter< int >::type instrument(instrumentSEXP);
    Rcpp::traits::input_parameter< bool >::type verbose(verboseSEXP);
    Rcpp::traits::input_parameter< std::string >::type url(urlSEXP);
    rcpp_result_gen = Rcpp::wrap(alphaPeptMs2GenericEnsemble(peptide, collisionEnergy, precursorCharge, instrument, verbose, url));
    return rcpp_result_gen;
END_RCPP
}
// prosit2019Intensity
Rcpp::List prosit2019Intensity(Rcpp::StringVector peptide, Rcpp::NumericVector collisionEnergy, Rcpp::NumericVector precursorCharge, bool verbose, bool usessl, std::string url);
RcppExport SEXP _dlomix_prosit2019Intensity(SEXP peptideSEXP, SEXP collisionEnergySEXP, SEXP precursorChargeSEXP, SEXP verboseSEXP, SEXP usesslSEXP, SEXP urlSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< Rcpp::StringVector >::type peptide(peptideSEXP);
    Rcpp::traits::input_parameter< Rcpp::NumericVector >::type collisionEnergy(collisionEnergySEXP);
    Rcpp::traits::input_parameter< Rcpp::NumericVector >::type precursorCharge(precursorChargeSEXP);
    Rcpp::traits::input_parameter< bool >::type verbose(verboseSEXP);
    Rcpp::traits::input_parameter< bool >::type usessl(usesslSEXP);
    Rcpp::traits::input_parameter< std::string >::type url(urlSEXP);
    rcpp_result_gen = Rcpp::wrap(prosit2019Intensity(peptide, collisionEnergy, precursorCharge, verbose, usessl, url));
    return rcpp_result_gen;
END_RCPP
}
// prosit2019IrtEnsemble
Rcpp::NumericVector prosit2019IrtEnsemble(Rcpp::StringVector peptide, bool verbose, std::string url);
RcppExport SEXP _dlomix_prosit2019IrtEnsemble(SEXP peptideSEXP, SEXP verboseSEXP, SEXP urlSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< Rcpp::StringVector >::type peptide(peptideSEXP);
    Rcpp::traits::input_parameter< bool >::type verbose(verboseSEXP);
    Rcpp::traits::input_parameter< std::string >::type url(urlSEXP);
    rcpp_result_gen = Rcpp::wrap(prosit2019IrtEnsemble(peptide, verbose, url));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_dlomix_alphaPeptMs2GenericEnsemble", (DL_FUNC) &_dlomix_alphaPeptMs2GenericEnsemble, 6},
    {"_dlomix_prosit2019Intensity", (DL_FUNC) &_dlomix_prosit2019Intensity, 6},
    {"_dlomix_prosit2019IrtEnsemble", (DL_FUNC) &_dlomix_prosit2019IrtEnsemble, 3},
    {NULL, NULL, 0}
};

RcppExport void R_init_dlomix(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
