#R

validateComposeSpecLibrary <- function(x){
  ## TODO
  x[!is.na(x$ProductMz), ]
}

#' Compose a specLibrary of a given peptide sequence
#'
#' @inheritParams prosit2019Intensity
#' @param proteinId description
#' @param mS2prediction a nest list objects of data.frames
#' @importFrom protViz fragmentIon
#' @description
#' define some R helper function for generating a specLibrary file as it 
#' can be read by DIANN.
#' @return a specLib \code{data.frame}.
#' @export
.composeSpecLibrary <- function(peptide,
	proteinId = NA,
        precursorCharge = 3,
        collisionEnergy = 35,
	mS2prediction = NULL){
  
  #iRT <- dlomix::prosit2019IrtEnsemble(peptide)
  iRT <- protViz::ssrc(peptide) |> as.double()
  
  ## derive ground truth
  n <- nchar(peptide)
  yIons <- protViz::fragmentIon(peptide)[[1]]$y[seq(1, n-1)]
  bIons <- protViz::fragmentIon(peptide)[[1]]$b[seq(1, n-1)]
  names(yIons) <- paste0('y', seq(1, n-1), "(1+)")
  names(bIons) <- paste0('b', seq(1, n-1), "(1+)")
  
  
  ## decode Prosit output vector 
  intensity <- mS2prediction$intensity
  mZ <- mS2prediction$mz
  
  
  ## compose specLibrary data.frame
  PrecursorMz <- (yIons[1] + bIons[nchar(peptide) - 1] + 1.007) / precursorCharge
  
  ## decode Prosit output vector 
  y1Idx <- seq(1, 6 * (n-1), by = 6)
  b1Idx <- y1Idx + 3
  
  names(y1Idx) <- paste0("y", seq(1,n-1), "^1")
  names(b1Idx) <- paste0("b", seq(1,n-1), "^1")
  
  ## sanity check
  # dError <- abs((((p[175 + y1Idx[1]] + p[175 + b1Idx[n-1]])+ 1.007) / precursorCharge) - PrecursorMz)
  #if (abs(dError) > 0.0001) warning(paste0("precursorMz difference ", dError, " is to high!"))

  n <-  nrow(mS2prediction)
  
  df <- data.frame(
    PrecursorMz = rep(PrecursorMz, n),
    ProductMz = mS2prediction$mz,
    Annotation = gsub("+", "^", mS2prediction$annotation, fixed = TRUE),
    ProteinId = proteinId,
    GeneName = NA,
    PeptideSequence = rep(peptide, n),
    ModifiedPeptideSequence = rep(peptide, n),
    PrecursorCharge = rep(precursorCharge, n),
    LibraryIntensity = mS2prediction$intensity, 
    NormalizedRetentionTime = iRT,
    PrecursorIonMobility = NA,
    FragmentType = substr(mS2prediction$annotation, 1,1),
    FragmentCharge = substr(mS2prediction$annotation, nchar(mS2prediction$annotation), nchar(mS2prediction$annotation)),
    FragmentSeriesNumber = sapply(strsplit(mS2prediction$annotation, '[by+]', perl=TRUE) , function(x){x[2]}),
    FragmentLossType = NA,
    AverageExperimentalRetentionTime = 100 * rep(iRT, n)
  ) 
    
  # class(df) <- "Prosit2019SpecLibrary"
  validateComposeSpecLibrary(df)
}
