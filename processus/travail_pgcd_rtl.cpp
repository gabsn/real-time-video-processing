#include <systemc.h>
#include <algorithm>

#define CLK_PERIOD 25

using namespace std;

SC_MODULE(PGCD_hl) {
    sc_in<bool> clk, valid, rst;
    sc_in<uint8_t> a,b;
    sc_out<bool> ready;
    sc_out<uint8_t> c;

    void main();
    uint8_t pgcd(uint8_t a, uint8_t b);

    SC_CTOR(PGCD_hl) {
        SC_THREAD(main);
        sensitive << clk.pos();
        async_reset_signal_is(rst,true);
    };
};

void PGCD_hl::main() {
    c = 0;
    ready = false;

    while (1) {
        // Autre façon de faire, mais moins précise :
        // wait(clk.posedge_event());
        // wait();
        do wait(); while (!valid.read());

        c = pgcd(a.read(),b.read());
        ready = true;
        wait();
        ready = false;
    }
}

uint8_t PGCD_rtl::pgcd(uint8_t _a, uint8_t _b) {
    // Rq : il faut utiliser des signaux quand un objet partagé entre 2 processus est modifié par l'un et observé par l'autre en même temps.
    uint8_t Max, Min, d;
    Max = max(_a,_b);
    Min = min(_a,_b);
    wait();

    if (valid) {
        Max = max(_a,_b);
        Min = min(_a,_b);
    else {
        int d = max-min;
        Max = max(Min,d);
        Min = min(Min,d);
    }
    return Min;
}

int sc_main(int argc, char* argv[]) {
    sc_trace_file *trace_f;
    trace_f = sc_create_vcd_trace_file ("trace_pgcd_caba");
    trace_f->set_time_unit(1,SC_NS);

    PGCD_hl pgcd_i("module_pgcd_caba");

    sc_signal<uint8_t> a, b, c;
    sc_signal<bool> ready, valid, rst;
    sc_clock clk("clk",CLK_PERIOD,SC_NS);

    sc_trace(trace_f, a, "a");
    sc_trace(trace_f, b, "b");
    sc_trace(trace_f, c, "c");
    sc_trace(trace_f, ready, "ready");
    sc_trace(trace_f, ready, "valid");
    sc_trace(trace_f, clk, "clk");
    sc_trace(trace_f, rst, "rst");

    pgcd_i.clk(clk);
    pgcd_i.a(a);
    pgcd_i.b(b);
    pgcd_i.c(c);
    pgcd_i.ready(ready);
    pgcd_i.valid(valid);
    pgcd_i.rst(rst);

    a = 9;
    b = 27;
    valid = true;
    sc_start(CLK_PERIOD,SC_NS);

    do {
        sc_start(CLK_PERIOD,SC_NS); 
        valid = false;
    } while (!ready);

    cout << "PGCD(" << (int) a <<","<< (int) b << ") = " << (int) c << endl;
    sc_start(CLK_PERIOD,SC_NS);

    sc_close_vcd_trace_file(trace_f);

    return 0;
}