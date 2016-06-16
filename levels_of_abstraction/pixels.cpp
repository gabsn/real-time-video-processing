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

SC_MODULE(SumPixel) {
    sc_in<Pixel> p1, p2;
    sc_out<Pixel> ps;

    void sum() {
        Pixel pc1 = p1;
        Pixel pc2 = p2;
        ps = pc1 + pc2;
    }

    SC_CTOR(SumPixel) {
        SC_METHOD (sum);
        sensitive << p1 << p2;
    }
};

int sc_main(int argc, char* argv[]) {
    SumPixel sum_module("sum_module");
    sc_signal<Pixel> sp1, sp2, sps;
    Pixel p1(10,23,3), p2(5,20,2), ps(0,0,0);

    sum_module.p1(sp1);
    sum_module.p2(sp2);
    sum_module.ps(sps);

    sp1 = p1;
    sp2 = p2;
    sps = ps;
    cout << "p1 : " << p1 << " and p2 : " << p2 << endl;
    cout << "ps : " << ps << endl;
    sc_start(0.133,SC_NS);

    p1.g = 55;
    sp1 = p1;
    sc_start(0.133,SC_NS);
    cout << "p1 : " << p1 << " and p2 : " << p2 << endl;
    cout << "ps : " << ps << endl;
    
    return 0;
}
