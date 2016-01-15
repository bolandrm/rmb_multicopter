pub mod types {
    #[allow(non_camel_case_types)] pub type c_size_t = usize;

    #[allow(non_camel_case_types)] pub type c_char = u8;
    #[allow(non_camel_case_types)] pub type c_schar = i8;
    #[allow(non_camel_case_types)] pub type c_uchar = u8;

    #[allow(non_camel_case_types)] pub type c_float = f32;
    #[allow(non_camel_case_types)] pub type c_double = f64;

    #[allow(non_camel_case_types)] pub type c_int = i32;
    #[allow(non_camel_case_types)] pub type c_uint = u32;
    #[allow(non_camel_case_types)] pub type c_long = i32;

    #[allow(non_camel_case_types, dead_code)]
    #[repr(u8)]
    pub enum c_void {
        #[doc(hidden)] __variant1,
        #[doc(hidden)] __variant2,
    }
}

mod native {
    extern {
        pub fn setup();
        pub fn _loop();
        pub fn _yield();
    }
}

pub fn setup() {
    unsafe { native::setup() }
}

pub fn _loop() {
    unsafe { native::_loop() }
}

pub fn _yield() {
    unsafe { native::_yield() }
}
