#include <systemc.h>
#include <algorithm>

using namespace std;

SC_MODULE(PGCD_m) {
    sc_in<bool> clk, ready;
    sc_in<uint> a,b;
    sc_out<bool> valid;
    sc_out<uint> c;

    uint Max, Min, d;

    void loop() {
        while (1) {
            wait(ready.posedge_event());
            int ia, ib, ic;
            ia = a;
            ib = b;
            ic = pgcd(ia,ib);
            c = ic;
            valid = true;
        }
    }

    uint pgcd(uint _a, uint _b) {
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
    PGCD_m pgcd_i("pgcd_module");

    sc_signal<uint> a, b, c;
    sc_signal<bool> ready, valid;
    sc_clock clk("clk",25,SC_NS);

    pgcd_i.clk(clk);
    pgcd_i.a(a);
    pgcd_i.b(b);
    pgcd_i.c(c);
    pgcd_i.ready(ready);
    pgcd_i.valid(valid);

    ready = false;
    valid = false;
    a = 30;
    b = 45;
    sc_start(50,SC_NS);

    ready = true;
    sc_start(50,SC_NS);

    cout << "PGCD(" << a <<","<< b << ") = " << c << endl;

    return 0;
}
