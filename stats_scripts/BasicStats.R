require(ggplot2)
require(scales)

#These are attractive and color-blind friendly colors to use
#Could also use the viridis package: https://cran.r-project.org/web/packages/viridis/vignettes/intro-to-viridis.html
colors <- c("#673F03", "#B50142","#AB08FF","#7B1DFF", "#5731FD","#5E8EFF", "#4755FF" ,"#6FC4FE", "#86E9FE", "#96FFF7", "#B2FCE3", "#BBFFDB", "#D4FFDD", "#EFFDF0")

#Set your working directory
# TODO: Change PATH_TO_YOUR_FOLDER
setwd("/Users/cslab/Desktop/final-project-kai-sam-and-vic/Example/")

#You can have whatever variables you want like this
pop_cap <- 10000

#Read in your data
initial_data <- read.table("munged_SizeChange30x30ReplicationDataSEED10.dat", h=T)


#You can add extra columns with calculations with cbind
initial_data <-cbind(initial_data, hist_0_prop=initial_data$hist_0/initial_data$count, Treatment=as.factor(initial_data$treatment))

#You can get just a portion of your data with subset
early <- subset(initial_data, update<2000 & update >= 0)
postExtinction <- subset(initial_data, update>99000 & update < 101000)
just_middle <- subset(initial_data, treatment==0.5)

#If you have NaN values that you would like to be 0, this is how
zeroed <- initial_data
zeroed[is.na(zeroed)] <- 0


#This will make a line plot over time
#It's not very exciting currently...
ggplot(data=initial_data, aes(x=update, y=tasks_solved)) + ylab("Total Number of Tasks Solved") + xlab("Updates") + stat_summary(fun.data="mean_cl_boot", geom=c("smooth"), se = TRUE)
update_1 <- subset(initial_data, update==100)

# This will make a point plot
ggplot(data=full, aes(x=update, y=tasks_solved)) +ylab("Mean Cooperation Value") + xlab("Number of Organisms") + geom_point() + scale_colour_manual(values=colors)

#Box plot
ggplot(data=early, aes(x=update, y=tasks_solved)) + geom_boxplot() 

# Non-parametric test to see if two sets of data are probably different
wilcox.test(subset(update_1, treatment==1.0)$coop, subset(update_1, treatment=0.5)$coop)
