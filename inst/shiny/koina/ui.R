#
# This is the user-interface definition of a Shiny web application. You can
# run the application by clicking 'Run App' above.
#
# Find out more about building applications with Shiny here:
#
#    http://shiny.rstudio.com/
#

library(shiny)

# Define UI for application that draws a histogram
shinyUI(fluidPage(

    # Application title
    titlePanel("koina Prosit 2019"),

    # Sidebar with a slider input for number of bins
    sidebarLayout(
        sidebarPanel(
            selectInput(
                "peptide",
                "peptide",
                c("LKEATIQLDELNQK", "YDSAPATDGSGTALGWTVAWK", "AGVNNGNPLDAVQQ",
                  "IEEALGDKAVFAG[UNIMOD:213]K",
                  "DLEEATLQH[UNIMOD:213]ATAAALR",
                  "AAAAAKAKM[UNIMOD:35]","AAAAAKAKM")
            ),
            sliderInput("collisionEnergy",
                        "collision Energy [eV]",
                        min = 15,
                        max = 35,
                        value = 30),
            radioButtons(
                "charge",
                "charge",
                choices =  c("2+" = "2",
                             "3+" = "3"
                             )
            ),
            selectInput(
                "url",
                "url",
                c("koina.proteomicsdb.org:443",
                  "dlomix.fgcz.uzh.ch:8080")
            ),
            checkboxInput("usessl", "use SSL", value = TRUE, width = NULL)
        ),
       

        # Show a plot of the generated distribution
        mainPanel(
            plotOutput("distPlot")
        )
    )
))
