
package require Img

proc p {} {
    foreach i [glob *-thumb.png] {
        set p [image create photo -file $i]
        regexp {(.*)-thumb.png} $i _ o
        set w [image width $p]
        set h [image height $p]
        
        puts "$i: ${w}x${h}"
        
        if {$w != $h} {
            puts "###################"
        }
        
        $p write ../${o}.bmp -format bmp
        image delete $p
    }
}
p

exit
