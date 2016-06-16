#include <systemc.h>
#include <algorithm>

#define CLK_PERIOD 15,SC_NS

using namespace std;

SC_MODULE(PGCD_hl) {
    sc_in<bool> clk, valid, rst;
    sc_in<uint8_t> a,b;
    sc_out<bool> ready;
    sc_out<uint8_t> c;

    void main();

    SC_CTOR(PGCD_hl) {
        SC_THREAD(main);
        sensitive << clk.pos();
        async_reset_signal_is(rst,true);
    };
};

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

int sc_main(int argc, char* argv[]) {
    sc_trace_file *trace_f;
    trace_f = sc_create_vcd_trace_file ("trace_pgcd_hl");
    trace_f->set_time_unit(1,SC_NS);

    PGCD_hl pgcd_i("module_pgcd_hl");

    sc_signal<uint8_t> a, b, c;
    sc_signal<bool> ready, valid, rst;
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

    rst = false;

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
