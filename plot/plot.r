setwd("/home/kallyous/Projects/HuffmanOcool/plot")
abb_avl <-read.table("Plot.txt",header=TRUE)
attach(abb_avl)
plot(VALUE,ABB,type='h',col="blue",ann = FALSE)
points(VALUE,AVL,type='h',col = "red",ann = FALSE)
title(main="ABB vs AVL",col.main = "red",font.main = 4)
title(xlab="Número Buscado",col.lab=rgb(0,0.5,0))
title(ylab="Comparações",col.lab=rgb(0,0.5,0))
legend("topright", legend=c("ABB","AVL"),col =c("blue","red"),lty=1,lwd=2)