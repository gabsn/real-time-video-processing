#include <systemc.h>

struct Pixel {
    sc_uint<5> r;
    sc_uint<6> g;
    sc_uint<6> b;

    // Constructeur
    Pixel (sc_uint<5> _r=0, sc_uint<6> _g=0, sc_uint<5> _b=0): r(_r), g(_g), b(_b) {}

    // Surcharge d'opérateurs
    bool operator == (const Pixel &other) {
        return (r == other.r) && (g == other.g) && (b == other.b);
    }

    friend ostream& operator << (ostream& o, const Pixel& P) {
        o << "[" << P.r << "," << P.g << "," << P.b << "]" ;
        return o;
    }

    Pixel operator + (const Pixel p2) {
        Pixel ps(0,0,0);
        ps.r = (r + p2.r > 31) ? 31 : r + p2.r;
        ps.g = (g + p2.g > 63) ? 63 : g + p2.g;
        ps.b = (b + p2.b > 63) ? 63 : b + p2.b;

        return ps;
    }
};

// surcharge de la fonction sc_trace pour la struct Pixel
void sc_trace(sc_trace_file* _f, const Pixel& p, const std::string& s)  {
    sc_trace(_f,p.r,s+"_R");
    sc_trace(_f,p.g,s+"_G");
    sc_trace(_f,p.b,s+"_B");
}

int sc_main(int argc, char* argv[]) {
    sc_trace_file *trace_f;
    trace_f = sc_create_vcd_trace_file ("my_simu_trace");
    trace_f->set_time_unit(1,SC_NS);

    sc_signal<Pixel> sp;
    Pixel p(0,0,0);

    sc_trace(trace_f, sp, "Pixel");

    sp = p;
    sc_start(10,SC_NS);

    for (int i=0; i<64; ++i) {
        cout << p << endl;
        if (p.r < 31) p.r++;
        sp = p;
        sc_start(10,SC_NS);
        if (p.g < 63) p.g++;
        sp = p;
        sc_start(10,SC_NS);
        if (p.b < 63) p.b++;
        sp = p;
        sc_start(10,SC_NS);
    }
   
    sc_close_vcd_trace_file(trace_f);

    return 0;
}
