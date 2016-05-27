#include <systemc.h>

SC_MODULE(foo) {
    sc_in <bool> i;
    sc_out<bool> o;
    SC_CTOR(foo):i("i"),o("o") { }
};

SC_MODULE(bar) {
    sc_in <bool> i;
    sc_out<bool> o;
    SC_CTOR(bar):i("i"),o("o") { }
};

SC_MODULE(foobar) {
    // entrée/sortie
    sc_in <bool> i;
    sc_out<bool> o;
    // interne
    sc_signal<bool> s;
    // sous modules
    foo foo_i;
    bar bar_i;

    SC_CTOR(foobar)
        :i("i"),o("o"),s("s"),
        foo_i("foo"), bar_i("bar")
    {
        // connexions aux I/O
        foo_i.i(i);
        bar_i.o(o);
        // connexion interne
        foo_i.o(s);
        bar_i.i(s);
    }
};

int sc_main(int argc, char * argv[]) {
    sc_signal<bool> i("i");
    sc_signal<bool> o("o");

    foobar uut("foobar");

    uut.i(i);
    uut.o(o);

    return 0;
}
