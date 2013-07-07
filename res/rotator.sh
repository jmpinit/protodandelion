convert -rotate 90 $1 $( basename $1 .png )_90.png
convert -rotate 180 $1 $( basename $1 .png )_180.png
convert -rotate 270 $1 $( basename $1 .png )_270.png
mv $1 $( basename $1 .png )_0.png
