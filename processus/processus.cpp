#include <systemc.h>

SC_MODULE(foo) {
    sc_in<bool> a, rst_bar, rst_lab;
    sc_in<int> b;
    sc_in<int> c;

    SC_CTOR(foo) {
        // Le thread bar est sensible aux évènements sur a, b et c
        SC_THREAD(bar);
        sensitive << a << b << c;
        reset_signal_is(rst_bar, true);
        // Le thread lab est sensible à l’évènement ”front montant” de a
        SC_THREAD(lab);
        sensitive << a.pos();
        async_reset_signal_is(rst_lab, false);
    }
    void bar() {
        int i(0);
        if (rst_bar) {
                cout << "reset bar : " << i << endl;
                i = 0;
        }
        while (1) {
            // attendre un évènement sur a, b ou c
            wait();
            cout << "bar : événement reçu" << endl;
            i++;
        }
    }
    void lab() {
        int i(0);
        if (!rst_lab) {
                cout << "reset lab : " << i << endl;
                i = 0;
        } 
        while (1) {
            // attendre un front montant de ”a”
            wait();
            cout << "front montant de a : " << i << endl;
            i++;
        }
    }
};

int sc_main(int argc, char* argv[]) {
    foo foo_i("foo");
    sc_signal<bool> a, rst_bar, rst_lab;
    sc_signal<int> b, c;

    foo_i.a(a);
    foo_i.b(b);
    foo_i.c(c);
    foo_i.rst_bar(rst_bar);
    foo_i.rst_lab(rst_lab);

    a = false;
    b = 1;
    c = 23;
    rst_bar = 0;
    rst_lab = 1;
    sc_start(0.33,SC_NS);

    a = true;
    b = 0;
    c = 43;
    rst_bar = 1;
    rst_lab = 0;
    sc_start();

    return 0;
}
