#include <systemc.h>
#include <algorithm>

#define CLK_PERIOD 25

using namespace std;

SC_MODULE(PGCD_m) {
    sc_in<bool> clk, ready;
    sc_in<uint8_t> a,b;
    sc_out<bool> valid;
    sc_out<uint8_t> c;

    uint8_t Max, Min, d;

    void loop() {
        while (1) {
            wait(ready.posedge_event());
            uint8_t ia, ib, ic;
            ia = a;
            ib = b;
            ic = pgcd(ia,ib);
            c = ic;
            valid = true;
        }
    }

    uint8_t pgcd(uint8_t _a, uint8_t _b) {
        Max = max(_a,_b);
        Min = min(_a,_b);

        while (Max != Min) {
            d = Max-Min;
            Max = max(Min,d);
            Min = min(Min,d);
        }

        return Min;
    }

    SC_CTOR(PGCD_m) {
        SC_THREAD(loop);
        sensitive << clk.pos();
    }
};

int sc_main(int argc, char* argv[]) {
    sc_trace_file *trace_f;
    trace_f = sc_create_vcd_trace_file ("trace_pgcd");
    trace_f->set_time_unit(1,SC_NS);

    PGCD_m pgcd_i("pgcd_module");

    sc_signal<uint8_t> a, b, c;
    sc_signal<bool> ready, valid;
    sc_clock clk("clk",CLK_PERIOD,SC_NS);

    sc_trace(trace_f, a, "a");
    sc_trace(trace_f, b, "b");
    sc_trace(trace_f, c, "c");
    sc_trace(trace_f, ready, "ready");
    sc_trace(trace_f, ready, "valid");
    sc_trace(trace_f, clk, "clk");

    pgcd_i.clk(clk);
    pgcd_i.a(a);
    pgcd_i.b(b);
    pgcd_i.c(c);
    pgcd_i.ready(ready);
    pgcd_i.valid(valid);

    ready = false;
    valid = false;
    a = 1;
    b = 7;
    sc_start(CLK_PERIOD,SC_NS);
    valid = true;
    sc_start(CLK_PERIOD,SC_NS);

    ready = true;
    sc_start(CLK_PERIOD,SC_NS);

    cout << "PGCD(" << (int)a <<","<< (int)b << ") = " << (int)c << endl;
    cout << "ready : " << ready << endl;

    sc_close_vcd_trace_file(trace_f);

    return 0;
}
