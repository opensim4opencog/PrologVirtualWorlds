class HelloWorld {
    public native void displayHelloWorld();
    public native void displayHelloWorld2();

    static {
        System.loadLibrary("hello");
    }
}
