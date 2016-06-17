#include <systemc.h>

#define max(x,y) ((x > y) ? x : y)
#define min(x,y) ((x < y) ? x : y)

#define CLK_PERIOD 25,SC_NS

using namespace std;

SC_MODULE(PGCD_rtl) {
    sc_in<bool> clk, valid;
    sc_in<uint8_t> a,b;
    sc_out<bool> ready;
    sc_out<uint8_t> c;

    uint8_t Max, Min;

    void control();
    void data_path();

    SC_CTOR(PGCD_rtl) {
        Max = 1;
        Min = 0;

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
    } else if (a == 0 || b == 0) {
        Min = 1;
        Max = 1;
    } else {
        int d = Max-Min;
        Max = max(Min,d);
        Min = min(Min,d);
    }

    c = Min;
}

SC_MODULE(PGCD_caba) {
    sc_in<bool> clk, valid;
    sc_in<uint8_t> a,b;
    sc_out<bool> ready;
    sc_out<uint8_t> c;

    void main();
    uint8_t pgcd(uint8_t a, uint8_t b);

    SC_CTOR(PGCD_caba) {
        SC_THREAD(main);
        sensitive << clk.pos();
    };
};

uint8_t PGCD_caba::pgcd(uint8_t _a, uint8_t _b) {
    if (_a == 0 || _b == 0)
        return 1;
    uint8_t Max, Min, d;
    Max = max(_a,_b);
    Min = min(_a,_b);
    wait();

    while (Max != Min) {
        d = Max-Min;
        Max = max(Min,d);
        Min = min(Min,d);
        wait();
    }

    return Min;
}

void PGCD_caba::main() {
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
uint8_t pgcd(uint8_t _a, uint8_t _b) {
    if (_a == 0 || _b == 0)
        return 1;
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
    trace_f = sc_create_vcd_trace_file ("trace_caba_vs_rtl");
    trace_f->set_time_unit(1,SC_NS);

    sc_clock clk("clk",CLK_PERIOD);
    sc_trace(trace_f, clk, "clk");

    PGCD_rtl pgcd_rtl("module_PGCD_rtl");
    PGCD_caba pgcd_caba("module_PGCD_caba");

    sc_signal<uint8_t> a("a"), b("b"), c("c");
    sc_signal<bool> ready("ready"), valid("valid");

    sc_signal<uint8_t> a_caba("a_caba"), b_caba("b_caba"), c_caba("c_caba");
    sc_signal<bool> ready_caba("ready_caba"), valid_caba("valid_caba");

    sc_trace(trace_f, a, "a");
    sc_trace(trace_f, b, "b");
    sc_trace(trace_f, c, "c");
    sc_trace(trace_f, ready, "ready");
    sc_trace(trace_f, valid, "valid");

    sc_trace(trace_f, a_caba, "a_caba");
    sc_trace(trace_f, b_caba, "b_caba");
    sc_trace(trace_f, c_caba, "c_caba");
    sc_trace(trace_f, ready_caba, "ready_caba");
    sc_trace(trace_f, valid_caba, "valid_caba");

    pgcd_rtl.clk(clk);
    pgcd_rtl.a(a);
    pgcd_rtl.b(b);
    pgcd_rtl.c(c);
    pgcd_rtl.ready(ready);
    pgcd_rtl.valid(valid);

    pgcd_caba.clk(clk);
    pgcd_caba.a(a_caba);
    pgcd_caba.b(b_caba);
    pgcd_caba.c(c_caba);
    pgcd_caba.ready(ready_caba);
    pgcd_caba.valid(valid_caba);

    int rtl(0), caba(0);
    for (int i=0; i<100; ++i) {
        a = rand() % 256;
        b = rand() % 256;
        a_caba = a;
        b_caba = b;

        valid = true;
        valid_caba = true;
        sc_start(CLK_PERIOD);

        valid = false;
        valid_caba = false;
        while (!ready && !ready_caba) {
            sc_start(CLK_PERIOD); 
        }
        
        if (ready) {
            rtl++;
            cout << "RTL finished first" << endl;
        } else {
            caba++;
            cout << "CABA finished first" << endl;
        }

        sc_start(CLK_PERIOD);
    }

    cout << endl << "------------------------------------" << endl;
    cout << "Nb times RTL finished first : " << rtl << endl;
    cout << "Nb times CABA finished first : " << caba << endl;

    sc_close_vcd_trace_file(trace_f);

    return 0;
}
