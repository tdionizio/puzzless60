for p in *-ibase.png; do 
	B=`echo $p | sed -e 's/-ibase.png//'`
	convert -verbose $p -thumbnail 60x60 $B-thumb.png
done
