#include <systemc.h>

//un type utilisateur
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

// surcharge de la fonction sc_trace pour le type utilisateur
void sc_trace( sc_trace_file* _f, const Pixel& _foo, const std::string& _s ) {
    sc_trace( _f, _foo.r, _s + "_r" );
    sc_trace( _f, _foo.g, _s + "_g" );
    sc_trace( _f, _foo.b, _s + "_b" );
}

// Le test
int sc_main (int argc, char * argv[])
{
    sc_signal<Pixel> P;
    cout << "--> @ " << sc_time_stamp() << " P = " << P << endl;

    sc_trace_file * trace_f;
    trace_f = sc_create_vcd_trace_file ("trace_travail_signal");
    trace_f->set_time_unit(1,SC_NS);
    // On trace notre Pixel
    sc_trace(trace_f, P, "P");

    // affectation au signal
    for (int i=0; i<32; ++i) {
        Pixel pixel = P;
        pixel.r++;
        pixel.g++;
        pixel.b++;
        P = pixel;
        sc_start(1,SC_NS);
        cout << "--> @ " << sc_time_stamp() << " P = " << P << endl;
    }
    return 0;
}
