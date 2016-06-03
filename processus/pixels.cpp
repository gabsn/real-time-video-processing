#include <systemc.h>

struct Pixel {
    sc_uint<5> r;
    sc_uint<6> g;
    sc_uint<6> b;
    // un constructeur particulier avec des valeurs par défaut
    Pixel(sc_uint<5> _r=0, sc_uint<6> _g=0, sc_uint<5> _b=0): r(_r), g(_g), b(_b) { }

    bool operator == (const Pixel &other) {
        return (r == other.r) && (g == other.g) && (b == other.b);
    }
    // Comment ce type est imprimé
    // l'opérateur << est un opérateur de la classe std::ostream
    friend ostream& operator << (ostream& o, const Pixel& P) {
        o << "[" << P.r << "," << P.g << "," << P.b << "]" ;
        return o;
    }
};

SC_MODULE(Pixel_A) {
    sc_signal<Pixel> ps;

    SC_CTOR(Pixel_A): ps("ps") {}
};

SC_MODULE(Sum) {
    sc_signal<Pixel> pa, pb;
    Pixel_A pixel_a;
    Pixel_B pixel_b;

    SC_CTOR(Sum): pa("pa"), pb("pb"), pixel_a("pixel_a"), pixel_b("pixel_b") {
        pixel_a.ps(pa);
        pixel_b.ps(pb);
    }
};

int sc_main(int argc, char* argv[]) {
    foo foo_i("foo");
    sc_signal<bool> a, rst_bar, rst_lab;
    sc_signal<int> b, c;

    foo_i.a(a);
    foo_i.b(b);
    foo_i.c(c);
    foo_i.rst_bar(rst_bar);
    foo_i.rst_lab(rst_lab);

    a = false;
    b = 1;
    c = 23;
    rst_bar = 0;
    rst_lab = 1;
    sc_start(0.33,SC_NS);

    a = true;
    b = 0;
    c = 43;
    rst_bar = 1;
    rst_lab = 0;
    sc_start();

    return 0;
}
