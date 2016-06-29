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

SC_MODULE(SumPixel_method) {
    sc_in<Pixel> p1, p2;
    sc_out<Pixel> ps;

    void sum() {
        Pixel pc1 = p1;
        Pixel pc2 = p2;
        ps = pc1 + pc2;
    }

    SC_CTOR(SumPixel_method) {
        SC_METHOD (sum);
        sensitive << p1 << p2;
    }
};

SC_MODULE(SumPixel_thread) {
    sc_in<Pixel> p1, p2;
    sc_out<Pixel> ps;

    void sum() {
        while (1) {
            wait();
            Pixel pc1 = p1;
            Pixel pc2 = p2;
            ps = pc1 + pc2;
        }
    }

    SC_CTOR(SumPixel_thread) {
        SC_THREAD(sum);
        sensitive << p1 << p2;
    }
};

SC_MODULE(GEN_METHOD) {
    sc_in<bool> clk, reset_n;
    sc_out<Pixel> ps;

    Pixel p;

    SC_CTOR(GEN_METHOD) : p(0,0,0) {
        SC_METHOD(loop);
        sensitive << clk.pos();
        async_reset_signal_is(reset_n,false);
    }

    void loop() {
        if (!reset_n) {
            p.r = 0;
            p.g = 0;
            p.b = 0;
        }

        if (p.r == 31 && p.g == 63 && p.b == 63) {
            p.r = 0;
            p.b = 0;
            p.g = 0;
        } 
        else if (p.b < p.g && p.b < 63) p.b++;
        else if (((p.g < p.r) || (p.g >= p.r && p.r == 31)) && p.g < 63) p.g++;
        else if (p.r < 31) p.r++;

        ps = p;
    }
};

SC_MODULE(GEN_THREAD) {
    sc_in<bool> clk, reset_n;
    sc_out<Pixel> ps;

    Pixel p;

    SC_CTOR(GEN_THREAD) : p(0,0,0) {
        SC_THREAD(loop);
        sensitive << clk.pos();
        async_reset_signal_is(reset_n,false);
    }

    void loop() {
        while (1) {
            if (!reset_n) {
                p.r = 0;
                p.g = 0;
                p.b = 0;
            }
            wait();

            if (p.r == 31 && p.g == 63 && p.b == 63) {
                p.r = 0;
                p.b = 0;
                p.g = 0;
            } 
            else if (p.b < p.g && p.b < 63) p.b++;
            else if (((p.g < p.r) || (p.g >= p.r && p.r == 31)) && p.g < 63) p.g++;
            else if (p.r < 31) p.r++;

            ps = p;
        }
    }
};

SC_MODULE(GEN_CTHREAD) {
    sc_in<bool> clk, reset_n;
    sc_out<Pixel> ps;

    Pixel p;

    SC_CTOR(GEN_CTHREAD) : p(0,0,0) {
        SC_CTHREAD(loop,clk.pos());
        async_reset_signal_is(reset_n,false);
    }

    void loop() {
        while (1) {
            if (!reset_n) {
                p.r = 0;
                p.g = 0;
                p.b = 0;
            }
            wait();

            if (p.r == 31 && p.g == 63 && p.b == 63) {
                p.r = 0;
                p.b = 0;
                p.g = 0;
            } 
            else if (p.b < p.g && p.b < 63) p.b++;
            else if (((p.g < p.r) || (p.g >= p.r && p.r == 31)) && p.g < 63) p.g++;
            else if (p.r < 31) p.r++;

            ps = p;
        }
    }
};



int sc_main(int argc, char* argv[]) {
    sc_trace_file *trace_f;
    trace_f = sc_create_vcd_trace_file ("my_simu_trace");
    trace_f->set_time_unit(1,SC_NS);

    sc_clock clk("clk",10,SC_NS);
    sc_signal<bool> reset_n;
    sc_signal<Pixel> sp1, sp2, sp_out;
    
    GEN_METHOD gen1("gen1_module");
    gen1.clk(clk);
    gen1.reset_n(reset_n);
    gen1.ps(sp1);

    //GEN_THREAD gen2("gen2_module");
    GEN_CTHREAD gen2("gen2_module");
    gen2.clk(clk);
    gen2.reset_n(reset_n);
    gen2.ps(sp2);

    //SumPixel_method sum("sum_module");
    SumPixel_thread sum("sum_module");
    sum.p1(sp1);
    sum.p2(sp2);
    sum.ps(sp_out);

    sc_trace(trace_f,clk,"clk");
    sc_trace(trace_f,reset_n,"reset_n");
    sc_trace(trace_f,sp1,"sp1");
    sc_trace(trace_f,sp2,"sp2");
    sc_trace(trace_f,sp_out,"sp_out");

    reset_n = true;
    sc_start(50,SC_NS);
    reset_n = false;
    sc_start(50,SC_NS);
    reset_n = true;
    sc_start(3,SC_US);

    sc_close_vcd_trace_file(trace_f);
    return 0;
}
