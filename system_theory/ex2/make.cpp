#include <cstdlib>
#include <iostream>

int main() {
    system("gcc prog-gen.c -o pg -g -std=c99");
    system("./pg test");
    system("gcc simple-computer.c -o scop -g -std=c99");
    system("./scop test 0");
    return 0;
}