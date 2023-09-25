#
# This is the server logic of a Shiny web application. You can run the
# application by clicking 'Run App' above.
#
# Find out more about building applications with Shiny here:
#
#    http://shiny.rstudio.com/
#

lp <- file.path("/home/cpanse",
                "src/v2.34.0dev.clients/lib/libgrpcclient.so")
stopifnot(file.exists(lp))

dyn.load(lp)

library(shiny)

## define some R helper function ...
.plotPrositIntensity <- function(peptide = "LKEATIQLDELNQK",
                                 precursorCharge = 3,
                                 collisionEnergy = 35,
                                 url = "koina.proteomicsdb.org:443",
                                 usessl = TRUE,
                                 ...){
    
    
    op <- par(mfrow=c(1,1))
    p <- koina::prosit2019Intensity( peptide = peptide,
                                     collisionEnergy = collisionEnergy,
                                     precursorCharge = precursorCharge,
                                     url = url, usessl = usessl)[[1]]
    
    plot(p$mz, p$intensity, type='n', sub=url, main=peptide)
    bidx <- grepl("^b", p$annotation)
    yidx <- grepl("^y", p$annotation)
    
    points(p$mz[bidx], p$intensity[bidx], col='blue', type = 'h')
    
    points(p$mz[yidx], p$intensity[yidx], col='red', type = 'h')
    
    text(p$mz, p$intensity, p$annotation, cex=0.75)
    
    if(FALSE){
        try({
            p <- p[!is.na(p$mz),]
            n <- nchar(peptide)
            
            yIons <- protViz::fragmentIon(peptide)[[1]]$y[seq(1, n-1)]
            bIons <- protViz::fragmentIon(peptide)[[1]]$b[seq(1, n-1)]
            names(yIons) <- paste0('y', seq(1, n-1), "(1+)")
            names(bIons) <- paste0('b', seq(1, n-1), "(1+)")
            
            ## retrieve mZ and intensities 
            #intensity <- p[seq(1, 6 * n)]
            #mZ <- p[175 + seq(1, 6 * n)]
            
            idx <- order(p$mz)
            mZ <- p$mz[idx]
            intensity <- p$intensity[idx]
            
            ## determine b- and y-ion match
            yIdx <- protViz::findNN(yIons, mZ)
            yHit <- abs(mZ[yIdx] - yIons) < 0.1
            
            bIdx <- protViz::findNN(bIons, mZ)
            bHit <- abs(mZ[bIdx] - bIons) < 0.1
            
            plot(intensity ~ mZ, type = 'h', axes=FALSE, ...)
            
            points(mZ[yIdx[yHit]], intensity[yIdx[yHit]],
                   col = 'red', type = 'h', lwd = 2.0)
            
            points(mZ[bIdx[yHit]], intensity[bIdx[bHit]],
                   col = 'cornflowerblue', type = 'h', lwd = 2.0)
            
            axis(4)
            axis(3, mZ[yIdx[yHit]],  names(yHit[yHit]))
            axis(2)
            axis(1, mZ[bIdx[bHit]],  names(bHit[bHit]))
        })}
}



# Define server logic required to draw a histogram
shinyServer(function(input, output) {

    output$distPlot <- renderPlot({

        # draw the histogram with the specified number of bins
        .plotPrositIntensity(peptide=input$peptide,
                             collisionEnergy = input$collisionEnergy,
                             precursorCharge = as.integer(input$charge),
                             url = input$url, usessl = input$usessl)

    })

})
