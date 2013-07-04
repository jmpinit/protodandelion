convert -rotate 90 $1 $( basename $1 .bmp )_90.bmp
convert -rotate 180 $1 $( basename $1 .bmp )_180.bmp
convert -rotate 270 $1 $( basename $1 .bmp )_270.bmp
mv $1 $( basename $1 .bmp)_0.bmp
