mod native {
    extern {
        pub fn setup();
        pub fn _yield();
    }
}

pub fn setup() {
    unsafe { native::setup() }
}

pub fn _yield() {
    unsafe { native::_yield() }
}
