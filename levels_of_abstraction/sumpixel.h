SC_MODULE(SumPixel) {
    sc_in<Pixel> p1, p2;
    sc_out<Pixel> ps;

    SC_CTOR (SumPixel) {
        SC_METHOD (sum);
        sensitive << p1 << p2;
    }

    void sum();
};
