namespace Fcf {

  #ifndef FCF_TYPEOF
    #if defined(_MSC_VER)
      #define FCF_TYPEOF(a1) decltype(a1)
    #elif defined(__clang__)
      #define FCF_TYPEOF(a1) __typeof__(a1)
    #else
      #define FCF_TYPEOF(a1) typeof(a1)
    #endif
  #endif

  template <typename F>
  struct ScopeInlineImpl0  {
    F   f;
    ScopeInlineImpl0(const ScopeInlineImpl0&);
    ScopeInlineImpl0(ScopeInlineImpl0&&);
    ScopeInlineImpl0(F a_f)
      : f(a_f) {
    };
    ~ScopeInlineImpl0(){
      try {
        f();
      } catch(...){
      }
    };
  };
  #define FCF_CLOSING_SCOPE0__(a_counter, a_funcBody) \
    typedef void(*func_scope_inline_impl##a_counter)();\
    Fcf::ScopeInlineImpl0<func_scope_inline_impl##a_counter> fcf_scope_inline_impl##a_counter([]() a_funcBody);
  #define FCF_CLOSING_SCOPE0_(a_counter, a_funcBody) FCF_CLOSING_SCOPE0__(a_counter, a_funcBody)
  #define FCF_CLOSING_SCOPE0(a_funcBody) FCF_CLOSING_SCOPE0_(__COUNTER__, a_funcBody)

  template <typename T1, typename F>
  struct ScopeInlineImpl1  {
    T1& a1;
    F   f;
    ScopeInlineImpl1(const ScopeInlineImpl1&);
    ScopeInlineImpl1(ScopeInlineImpl1&&);
    ScopeInlineImpl1(T1& a_a1, F a_f)
      : a1(a_a1), f(a_f) {
    };
    ~ScopeInlineImpl1(){
      try {
        f(a1);
      } catch(...){
      }
    };
  };
  #define FCF_CLOSING_SCOPE1__(a_counter, a_arg1, a_funcBody) \
    typedef FCF_TYPEOF(a_arg1) fcf_scope_inline_impl_##a_counter##_arg1_type; typedef fcf_scope_inline_impl_##a_counter##_arg1_type& fcf_scope_inline_impl_##a_counter##_arg1_ref_type; \
    typedef void(*func_scope_inline_impl##a_counter)(fcf_scope_inline_impl_##a_counter##_arg1_ref_type);\
    Fcf::ScopeInlineImpl1<FCF_TYPEOF(a_arg1), func_scope_inline_impl##a_counter> fcf_scope_inline_impl##a_counter(a_arg1, [](fcf_scope_inline_impl_##a_counter##_arg1_ref_type a_arg1) a_funcBody);
  #define FCF_CLOSING_SCOPE1_(a_counter, a_arg1, a_funcBody) FCF_CLOSING_SCOPE1__(a_counter, a_arg1, a_funcBody)
  #define FCF_CLOSING_SCOPE1(a_arg1, a_funcBody) FCF_CLOSING_SCOPE1_(__COUNTER__, a_arg1, a_funcBody)



  template <typename T1, typename T2, typename F>
  struct ScopeInlineImpl2 {
    T1& a1;
    T2& a2;
    F   f;
    ScopeInlineImpl2(const ScopeInlineImpl2&);
    ScopeInlineImpl2(ScopeInlineImpl2&&);
    ScopeInlineImpl2(T1& a_a1, T2& a_a2, F a_f)
      : a1(a_a1), a2(a_a2), f(a_f) {
    };
    ~ScopeInlineImpl2(){
      try {
        f(a1, a2);
      } catch(...){
      }
    };
  };
  #define FCF_CLOSING_SCOPE2__(a_counter, a_arg1, a_arg2, a_funcBody) \
    typedef FCF_TYPEOF(a_arg1) fcf_scope_inline_impl_##a_counter##_arg1_type; typedef fcf_scope_inline_impl_##a_counter##_arg1_type& fcf_scope_inline_impl_##a_counter##_arg1_ref_type; \
    typedef FCF_TYPEOF(a_arg2) fcf_scope_inline_impl_##a_counter##_arg2_type; typedef fcf_scope_inline_impl_##a_counter##_arg2_type& fcf_scope_inline_impl_##a_counter##_arg2_ref_type; \
    typedef void(*func_scope_inline_impl##a_counter)(fcf_scope_inline_impl_##a_counter##_arg1_ref_type, fcf_scope_inline_impl_##a_counter##_arg2_ref_type);\
    Fcf::ScopeInlineImpl2<FCF_TYPEOF(a_arg1), FCF_TYPEOF(a_arg2), func_scope_inline_impl##a_counter> fcf_scope_inline_impl##a_counter(a_arg1, a_arg2, [](fcf_scope_inline_impl_##a_counter##_arg1_ref_type a_arg1, fcf_scope_inline_impl_##a_counter##_arg2_ref_type a_arg2) a_funcBody);
  #define FCF_CLOSING_SCOPE2_(a_counter, a_arg1, a_arg2, a_funcBody) FCF_CLOSING_SCOPE2__(a_counter, a_arg1, a_arg2, a_funcBody)
  #define FCF_CLOSING_SCOPE2(a_arg1, a_arg2, a_funcBody) FCF_CLOSING_SCOPE2_(__COUNTER__, a_arg1, a_arg2, a_funcBody)


  template <typename T1, typename T2, typename T3, typename F>
  struct ScopeInlineImpl3 {
    T1& a1;
    T2& a2;
    T3& a3;
    F   f;
    ScopeInlineImpl3(const ScopeInlineImpl3&);
    ScopeInlineImpl3(ScopeInlineImpl3&&);
    ScopeInlineImpl3(T1& a_a1, T2& a_a2, T3& a_a3, F a_f)
      : a1(a_a1), a2(a_a2), a3(a_a3), f(a_f) {
    };
    ~ScopeInlineImpl3(){
      try {
        f(a1, a2, a3);
      } catch(...){
      }
    };
  };
  #define FCF_CLOSING_SCOPE3__(a_counter, a_arg1, a_arg2, a_arg3, a_funcBody) \
    typedef FCF_TYPEOF(a_arg1) fcf_scope_inline_impl_##a_counter##_arg1_type; typedef fcf_scope_inline_impl_##a_counter##_arg1_type& fcf_scope_inline_impl_##a_counter##_arg1_ref_type; \
    typedef FCF_TYPEOF(a_arg2) fcf_scope_inline_impl_##a_counter##_arg2_type; typedef fcf_scope_inline_impl_##a_counter##_arg2_type& fcf_scope_inline_impl_##a_counter##_arg2_ref_type; \
    typedef FCF_TYPEOF(a_arg3) fcf_scope_inline_impl_##a_counter##_arg3_type; typedef fcf_scope_inline_impl_##a_counter##_arg3_type& fcf_scope_inline_impl_##a_counter##_arg3_ref_type; \
    typedef void(*func_scope_inline_impl##a_counter)(fcf_scope_inline_impl_##a_counter##_arg1_ref_type, fcf_scope_inline_impl_##a_counter##_arg2_ref_type, fcf_scope_inline_impl_##a_counter##_arg3_ref_type);\
    Fcf::ScopeInlineImpl3<FCF_TYPEOF(a_arg1), FCF_TYPEOF(a_arg2), FCF_TYPEOF(a_arg3), func_scope_inline_impl##a_counter> fcf_scope_inline_impl##a_counter(a_arg1, a_arg2, a_arg3, [](fcf_scope_inline_impl_##a_counter##_arg1_ref_type a_arg1, fcf_scope_inline_impl_##a_counter##_arg2_ref_type a_arg2, fcf_scope_inline_impl_##a_counter##_arg3_ref_type a_arg3) a_funcBody);
  #define FCF_CLOSING_SCOPE3_(a_counter, a_arg1, a_arg2, a_arg3, a_funcBody) FCF_CLOSING_SCOPE3__(a_counter, a_arg1, a_arg2, a_arg3, a_funcBody)
  #define FCF_CLOSING_SCOPE3(a_arg1, a_arg2, a_arg3, a_funcBody) FCF_CLOSING_SCOPE3_(__COUNTER__, a_arg1, a_arg2, a_arg3, a_funcBody)



  template <typename T1, typename T2, typename T3, typename T4, typename F>
  struct ScopeInlineImpl4 {
    T1& a1;
    T2& a2;
    T3& a3;
    T4& a4;
    F   f;
    ScopeInlineImpl4(const ScopeInlineImpl4&);
    ScopeInlineImpl4(ScopeInlineImpl4&&);
    ScopeInlineImpl4(T1& a_a1, T2& a_a2, T3& a_a3, T4& a_a4, F a_f)
      : a1(a_a1), a2(a_a2), a3(a_a3), a4(a_a4), f(a_f) {
    };
    ~ScopeInlineImpl4(){
      try {
        f(a1, a2, a3, a4);
      } catch(...){
      }
    };
  };
  #define FCF_CLOSING_SCOPE4__(a_counter, a_arg1, a_arg2, a_arg3, a_arg4, a_funcBody) \
    typedef FCF_TYPEOF(a_arg1) fcf_scope_inline_impl_##a_counter##_arg1_type; typedef fcf_scope_inline_impl_##a_counter##_arg1_type& fcf_scope_inline_impl_##a_counter##_arg1_ref_type; \
    typedef FCF_TYPEOF(a_arg2) fcf_scope_inline_impl_##a_counter##_arg2_type; typedef fcf_scope_inline_impl_##a_counter##_arg2_type& fcf_scope_inline_impl_##a_counter##_arg2_ref_type; \
    typedef FCF_TYPEOF(a_arg3) fcf_scope_inline_impl_##a_counter##_arg3_type; typedef fcf_scope_inline_impl_##a_counter##_arg3_type& fcf_scope_inline_impl_##a_counter##_arg3_ref_type; \
    typedef FCF_TYPEOF(a_arg4) fcf_scope_inline_impl_##a_counter##_arg4_type; typedef fcf_scope_inline_impl_##a_counter##_arg4_type& fcf_scope_inline_impl_##a_counter##_arg4_ref_type; \
    typedef void(*func_scope_inline_impl##a_counter)(fcf_scope_inline_impl_##a_counter##_arg1_ref_type, \
                                                     fcf_scope_inline_impl_##a_counter##_arg2_ref_type, \
                                                     fcf_scope_inline_impl_##a_counter##_arg3_ref_type, \
                                                     fcf_scope_inline_impl_##a_counter##_arg4_ref_type  \
                                                   );\
    Fcf::ScopeInlineImpl4<FCF_TYPEOF(a_arg1), FCF_TYPEOF(a_arg2), FCF_TYPEOF(a_arg3), FCF_TYPEOF(a_arg4), func_scope_inline_impl##a_counter> \
      fcf_scope_inline_impl##a_counter(\
        a_arg1, \
        a_arg2, \
        a_arg3, \
        a_arg4, \
        []( fcf_scope_inline_impl_##a_counter##_arg1_ref_type a_arg1, \
            fcf_scope_inline_impl_##a_counter##_arg2_ref_type a_arg2, \
            fcf_scope_inline_impl_##a_counter##_arg3_ref_type a_arg3, \
            fcf_scope_inline_impl_##a_counter##_arg4_ref_type a_arg4  \
          ) a_funcBody);
  #define FCF_CLOSING_SCOPE4_(a_counter, a_arg1, a_arg2, a_arg3, a_arg4, a_funcBody) FCF_CLOSING_SCOPE4__(a_counter, a_arg1, a_arg2, a_arg3, a_arg4, a_funcBody)
  #define FCF_CLOSING_SCOPE4(a_arg1, a_arg2, a_arg3, a_arg4, a_funcBody) FCF_CLOSING_SCOPE4_(__COUNTER__, a_arg1, a_arg2, a_arg3, a_arg4, a_funcBody)



  template <typename T1, typename T2, typename T3, typename T4, typename T5, typename F>
  struct ScopeInlineImpl5 {
    T1& a1;
    T2& a2;
    T3& a3;
    T4& a4;
    T5& a5;
    F   f;
    ScopeInlineImpl5(const ScopeInlineImpl5&);
    ScopeInlineImpl5(ScopeInlineImpl5&&);
    ScopeInlineImpl5(T1& a_a1, T2& a_a2, T3& a_a3, T4& a_a4, T5& a_a5, F a_f)
      : a1(a_a1), a2(a_a2), a3(a_a3), a4(a_a4), a5(a_a5), f(a_f) {
    };
    ~ScopeInlineImpl5(){
      try {
        f(a1, a2, a3, a4, a5);
      } catch(...){
      }
    };
  };
  #define FCF_CLOSING_SCOPE5__(a_counter, a_arg1, a_arg2, a_arg3, a_arg4, a_arg5, a_funcBody) \
    typedef FCF_TYPEOF(a_arg1) fcf_scope_inline_impl_##a_counter##_arg1_type; typedef fcf_scope_inline_impl_##a_counter##_arg1_type& fcf_scope_inline_impl_##a_counter##_arg1_ref_type; \
    typedef FCF_TYPEOF(a_arg2) fcf_scope_inline_impl_##a_counter##_arg2_type; typedef fcf_scope_inline_impl_##a_counter##_arg2_type& fcf_scope_inline_impl_##a_counter##_arg2_ref_type; \
    typedef FCF_TYPEOF(a_arg3) fcf_scope_inline_impl_##a_counter##_arg3_type; typedef fcf_scope_inline_impl_##a_counter##_arg3_type& fcf_scope_inline_impl_##a_counter##_arg3_ref_type; \
    typedef FCF_TYPEOF(a_arg4) fcf_scope_inline_impl_##a_counter##_arg4_type; typedef fcf_scope_inline_impl_##a_counter##_arg4_type& fcf_scope_inline_impl_##a_counter##_arg4_ref_type; \
    typedef FCF_TYPEOF(a_arg5) fcf_scope_inline_impl_##a_counter##_arg5_type; typedef fcf_scope_inline_impl_##a_counter##_arg5_type& fcf_scope_inline_impl_##a_counter##_arg5_ref_type; \
    typedef void(*func_scope_inline_impl##a_counter)(fcf_scope_inline_impl_##a_counter##_arg1_ref_type, \
                                                     fcf_scope_inline_impl_##a_counter##_arg2_ref_type, \
                                                     fcf_scope_inline_impl_##a_counter##_arg3_ref_type, \
                                                     fcf_scope_inline_impl_##a_counter##_arg4_ref_type, \
                                                     fcf_scope_inline_impl_##a_counter##_arg5_ref_type  \
                                                   );\
    Fcf::ScopeInlineImpl5<FCF_TYPEOF(a_arg1), FCF_TYPEOF(a_arg2), FCF_TYPEOF(a_arg3), FCF_TYPEOF(a_arg4), FCF_TYPEOF(a_arg5), func_scope_inline_impl##a_counter> \
      fcf_scope_inline_impl##a_counter(\
        a_arg1, \
        a_arg2, \
        a_arg3, \
        a_arg4, \
        a_arg5, \
        []( fcf_scope_inline_impl_##a_counter##_arg1_ref_type a_arg1, \
            fcf_scope_inline_impl_##a_counter##_arg2_ref_type a_arg2, \
            fcf_scope_inline_impl_##a_counter##_arg3_ref_type a_arg3, \
            fcf_scope_inline_impl_##a_counter##_arg4_ref_type a_arg4, \
            fcf_scope_inline_impl_##a_counter##_arg5_ref_type a_arg5  \
          ) a_funcBody);
  #define FCF_CLOSING_SCOPE5_(a_counter, a_arg1, a_arg2, a_arg3, a_arg4, a_arg5, a_funcBody) FCF_CLOSING_SCOPE5__(a_counter, a_arg1, a_arg2, a_arg3, a_arg4, a_arg5, a_funcBody)
  #define FCF_CLOSING_SCOPE5(a_arg1, a_arg2, a_arg3, a_arg4, a_arg5, a_funcBody) FCF_CLOSING_SCOPE5_(__COUNTER__, a_arg1, a_arg2, a_arg3, a_arg4, a_arg5, a_funcBody)



  template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename F>
  struct ScopeInlineImpl6 {
    T1& a1;
    T2& a2;
    T3& a3;
    T4& a4;
    T5& a5;
    T6& a6;
    F   f;
    ScopeInlineImpl6(const ScopeInlineImpl6&);
    ScopeInlineImpl6(ScopeInlineImpl6&&);
    ScopeInlineImpl6(T1& a_a1, T2& a_a2, T3& a_a3, T4& a_a4, T5& a_a5, T6& a_a6, F a_f)
      : a1(a_a1), a2(a_a2), a3(a_a3), a4(a_a4), a5(a_a5), a6(a_a6), f(a_f) {
    };
    ~ScopeInlineImpl6(){
      try {
        f(a1, a2, a3, a4, a5, a6);
      } catch(...){
      }
    };
  };
  #define FCF_CLOSING_SCOPE6__(a_counter, a_arg1, a_arg2, a_arg3, a_arg4, a_arg5, a_arg6, a_funcBody) \
    typedef FCF_TYPEOF(a_arg1) fcf_scope_inline_impl_##a_counter##_arg1_type; typedef fcf_scope_inline_impl_##a_counter##_arg1_type& fcf_scope_inline_impl_##a_counter##_arg1_ref_type; \
    typedef FCF_TYPEOF(a_arg2) fcf_scope_inline_impl_##a_counter##_arg2_type; typedef fcf_scope_inline_impl_##a_counter##_arg2_type& fcf_scope_inline_impl_##a_counter##_arg2_ref_type; \
    typedef FCF_TYPEOF(a_arg3) fcf_scope_inline_impl_##a_counter##_arg3_type; typedef fcf_scope_inline_impl_##a_counter##_arg3_type& fcf_scope_inline_impl_##a_counter##_arg3_ref_type; \
    typedef FCF_TYPEOF(a_arg4) fcf_scope_inline_impl_##a_counter##_arg4_type; typedef fcf_scope_inline_impl_##a_counter##_arg4_type& fcf_scope_inline_impl_##a_counter##_arg4_ref_type; \
    typedef FCF_TYPEOF(a_arg5) fcf_scope_inline_impl_##a_counter##_arg5_type; typedef fcf_scope_inline_impl_##a_counter##_arg5_type& fcf_scope_inline_impl_##a_counter##_arg5_ref_type; \
    typedef FCF_TYPEOF(a_arg6) fcf_scope_inline_impl_##a_counter##_arg6_type; typedef fcf_scope_inline_impl_##a_counter##_arg6_type& fcf_scope_inline_impl_##a_counter##_arg6_ref_type; \
    typedef void(*func_scope_inline_impl##a_counter)(fcf_scope_inline_impl_##a_counter##_arg1_ref_type, \
                                                     fcf_scope_inline_impl_##a_counter##_arg2_ref_type, \
                                                     fcf_scope_inline_impl_##a_counter##_arg3_ref_type, \
                                                     fcf_scope_inline_impl_##a_counter##_arg4_ref_type, \
                                                     fcf_scope_inline_impl_##a_counter##_arg5_ref_type, \
                                                     fcf_scope_inline_impl_##a_counter##_arg6_ref_type  \
                                                   );\
    Fcf::ScopeInlineImpl6<FCF_TYPEOF(a_arg1), FCF_TYPEOF(a_arg2), FCF_TYPEOF(a_arg3), FCF_TYPEOF(a_arg4), FCF_TYPEOF(a_arg5), FCF_TYPEOF(a_arg6), func_scope_inline_impl##a_counter> \
      fcf_scope_inline_impl##a_counter(\
        a_arg1, \
        a_arg2, \
        a_arg3, \
        a_arg4, \
        a_arg5, \
        a_arg6, \
        []( fcf_scope_inline_impl_##a_counter##_arg1_ref_type a_arg1, \
            fcf_scope_inline_impl_##a_counter##_arg2_ref_type a_arg2, \
            fcf_scope_inline_impl_##a_counter##_arg3_ref_type a_arg3, \
            fcf_scope_inline_impl_##a_counter##_arg4_ref_type a_arg4, \
            fcf_scope_inline_impl_##a_counter##_arg5_ref_type a_arg5, \
            fcf_scope_inline_impl_##a_counter##_arg6_ref_type a_arg6  \
          ) a_funcBody);
  #define FCF_CLOSING_SCOPE6_(a_counter, a_arg1, a_arg2, a_arg3, a_arg4, a_arg5, a_arg6, a_funcBody) FCF_CLOSING_SCOPE6__(a_counter, a_arg1, a_arg2, a_arg3, a_arg4, a_arg5, a_arg6, a_funcBody)
  #define FCF_CLOSING_SCOPE6(a_arg1, a_arg2, a_arg3, a_arg4, a_arg5, a_arg6, a_funcBody) FCF_CLOSING_SCOPE6_(__COUNTER__, a_arg1, a_arg2, a_arg3, a_arg4, a_arg5, a_arg6, a_funcBody)



  template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename F>
  struct ScopeInlineImpl7 {
    T1& a1;
    T2& a2;
    T3& a3;
    T4& a4;
    T5& a5;
    T6& a6;
    T7& a7;
    F   f;
    ScopeInlineImpl7(const ScopeInlineImpl7&);
    ScopeInlineImpl7(ScopeInlineImpl7&&);
    ScopeInlineImpl7(T1& a_a1, T2& a_a2, T3& a_a3, T4& a_a4, T5& a_a5, T6& a_a6, T7& a_a7, F a_f)
      : a1(a_a1), a2(a_a2), a3(a_a3), a4(a_a4), a5(a_a5), a6(a_a6), a7(a_a7), f(a_f) {
    };
    ~ScopeInlineImpl7(){
      try {
        f(a1, a2, a3, a4, a5, a6, a7);
      } catch(...){
      }
    };
  };
  #define FCF_CLOSING_SCOPE7__(a_counter, a_arg1, a_arg2, a_arg3, a_arg4, a_arg5, a_arg6, a_arg7, a_funcBody) \
    typedef FCF_TYPEOF(a_arg1) fcf_scope_inline_impl_##a_counter##_arg1_type; typedef fcf_scope_inline_impl_##a_counter##_arg1_type& fcf_scope_inline_impl_##a_counter##_arg1_ref_type; \
    typedef FCF_TYPEOF(a_arg2) fcf_scope_inline_impl_##a_counter##_arg2_type; typedef fcf_scope_inline_impl_##a_counter##_arg2_type& fcf_scope_inline_impl_##a_counter##_arg2_ref_type; \
    typedef FCF_TYPEOF(a_arg3) fcf_scope_inline_impl_##a_counter##_arg3_type; typedef fcf_scope_inline_impl_##a_counter##_arg3_type& fcf_scope_inline_impl_##a_counter##_arg3_ref_type; \
    typedef FCF_TYPEOF(a_arg4) fcf_scope_inline_impl_##a_counter##_arg4_type; typedef fcf_scope_inline_impl_##a_counter##_arg4_type& fcf_scope_inline_impl_##a_counter##_arg4_ref_type; \
    typedef FCF_TYPEOF(a_arg5) fcf_scope_inline_impl_##a_counter##_arg5_type; typedef fcf_scope_inline_impl_##a_counter##_arg5_type& fcf_scope_inline_impl_##a_counter##_arg5_ref_type; \
    typedef FCF_TYPEOF(a_arg6) fcf_scope_inline_impl_##a_counter##_arg6_type; typedef fcf_scope_inline_impl_##a_counter##_arg6_type& fcf_scope_inline_impl_##a_counter##_arg6_ref_type; \
    typedef FCF_TYPEOF(a_arg7) fcf_scope_inline_impl_##a_counter##_arg7_type; typedef fcf_scope_inline_impl_##a_counter##_arg7_type& fcf_scope_inline_impl_##a_counter##_arg7_ref_type; \
    typedef void(*func_scope_inline_impl##a_counter)(fcf_scope_inline_impl_##a_counter##_arg1_ref_type, \
                                                     fcf_scope_inline_impl_##a_counter##_arg2_ref_type, \
                                                     fcf_scope_inline_impl_##a_counter##_arg3_ref_type, \
                                                     fcf_scope_inline_impl_##a_counter##_arg4_ref_type, \
                                                     fcf_scope_inline_impl_##a_counter##_arg5_ref_type, \
                                                     fcf_scope_inline_impl_##a_counter##_arg6_ref_type, \
                                                     fcf_scope_inline_impl_##a_counter##_arg7_ref_type  \
                                                   );\
    Fcf::ScopeInlineImpl7<FCF_TYPEOF(a_arg1), FCF_TYPEOF(a_arg2), FCF_TYPEOF(a_arg3), FCF_TYPEOF(a_arg4), FCF_TYPEOF(a_arg5), FCF_TYPEOF(a_arg6), FCF_TYPEOF(a_arg7), func_scope_inline_impl##a_counter> \
      fcf_scope_inline_impl##a_counter(\
        a_arg1, \
        a_arg2, \
        a_arg3, \
        a_arg4, \
        a_arg5, \
        a_arg6, \
        a_arg7, \
        []( fcf_scope_inline_impl_##a_counter##_arg1_ref_type a_arg1, \
            fcf_scope_inline_impl_##a_counter##_arg2_ref_type a_arg2, \
            fcf_scope_inline_impl_##a_counter##_arg3_ref_type a_arg3, \
            fcf_scope_inline_impl_##a_counter##_arg4_ref_type a_arg4, \
            fcf_scope_inline_impl_##a_counter##_arg5_ref_type a_arg5, \
            fcf_scope_inline_impl_##a_counter##_arg6_ref_type a_arg6, \
            fcf_scope_inline_impl_##a_counter##_arg7_ref_type a_arg7  \
          ) a_funcBody);
  #define FCF_CLOSING_SCOPE7_(a_counter, a_arg1, a_arg2, a_arg3, a_arg4, a_arg5, a_arg6, a_arg7, a_funcBody) FCF_CLOSING_SCOPE7__(a_counter, a_arg1, a_arg2, a_arg3, a_arg4, a_arg5, a_arg6, a_arg7, a_funcBody)
  #define FCF_CLOSING_SCOPE7(a_arg1, a_arg2, a_arg3, a_arg4, a_arg5, a_arg6, a_arg7, a_funcBody) FCF_CLOSING_SCOPE7_(__COUNTER__, a_arg1, a_arg2, a_arg3, a_arg4, a_arg5, a_arg6, a_arg7, a_funcBody)



  template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename F>
  struct ScopeInlineImpl8 {
    T1& a1;
    T2& a2;
    T3& a3;
    T4& a4;
    T5& a5;
    T6& a6;
    T7& a7;
    T8& a8;
    F   f;
    ScopeInlineImpl8(const ScopeInlineImpl8&);
    ScopeInlineImpl8(ScopeInlineImpl8&&);
    ScopeInlineImpl8(T1& a_a1, T2& a_a2, T3& a_a3, T4& a_a4, T5& a_a5, T6& a_a6, T7& a_a7, T8& a_a8, F a_f)
      : a1(a_a1), a2(a_a2), a3(a_a3), a4(a_a4), a5(a_a5), a6(a_a6), a7(a_a7), a8(a_a8), f(a_f) {
    };
    ~ScopeInlineImpl8(){
      try {
        f(a1, a2, a3, a4, a5, a6, a7, a8);
      } catch(...){
      }
    };
  };
  #define FCF_CLOSING_SCOPE8__(a_counter, a_arg1, a_arg2, a_arg3, a_arg4, a_arg5, a_arg6, a_arg7, a_arg8, a_funcBody) \
    typedef FCF_TYPEOF(a_arg1) fcf_scope_inline_impl_##a_counter##_arg1_type; typedef fcf_scope_inline_impl_##a_counter##_arg1_type& fcf_scope_inline_impl_##a_counter##_arg1_ref_type; \
    typedef FCF_TYPEOF(a_arg2) fcf_scope_inline_impl_##a_counter##_arg2_type; typedef fcf_scope_inline_impl_##a_counter##_arg2_type& fcf_scope_inline_impl_##a_counter##_arg2_ref_type; \
    typedef FCF_TYPEOF(a_arg3) fcf_scope_inline_impl_##a_counter##_arg3_type; typedef fcf_scope_inline_impl_##a_counter##_arg3_type& fcf_scope_inline_impl_##a_counter##_arg3_ref_type; \
    typedef FCF_TYPEOF(a_arg4) fcf_scope_inline_impl_##a_counter##_arg4_type; typedef fcf_scope_inline_impl_##a_counter##_arg4_type& fcf_scope_inline_impl_##a_counter##_arg4_ref_type; \
    typedef FCF_TYPEOF(a_arg5) fcf_scope_inline_impl_##a_counter##_arg5_type; typedef fcf_scope_inline_impl_##a_counter##_arg5_type& fcf_scope_inline_impl_##a_counter##_arg5_ref_type; \
    typedef FCF_TYPEOF(a_arg6) fcf_scope_inline_impl_##a_counter##_arg6_type; typedef fcf_scope_inline_impl_##a_counter##_arg6_type& fcf_scope_inline_impl_##a_counter##_arg6_ref_type; \
    typedef FCF_TYPEOF(a_arg7) fcf_scope_inline_impl_##a_counter##_arg7_type; typedef fcf_scope_inline_impl_##a_counter##_arg7_type& fcf_scope_inline_impl_##a_counter##_arg7_ref_type; \
    typedef FCF_TYPEOF(a_arg8) fcf_scope_inline_impl_##a_counter##_arg8_type; typedef fcf_scope_inline_impl_##a_counter##_arg8_type& fcf_scope_inline_impl_##a_counter##_arg8_ref_type; \
    typedef void(*func_scope_inline_impl##a_counter)(fcf_scope_inline_impl_##a_counter##_arg1_ref_type, \
                                                     fcf_scope_inline_impl_##a_counter##_arg2_ref_type, \
                                                     fcf_scope_inline_impl_##a_counter##_arg3_ref_type, \
                                                     fcf_scope_inline_impl_##a_counter##_arg4_ref_type, \
                                                     fcf_scope_inline_impl_##a_counter##_arg5_ref_type, \
                                                     fcf_scope_inline_impl_##a_counter##_arg6_ref_type, \
                                                     fcf_scope_inline_impl_##a_counter##_arg7_ref_type, \
                                                     fcf_scope_inline_impl_##a_counter##_arg8_ref_type  \
                                                   );\
    Fcf::ScopeInlineImpl8<FCF_TYPEOF(a_arg1), FCF_TYPEOF(a_arg2), FCF_TYPEOF(a_arg3), FCF_TYPEOF(a_arg4), FCF_TYPEOF(a_arg5), FCF_TYPEOF(a_arg6), FCF_TYPEOF(a_arg7), FCF_TYPEOF(a_arg8), func_scope_inline_impl##a_counter> \
      fcf_scope_inline_impl##a_counter(\
        a_arg1, \
        a_arg2, \
        a_arg3, \
        a_arg4, \
        a_arg5, \
        a_arg6, \
        a_arg7, \
        a_arg8, \
        []( fcf_scope_inline_impl_##a_counter##_arg1_ref_type a_arg1, \
            fcf_scope_inline_impl_##a_counter##_arg2_ref_type a_arg2, \
            fcf_scope_inline_impl_##a_counter##_arg3_ref_type a_arg3, \
            fcf_scope_inline_impl_##a_counter##_arg4_ref_type a_arg4, \
            fcf_scope_inline_impl_##a_counter##_arg5_ref_type a_arg5, \
            fcf_scope_inline_impl_##a_counter##_arg6_ref_type a_arg6, \
            fcf_scope_inline_impl_##a_counter##_arg7_ref_type a_arg7, \
            fcf_scope_inline_impl_##a_counter##_arg8_ref_type a_arg8  \
          ) a_funcBody);
  #define FCF_CLOSING_SCOPE8_(a_counter, a_arg1, a_arg2, a_arg3, a_arg4, a_arg5, a_arg6, a_arg7, a_arg8, a_funcBody) FCF_CLOSING_SCOPE8__(a_counter, a_arg1, a_arg2, a_arg3, a_arg4, a_arg5, a_arg6, a_arg7, a_arg8, a_funcBody)
  #define FCF_CLOSING_SCOPE8(a_arg1, a_arg2, a_arg3, a_arg4, a_arg5, a_arg6, a_arg7, a_arg8, a_funcBody) FCF_CLOSING_SCOPE8_(__COUNTER__, a_arg1, a_arg2, a_arg3, a_arg4, a_arg5, a_arg6, a_arg7, a_arg8, a_funcBody)



  template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename F>
  struct ScopeInlineImpl9 {
    T1& a1;
    T2& a2;
    T3& a3;
    T4& a4;
    T5& a5;
    T6& a6;
    T7& a7;
    T8& a8;
    T9& a9;
    F   f;
    ScopeInlineImpl9(const ScopeInlineImpl9&);
    ScopeInlineImpl9(ScopeInlineImpl9&&);
    ScopeInlineImpl9(T1& a_a1, T2& a_a2, T3& a_a3, T4& a_a4, T5& a_a5, T6& a_a6, T7& a_a7, T8& a_a8, T9& a_a9, F a_f)
      : a1(a_a1), a2(a_a2), a3(a_a3), a4(a_a4), a5(a_a5), a6(a_a6), a7(a_a7), a8(a_a8), a9(a_a9), f(a_f) {
    };
    ~ScopeInlineImpl9(){
      try {
        f(a1, a2, a3, a4, a5, a6, a7, a8, a9);
      } catch(...){
      }
    };
  };
  #define FCF_CLOSING_SCOPE9__(a_counter, a_arg1, a_arg2, a_arg3, a_arg4, a_arg5, a_arg6, a_arg7, a_arg8, a_arg9, a_funcBody) \
    typedef FCF_TYPEOF(a_arg1) fcf_scope_inline_impl_##a_counter##_arg1_type; typedef fcf_scope_inline_impl_##a_counter##_arg1_type& fcf_scope_inline_impl_##a_counter##_arg1_ref_type; \
    typedef FCF_TYPEOF(a_arg2) fcf_scope_inline_impl_##a_counter##_arg2_type; typedef fcf_scope_inline_impl_##a_counter##_arg2_type& fcf_scope_inline_impl_##a_counter##_arg2_ref_type; \
    typedef FCF_TYPEOF(a_arg3) fcf_scope_inline_impl_##a_counter##_arg3_type; typedef fcf_scope_inline_impl_##a_counter##_arg3_type& fcf_scope_inline_impl_##a_counter##_arg3_ref_type; \
    typedef FCF_TYPEOF(a_arg4) fcf_scope_inline_impl_##a_counter##_arg4_type; typedef fcf_scope_inline_impl_##a_counter##_arg4_type& fcf_scope_inline_impl_##a_counter##_arg4_ref_type; \
    typedef FCF_TYPEOF(a_arg5) fcf_scope_inline_impl_##a_counter##_arg5_type; typedef fcf_scope_inline_impl_##a_counter##_arg5_type& fcf_scope_inline_impl_##a_counter##_arg5_ref_type; \
    typedef FCF_TYPEOF(a_arg6) fcf_scope_inline_impl_##a_counter##_arg6_type; typedef fcf_scope_inline_impl_##a_counter##_arg6_type& fcf_scope_inline_impl_##a_counter##_arg6_ref_type; \
    typedef FCF_TYPEOF(a_arg7) fcf_scope_inline_impl_##a_counter##_arg7_type; typedef fcf_scope_inline_impl_##a_counter##_arg7_type& fcf_scope_inline_impl_##a_counter##_arg7_ref_type; \
    typedef FCF_TYPEOF(a_arg8) fcf_scope_inline_impl_##a_counter##_arg8_type; typedef fcf_scope_inline_impl_##a_counter##_arg8_type& fcf_scope_inline_impl_##a_counter##_arg8_ref_type; \
    typedef FCF_TYPEOF(a_arg9) fcf_scope_inline_impl_##a_counter##_arg9_type; typedef fcf_scope_inline_impl_##a_counter##_arg9_type& fcf_scope_inline_impl_##a_counter##_arg9_ref_type; \
    typedef void(*func_scope_inline_impl##a_counter)(fcf_scope_inline_impl_##a_counter##_arg1_ref_type, \
                                                     fcf_scope_inline_impl_##a_counter##_arg2_ref_type, \
                                                     fcf_scope_inline_impl_##a_counter##_arg3_ref_type, \
                                                     fcf_scope_inline_impl_##a_counter##_arg4_ref_type, \
                                                     fcf_scope_inline_impl_##a_counter##_arg5_ref_type, \
                                                     fcf_scope_inline_impl_##a_counter##_arg6_ref_type, \
                                                     fcf_scope_inline_impl_##a_counter##_arg7_ref_type, \
                                                     fcf_scope_inline_impl_##a_counter##_arg8_ref_type, \
                                                     fcf_scope_inline_impl_##a_counter##_arg9_ref_type  \
                                                   );\
    Fcf::ScopeInlineImpl9<FCF_TYPEOF(a_arg1), FCF_TYPEOF(a_arg2), FCF_TYPEOF(a_arg3), FCF_TYPEOF(a_arg4), FCF_TYPEOF(a_arg5), FCF_TYPEOF(a_arg6), FCF_TYPEOF(a_arg7), FCF_TYPEOF(a_arg8), FCF_TYPEOF(a_arg9), func_scope_inline_impl##a_counter> \
      fcf_scope_inline_impl##a_counter(\
        a_arg1, \
        a_arg2, \
        a_arg3, \
        a_arg4, \
        a_arg5, \
        a_arg6, \
        a_arg7, \
        a_arg8, \
        a_arg9, \
        []( fcf_scope_inline_impl_##a_counter##_arg1_ref_type a_arg1, \
            fcf_scope_inline_impl_##a_counter##_arg2_ref_type a_arg2, \
            fcf_scope_inline_impl_##a_counter##_arg3_ref_type a_arg3, \
            fcf_scope_inline_impl_##a_counter##_arg4_ref_type a_arg4, \
            fcf_scope_inline_impl_##a_counter##_arg5_ref_type a_arg5, \
            fcf_scope_inline_impl_##a_counter##_arg6_ref_type a_arg6, \
            fcf_scope_inline_impl_##a_counter##_arg7_ref_type a_arg7, \
            fcf_scope_inline_impl_##a_counter##_arg8_ref_type a_arg8, \
            fcf_scope_inline_impl_##a_counter##_arg9_ref_type a_arg9  \
          ) a_funcBody);
  #define FCF_CLOSING_SCOPE9_(a_counter, a_arg1, a_arg2, a_arg3, a_arg4, a_arg5, a_arg6, a_arg7, a_arg8, a_arg9, a_funcBody) FCF_CLOSING_SCOPE9__(a_counter, a_arg1, a_arg2, a_arg3, a_arg4, a_arg5, a_arg6, a_arg7, a_arg8, a_arg9, a_funcBody)
  #define FCF_CLOSING_SCOPE9(a_arg1, a_arg2, a_arg3, a_arg4, a_arg5, a_arg6, a_arg7, a_arg8, a_arg9, a_funcBody) FCF_CLOSING_SCOPE9_(__COUNTER__, a_arg1, a_arg2, a_arg3, a_arg4, a_arg5, a_arg6, a_arg7, a_arg8, a_arg9, a_funcBody)



  template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename F>
  struct ScopeInlineImpl10 {
    T1& a1;
    T2& a2;
    T3& a3;
    T4& a4;
    T5& a5;
    T6& a6;
    T7& a7;
    T8& a8;
    T9& a9;
    T10& a10;
    F   f;
    ScopeInlineImpl10(const ScopeInlineImpl10&);
    ScopeInlineImpl10(ScopeInlineImpl10&&);
    ScopeInlineImpl10(T1& a_a1, T2& a_a2, T3& a_a3, T4& a_a4, T5& a_a5, T6& a_a6, T7& a_a7, T8& a_a8, T9& a_a9, T10& a_a10, F a_f)
      : a1(a_a1), a2(a_a2), a3(a_a3), a4(a_a4), a5(a_a5), a6(a_a6), a7(a_a7), a8(a_a8), a9(a_a9), a10(a_a10), f(a_f) {
    };
    ~ScopeInlineImpl10(){
      try {
        f(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
      } catch(...){
      }
    };
  };
  #define FCF_CLOSING_SCOPE10__(a_counter, a_arg1, a_arg2, a_arg3, a_arg4, a_arg5, a_arg6, a_arg7, a_arg8, a_arg9, a_arg10, a_funcBody) \
    typedef FCF_TYPEOF(a_arg1) fcf_scope_inline_impl_##a_counter##_arg1_type; typedef fcf_scope_inline_impl_##a_counter##_arg1_type& fcf_scope_inline_impl_##a_counter##_arg1_ref_type; \
    typedef FCF_TYPEOF(a_arg2) fcf_scope_inline_impl_##a_counter##_arg2_type; typedef fcf_scope_inline_impl_##a_counter##_arg2_type& fcf_scope_inline_impl_##a_counter##_arg2_ref_type; \
    typedef FCF_TYPEOF(a_arg3) fcf_scope_inline_impl_##a_counter##_arg3_type; typedef fcf_scope_inline_impl_##a_counter##_arg3_type& fcf_scope_inline_impl_##a_counter##_arg3_ref_type; \
    typedef FCF_TYPEOF(a_arg4) fcf_scope_inline_impl_##a_counter##_arg4_type; typedef fcf_scope_inline_impl_##a_counter##_arg4_type& fcf_scope_inline_impl_##a_counter##_arg4_ref_type; \
    typedef FCF_TYPEOF(a_arg5) fcf_scope_inline_impl_##a_counter##_arg5_type; typedef fcf_scope_inline_impl_##a_counter##_arg5_type& fcf_scope_inline_impl_##a_counter##_arg5_ref_type; \
    typedef FCF_TYPEOF(a_arg6) fcf_scope_inline_impl_##a_counter##_arg6_type; typedef fcf_scope_inline_impl_##a_counter##_arg6_type& fcf_scope_inline_impl_##a_counter##_arg6_ref_type; \
    typedef FCF_TYPEOF(a_arg7) fcf_scope_inline_impl_##a_counter##_arg7_type; typedef fcf_scope_inline_impl_##a_counter##_arg7_type& fcf_scope_inline_impl_##a_counter##_arg7_ref_type; \
    typedef FCF_TYPEOF(a_arg8) fcf_scope_inline_impl_##a_counter##_arg8_type; typedef fcf_scope_inline_impl_##a_counter##_arg8_type& fcf_scope_inline_impl_##a_counter##_arg8_ref_type; \
    typedef FCF_TYPEOF(a_arg9) fcf_scope_inline_impl_##a_counter##_arg9_type; typedef fcf_scope_inline_impl_##a_counter##_arg9_type& fcf_scope_inline_impl_##a_counter##_arg9_ref_type; \
    typedef FCF_TYPEOF(a_arg10) fcf_scope_inline_impl_##a_counter##_arg10_type; typedef fcf_scope_inline_impl_##a_counter##_arg10_type& fcf_scope_inline_impl_##a_counter##_arg10_ref_type; \
    typedef void(*func_scope_inline_impl##a_counter)(fcf_scope_inline_impl_##a_counter##_arg1_ref_type, \
                                                     fcf_scope_inline_impl_##a_counter##_arg2_ref_type, \
                                                     fcf_scope_inline_impl_##a_counter##_arg3_ref_type, \
                                                     fcf_scope_inline_impl_##a_counter##_arg4_ref_type, \
                                                     fcf_scope_inline_impl_##a_counter##_arg5_ref_type, \
                                                     fcf_scope_inline_impl_##a_counter##_arg6_ref_type, \
                                                     fcf_scope_inline_impl_##a_counter##_arg7_ref_type, \
                                                     fcf_scope_inline_impl_##a_counter##_arg8_ref_type, \
                                                     fcf_scope_inline_impl_##a_counter##_arg9_ref_type, \
                                                     fcf_scope_inline_impl_##a_counter##_arg10_ref_type \
                                                   );\
    Fcf::ScopeInlineImpl10<FCF_TYPEOF(a_arg1), FCF_TYPEOF(a_arg2), FCF_TYPEOF(a_arg3), FCF_TYPEOF(a_arg4), FCF_TYPEOF(a_arg5), FCF_TYPEOF(a_arg6), FCF_TYPEOF(a_arg7), FCF_TYPEOF(a_arg8), FCF_TYPEOF(a_arg9), FCF_TYPEOF(a_arg10), func_scope_inline_impl##a_counter> \
      fcf_scope_inline_impl##a_counter(\
        a_arg1, \
        a_arg2, \
        a_arg3, \
        a_arg4, \
        a_arg5, \
        a_arg6, \
        a_arg7, \
        a_arg8, \
        a_arg9, \
        a_arg10,\
        []( fcf_scope_inline_impl_##a_counter##_arg1_ref_type a_arg1, \
            fcf_scope_inline_impl_##a_counter##_arg2_ref_type a_arg2, \
            fcf_scope_inline_impl_##a_counter##_arg3_ref_type a_arg3, \
            fcf_scope_inline_impl_##a_counter##_arg4_ref_type a_arg4, \
            fcf_scope_inline_impl_##a_counter##_arg5_ref_type a_arg5, \
            fcf_scope_inline_impl_##a_counter##_arg6_ref_type a_arg6, \
            fcf_scope_inline_impl_##a_counter##_arg7_ref_type a_arg7, \
            fcf_scope_inline_impl_##a_counter##_arg8_ref_type a_arg8, \
            fcf_scope_inline_impl_##a_counter##_arg9_ref_type a_arg9, \
            fcf_scope_inline_impl_##a_counter##_arg10_ref_type a_arg10 \
          ) a_funcBody);
  #define FCF_CLOSING_SCOPE10_(a_counter, a_arg1, a_arg2, a_arg3, a_arg4, a_arg5, a_arg6, a_arg7, a_arg8, a_arg9, a_arg10, a_funcBody) FCF_CLOSING_SCOPE10__(a_counter, a_arg1, a_arg2, a_arg3, a_arg4, a_arg5, a_arg6, a_arg7, a_arg8, a_arg9, a_arg10, a_funcBody)
  #define FCF_CLOSING_SCOPE10(a_arg1, a_arg2, a_arg3, a_arg4, a_arg5, a_arg6, a_arg7, a_arg8, a_arg9, a_arg10, a_funcBody) FCF_CLOSING_SCOPE10_(__COUNTER__, a_arg1, a_arg2, a_arg3, a_arg4, a_arg5, a_arg6, a_arg7, a_arg8, a_arg9, a_arg10, a_funcBody)



  #define FCF_CLOSING_SCOPE_RESOLVE(a1) a1
  #define FCF_CLOSING_SCOPE_COUNT_ARGUMENTS_(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, ...) a12
  #define FCF_CLOSING_SCOPE_COUNT_ARGUMENTS(...) FCF_CLOSING_SCOPE_RESOLVE(FCF_CLOSING_SCOPE_COUNT_ARGUMENTS_(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))
  #define FCF_CLOSING_SCOPE_MACRO_NAME__(a_name, a_counter) a_name##a_counter
  #define FCF_CLOSING_SCOPE_MACRO_NAME_(a_name, a_counter) FCF_CLOSING_SCOPE_MACRO_NAME__(a_name, a_counter)
  #define FCF_CLOSING_SCOPE_MACRO_NAME(...) FCF_CLOSING_SCOPE_MACRO_NAME_(FCF_CLOSING_SCOPE, FCF_CLOSING_SCOPE_COUNT_ARGUMENTS(__VA_ARGS__))
  #define FCF_CLOSING_SCOPE(...) FCF_CLOSING_SCOPE_MACRO_NAME(__VA_ARGS__)(__VA_ARGS__);

} // Fcf namespace
