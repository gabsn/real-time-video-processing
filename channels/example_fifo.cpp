#include <systemc.h>

SC_MODULE(A) {
    sc_fifo_out<int> out;
    void loop() {
        int i = 0;
        for(;;){
            //write est obligatoire
            out.write(i);
            i++;
            wait(33, SC_NS);
        }
    }
    SC_CTOR(A) { SC_THREAD(loop); }
};

SC_MODULE(B) {
    sc_fifo_in<int> in;
    void loop() {
        wait(300, SC_NS);
        for(;;){
            // in.read() appelle in->read()
            cout << "Lecture 1 : " << in->read() << "@ " << sc_time_stamp() << endl;
            cout << "Lecture 2 : " << in.read()  << "@ " << sc_time_stamp() << endl;
            wait(55, SC_NS);
        }
    }
    SC_CTOR(B) { SC_THREAD(loop); }
};

int sc_main(int, char **) {
    // une fifo de 10 entiers
    sc_fifo<int> fifo(10);

    A a("modA");
    a.out(fifo);

    B b("modB");
    b.in(fifo);

    sc_start(300,SC_NS);
    cout 
        << "contenu de la fifo @" << sc_time_stamp() << endl
        << fifo << endl;

    sc_start(1,SC_US);
    cout 
        << "contenu de la fifo @" << sc_time_stamp() << endl
        << fifo << endl;

    return 0;
}
