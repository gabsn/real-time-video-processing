#include <systemc.h>

#define max(x,y) ((x > y) ? x : y)
#define min(x,y) ((x < y) ? x : y)

#define CLK_PERIOD 25,SC_NS

using namespace std;

SC_MODULE(PGCD_rtl) {
    sc_in<bool> clk, valid, rst;
    sc_in<uint8_t> a,b;
    sc_out<bool> ready;
    sc_out<uint8_t> c;

    uint8_t Max, Min;

    void control();
    void data_path();

    SC_CTOR(PGCD_rtl) {
        Max = 1;
        Min = 0;

        //async_reset_signal_is(rst,true);

        SC_METHOD(control);
        sensitive << clk.pos();
        SC_METHOD(data_path);
        sensitive << clk.pos();
    };
};

void PGCD_rtl::control() {
    if (Max == Min && ready == false)
        ready = true;
    else
        ready = false;
}

void PGCD_rtl::data_path() {
    // Rq : il faut utiliser des signaux quand un objet partagé entre 2 processus est modifié par l'un et observé par l'autre en même temps.
    if (valid) {
        Max = max(a,b);
        Min = min(a,b);
    } else {
        int d = Max-Min;
        Max = max(Min,d);
        Min = min(Min,d);
    }

    c = Min;
}

uint8_t pgcd(uint8_t _a, uint8_t _b) {
    uint8_t Max, Min, d;
    Max = max(_a,_b);
    Min = min(_a,_b);

    while (Max != Min) {
        d = Max-Min;
        Max = max(Min,d);
        Min = min(Min,d);
    }

    return Min;
}

int sc_main(int argc, char* argv[]) {
    sc_trace_file *trace_f;
    trace_f = sc_create_vcd_trace_file ("trace_PGCD_rtl");
    trace_f->set_time_unit(1,SC_NS);

    PGCD_rtl pgcd_i("module_PGCD_rtl");

    sc_signal<uint8_t> a("a"), b("b"), c("c");
    sc_signal<bool> ready("ready"), valid("valid"), rst("rst");
    sc_clock clk("clk",CLK_PERIOD);

    sc_trace(trace_f, a, "a");
    sc_trace(trace_f, b, "b");
    sc_trace(trace_f, c, "c");
    sc_trace(trace_f, ready, "ready");
    sc_trace(trace_f, valid, "valid");
    sc_trace(trace_f, clk, "clk");
    sc_trace(trace_f, rst, "rst");

    pgcd_i.clk(clk);
    pgcd_i.a(a);
    pgcd_i.b(b);
    pgcd_i.c(c);
    pgcd_i.ready(ready);
    pgcd_i.valid(valid);
    pgcd_i.rst(rst);

    for (int i=0; i<50; ++i) {
        a = rand() % 256;
        b = rand() % 256;
        //cout << "i = " << i << " a = " << (int)a << " b = " << (int)b << " and i = " << i << endl;
        valid = true;
        sc_start(CLK_PERIOD);

        valid = false;
        while (!ready) {
            sc_start(CLK_PERIOD); 
        }

        if (c == pgcd(a,b))
            cout << "PGCD(" << (int) a << "," << (int) b << ") = " << (int) c << endl;
        else
            cout << "Wrong answer : PGCD(" << (int) a << "," << (int) b << ") != " << (int) c << endl;

        sc_start(CLK_PERIOD);
    }

    sc_close_vcd_trace_file(trace_f);

    return 0;
}
