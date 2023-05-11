#R


validateComposeProsit2019SpecLibrary <- function(x){
  ## TODO
  x
}

#' Compose a specLibrary of a given peptide sequence
#'
#' @inheritParams prosit2019IntensityEnsemble
#' @importFrom protViz fragmentIon
#' @description
#' define some R helper function for generating a specLibrary file as it 
#' can be read by DIANN.
#' @return a specLib \code{data.frame}.
#' @export
composeProsit2019SpecLibrary <- function(peptide,
                                     precursorCharge = 3,
                                     collisionEnergy = 0.35){
  ## predict MS2 and rt 
  p <- dlomix::prosit2019IntensityEnsemble(peptide,
                                           precursorCharge = precursorCharge,
                                           collisionEnergy = collisionEnergy)
  
  iRT <- dlomix::prosit2019IrtEnsemble(peptide)
  
  ## derive ground truth
  n <- nchar(peptide)
  yIons <- protViz::fragmentIon(peptide)[[1]]$y[seq(1, n-1)]
  bIons <- protViz::fragmentIon(peptide)[[1]]$b[seq(1, n-1)]
  names(yIons) <- paste0('y', seq(1, n-1), "(1+)")
  names(bIons) <- paste0('b', seq(1, n-1), "(1+)")
  
  
  ## decode Prosit output vector 
  intensity <- p[seq(1, 6 * n)]
  mZ <- p[175 + seq(1, 6 * n)]
  
  
  ## compose specLibrary data.frame
  PrecursorMz <- (yIons[1] + bIons[nchar(peptide) - 1] + 1.007) / precursorCharge
  
  ## decode Prosit output vector 
  y1Idx <- seq(1, 6 * (n-1), by = 6)
  b1Idx <- y1Idx + 3
  
  names(y1Idx) <- paste0("y", seq(1,n-1), "^1")
  names(b1Idx) <- paste0("b", seq(1,n-1), "^1")
  
  ## sanity check
  dError <- abs((((p[175 + y1Idx[1]] + p[175 + b1Idx[n-1]])+ 1.007) / precursorCharge) - PrecursorMz)
  if (abs(dError) > 0.0001) warning(paste0("precursorMz difference ", dError, " is to high!"))
  
  df <- data.frame(
    PrecursorMz = rep(PrecursorMz, 2 * (n-1)),
    ProductMz = p[175 + c(y1Idx, b1Idx)],
    Annotation = c(names(y1Idx), names(b1Idx)),
    ProteinId = NA,
    GeneName = NA,
    PeptideSequence = rep(peptide, 2 * (n-1)),
    ModifiedPeptideSequence = rep(peptide, 2 * (n-1)),
    PrecursorCharge = rep(precursorCharge, 2 * (n-1)),
    LibraryIntensity = p[c(y1Idx, b1Idx)], 
    NormalizedRetentionTime = iRT,
    PrecursorIonMobility = NA,
    FragmentType = c(rep('y', n-1), rep('b', n-1)),
    FragmentCharge = rep(1, 2*(n-1)),
    FragmentSeriesNumber = rep(seq(1, n-1),2),
    FragmentLossType = NA,
    AverageExperimentalRetentionTime = 100 * iRT
  ) 
    
  # class(df) <- "Prosit2019SpecLibrary"
  validateComposeProsit2019SpecLibrary(df)
}
