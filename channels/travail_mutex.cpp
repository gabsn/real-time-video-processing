#include <systemc.h>

#include <cstdlib>

#define CLK_PERIOD 15,SC_NS

SC_MODULE(Test_Mutex) {
    sc_in<bool> clk;
    sc_int<8> count;
    sc_mutex padlock;
    
    SC_CTOR(Test_Mutex) { 
        count = 0;

        SC_CTHREAD(thread_1,clk.pos()); 
        SC_CTHREAD(thread_2,clk.pos()); 
    }

    void thread_1() {
        while (1) {
            wait(1);
            padlock.lock();
            count++;
            cout << "thread 1 : count = " << count << endl;
            if (count == 5) {
                cout << "thread_1 reached a count of 5 at " << sc_time_stamp() << endl; 
                sc_stop();
            }
            padlock.unlock();
            wait(5);
        }
    }

    void thread_2() {
        while (1) {
            wait(3);
            padlock.lock();
            count--;
            cout << "thread 2 : count = " << count << endl;
            if (count == -5) {
                cout << "thread_2 reached a count of -5 at " << sc_time_stamp() << endl; 
                sc_stop();
            }
            padlock.unlock();
            wait(2);
        }
    }
};

int sc_main(int, char **) {
    sc_core::sc_report_handler::set_actions( "/IEEE_Std_1666/deprecated",sc_core::SC_DO_NOTHING );

    sc_clock clk("clk",CLK_PERIOD);

    Test_Mutex mod("test_mutex");
    mod.clk(clk);

    sc_start();

    return 0;
}
