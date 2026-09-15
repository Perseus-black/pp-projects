#include <cstdlib>

int main()
{
    system(
        "gnuplot -persist -e \""
        "set xrange [-10:10]; "
        "set yrange [-10:10]; "
        "set xzeroaxis; "
        "set yzeroaxis; "
        "set grid; "
        "set xlabel 'x'; "
        "set ylabel 'y'; "
        "set title 'Cartesian Coordinate System'; "
        "plot "
        "x title 'y = x', "
        "-x title 'x = -y', "
        "x**2 title 'y = x^2', "
        "x+1 title 'y = x + 1', "
        "x**2+2 title 'y = x^2 + 2'"
        "\""
    );

    return 0;
}