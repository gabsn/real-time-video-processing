#include <systemc.h>

#include <cstdlib>

#define CLK_PERIOD 15,SC_NS

SC_MODULE(Test_Buffer) {
    sc_in<int> signal, buffer;
    
    SC_CTOR(Test_Buffer) { 
        SC_THREAD(signal_thread); 
        sensitive << signal;
        SC_THREAD(buffer_thread); 
        sensitive << buffer;
    }

    void signal_thread() {
        while (1) {
            wait();
            cout << "signal received at " << sc_time_stamp() << endl;
        }
    }

    void buffer_thread() {
        while (1) {
            wait();
            cout << "buffer received at " << sc_time_stamp() << endl;
        }
    }
};

int sc_main(int, char **) {
    sc_core::sc_report_handler::set_actions( "/IEEE_Std_1666/deprecated",sc_core::SC_DO_NOTHING );

    sc_signal<int> signal;
    sc_buffer<int> buffer;

    Test_Buffer mod("test_buffer");
    mod.signal(signal);
    mod.buffer(buffer);

    signal = 0;
    buffer = 0;
    cout << "signal = 0 & buffer = 0 at " << sc_time_stamp() << endl;
    sc_start(CLK_PERIOD);

    signal = 1;
    buffer = 1;
    cout << "signal = 1 & buffer = 1 at " << sc_time_stamp() << endl;
    sc_start(CLK_PERIOD);

    signal = 1;
    buffer = 1;
    cout << "signal = 1 & buffer = 1 at " << sc_time_stamp() << endl;
    sc_start(CLK_PERIOD);

    return 0;
}
